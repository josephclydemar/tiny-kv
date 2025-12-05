#ifndef KV_SERVER_H_
#define KV_SERVER_H_

#include <pthread.h>
#include <semaphore.h>
#include "kv_fd_rqueue.h"

#define MAX_CONN_HANDLERS 5
#define MAX_CONN_BACKLOG  100

typedef struct kv_server_t {
  struct {
    pthread_mutex_t lock;
    kv_fd_rqueue_t kv_q;
  } kv_conn_queue;
  pthread_cond_t kv_conn_arrival_cond;
  sem_t kv_conn_handler_ready_sem;
  pthread_t kv_ctrl_thread, kv_conn_handler_threads[MAX_CONN_HANDLERS];
} kv_server_t;
extern kv_server_t kv_serv;

void* kv_ctrl_fn(void* param);
void* kv_conn_handler_fn(void* param);

#endif // KV_SERVER_H_

