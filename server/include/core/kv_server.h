#ifndef KV_SERVER_H_
#define KV_SERVER_H_

#include <pthread.h>
#include <semaphore.h>

#define MAX_CONN_HANDLERS 5
#define MAX_CONN_BACKLOG  100

typedef struct kv_conn_queue_t {
  int* kv_conn_fd_buf;
  size_t front_idx, back_idx;
  size_t count, cap;
} kv_conn_queue_t;

typedef struct kv_server_t {
  pthread_mutex_t kv_conn_queue_lock;
  pthread_cond_t kv_conn_arrival_cond;
  sem_t kv_conn_handler_ready_sem;
  kv_conn_queue_t kv_conn_queue;
  pthread_t kv_ctrl_thread, kv_conn_handlers[MAX_CONN_HANDLERS];
} kv_server_t;
extern kv_server_t kv_serv;


int kv_conn_enqueue(kv_conn_queue_t* const kv_conn_queue, int kv_conn_fd);
int kv_conn_dequeue(kv_conn_queue_t* const kv_conn_queue);

void* kv_ctrl_fn(void* param);
void* kv_conn_handler_fn(void* param);

#endif // KV_SERVER_H_

