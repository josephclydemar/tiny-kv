#include "core/kv_server.h"
#include "network/server_socket.h"
#include "network/client_socket.h"
#include "util/debugging.h"
#include "util/logging.h"
#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <asm-generic/socket.h>
#include <string.h>
#include <sys/socket.h>

#define RECV_BUF_SIZE         50
#define SEND_BUF_SIZE         50

kv_server_t kv_serv;
server_socket_t serv_socket;


static inline void kv_conn_server_init(kv_server_t* const kv_server)
{
  ASSERT("PTHREAD_MUTEX_INIT",
    pthread_mutex_init(
      &kv_serv.kv_conn_queue_lock,
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

  kv_server->kv_conn_queue.back_idx = 0;
  kv_server->kv_conn_queue.back_idx = 0;
  kv_server->kv_conn_queue.count = 0;
  kv_server->kv_conn_queue.cap = MAX_CONN_BACKLOG;
  kv_server->kv_conn_queue.kv_conn_fd_buf = malloc(sizeof(int) * MAX_CONN_BACKLOG);
}

int kv_conn_enqueue(kv_conn_queue_t* const kv_conn_queue, int kv_conn_fd)
{
  if (kv_conn_queue->count == kv_conn_queue->cap) return -1;
  if (kv_conn_queue->back_idx >= kv_conn_queue->cap) {
    kv_conn_queue->back_idx = kv_conn_queue->back_idx % kv_conn_queue->cap;
  }
  *(kv_conn_queue->kv_conn_fd_buf + kv_conn_queue->back_idx) = kv_conn_fd;
  kv_conn_queue->back_idx++;
  kv_conn_queue->count++;
  return 0;
}

int kv_conn_dequeue(kv_conn_queue_t* const kv_conn_queue)
{
  if (kv_conn_queue->count == 0) return -1;
  int kv_conn_fd = *(kv_conn_queue->kv_conn_fd_buf + kv_conn_queue->front_idx);
  if (kv_conn_queue->front_idx == 0) kv_conn_queue->front_idx = kv_conn_queue->cap - 1;
  else kv_conn_queue->front_idx--;
  kv_conn_queue->count--;
  return kv_conn_fd;
}

void* kv_ctrl_fn(void* param)
{
  (void)param;
  client_socket_t clnt_socket;

  server_socket_setup(&serv_socket, MAX_CONN_BACKLOG);
  client_socket_setup(&clnt_socket);
  kv_conn_server_init(&kv_serv);

  while (true) {
    sem_wait(&kv_serv.kv_conn_handler_ready_sem);

    clnt_socket.client_socket_fd = accept(
      serv_socket.server_socket_fd,
      (struct sockaddr *)&clnt_socket.client_addr,
      &clnt_socket.client_addr_size
    );
    ASSERT("ACCEPT", clnt_socket.client_socket_fd >= 0);

    pthread_mutex_lock(&kv_serv.kv_conn_queue_lock);
    ASSERT("KV_CONN_ENQUEUE",
      kv_conn_enqueue(
        &kv_serv.kv_conn_queue,
        clnt_socket.client_socket_fd
      ) == 0
    );
    pthread_mutex_unlock(&kv_serv.kv_conn_queue_lock);
    pthread_cond_signal(&kv_serv.kv_conn_arrival_cond);
  }

  pthread_exit(NULL);
}

void* kv_conn_handler_fn(void* param)
{
  (void)param;
  client_socket_t clnt_socket;
  client_socket_setup(&clnt_socket);

  char recv_buf[RECV_BUF_SIZE];
  char send_buf[SEND_BUF_SIZE];
  while (true) {
    pthread_mutex_lock(&kv_serv.kv_conn_queue_lock);
    clnt_socket.client_socket_fd = kv_conn_dequeue(&kv_serv.kv_conn_queue);
    ASSERT("KV_CONN_ENQUEUE",
      clnt_socket.client_socket_fd == 0
    );
    pthread_mutex_unlock(&kv_serv.kv_conn_queue_lock);

    memset(recv_buf, 0, RECV_BUF_SIZE);
    ASSERT("RECV",
      recv(
        clnt_socket.client_socket_fd,
        recv_buf,
        RECV_BUF_SIZE - 1,
        0
      ) >= 0
    );
    DEBUG_LOG("MSG", "[%s]", recv_buf);

    ASSERT("SEND",
      send(
        clnt_socket.client_socket_fd,
        send_buf,
        SEND_BUF_SIZE,
        0
      ) >= 0
    );

    close(clnt_socket.client_socket_fd);
    sem_post(&kv_serv.kv_conn_handler_ready_sem);
  }
  pthread_exit(NULL);
}

