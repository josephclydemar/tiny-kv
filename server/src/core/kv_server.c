#include "core/kv_server.h"
#include "core/kv_fd_rqueue.h"
#include "network/server_socket.h"
#include "network/client_socket.h"
#include "util/debugging.h"
#include "util/logging.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include <asm-generic/socket.h>
#include <string.h>
#include <sys/socket.h>

#define PORT                  "5800"
#define RECV_BUF_SIZE         4096
#define SEND_BUF_SIZE         1024

kv_server_t kv_serv;
server_socket_t serv_socket;


static inline void kv_conn_server_init(kv_server_t* const kv_server)
{
  ASSERT("PTHREAD_MUTEX_INIT",
    pthread_mutex_init(
      &kv_serv.kv_conn_queue.lock,
      NULL
    ) == 0
  );
  ASSERT("PTHREAD_COND_INIT",
    pthread_cond_init(
      &kv_serv.kv_conn_arrival_cond,
      NULL
    ) == 0
  );
  ASSERT("SEM_INIT",
    sem_init(
      &kv_serv.kv_conn_handler_ready_sem,
      0,
      MAX_CONN_HANDLERS
    ) == 0
  );

  kv_fd_rqueue_init(&kv_server->kv_conn_queue.kv_q, MAX_CONN_BACKLOG);
}

void* kv_ctrl_fn(void* param)
{
  LOG_ONLY_EXPR(param);
  client_socket_t clnt_socket;

  server_socket_setup(&serv_socket, PORT, MAX_CONN_BACKLOG);
  client_socket_setup(&clnt_socket);
  kv_conn_server_init(&kv_serv);

  DEBUG_LOG("CONTROLLER", "[PORT: %s] %s", PORT, (char *)param);

  while (true) {
    sem_wait(&kv_serv.kv_conn_handler_ready_sem);

    clnt_socket.client_socket_fd = accept(
      serv_socket.server_socket_fd,
      (struct sockaddr *)&(clnt_socket.client_addr),
      &clnt_socket.client_addr_size
    );
    ASSERT("ACCEPT", clnt_socket.client_socket_fd >= 0);

    pthread_mutex_lock(&kv_serv.kv_conn_queue.lock);
    ASSERT("KV_FD_RENQUEUE",
      kv_fd_renqueue(
        &kv_serv.kv_conn_queue.kv_q,
        clnt_socket.client_socket_fd
      ) == 0
    );
    pthread_mutex_unlock(&kv_serv.kv_conn_queue.lock);
    pthread_cond_signal(&kv_serv.kv_conn_arrival_cond);
  }

  pthread_exit(NULL);
}

void* kv_conn_handler_fn(void* param)
{
  LOG_ONLY_EXPR(param);
  int client_socket_fd = 0;
  char recv_buf[RECV_BUF_SIZE] = {0};
  char send_buf[SEND_BUF_SIZE] = {0};

  pthread_t tid = pthread_self();
  INFO_LOG("HANDLER", "[0x%lx] %s", tid, (char *)param);
  snprintf(send_buf, SEND_BUF_SIZE - 1, "FROM tid: 0x%lx", tid);
  while (true) {
    pthread_mutex_lock(&kv_serv.kv_conn_queue.lock);
    while (kv_serv.kv_conn_queue.kv_q.len <= 0) {
      pthread_cond_wait(&kv_serv.kv_conn_arrival_cond, &kv_serv.kv_conn_queue.lock);
    }
    client_socket_fd = kv_fd_rdequeue(&kv_serv.kv_conn_queue.kv_q);
    ASSERT("KV_FD_RDEQUEUE", client_socket_fd > -1);
    pthread_mutex_unlock(&kv_serv.kv_conn_queue.lock);

    memset(recv_buf, 0, RECV_BUF_SIZE);
    ASSERT("RECV",
      recv(
        client_socket_fd,
        recv_buf,
        RECV_BUF_SIZE - 1,
        0
      ) >= 0
    );
    INFO_LOG("RECV_MSG", YELLOW_COLOR_TEXT "%s" DEFAULT_COLOR_TEXT, recv_buf);

    ASSERT("SEND",
      send(
        client_socket_fd,
        send_buf,
        SEND_BUF_SIZE,
        0
      ) >= 0
    );

    close(client_socket_fd);
    sem_post(&kv_serv.kv_conn_handler_ready_sem);
  }
  pthread_exit(NULL);
}

