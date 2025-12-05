#ifndef KV_FD_RQUEUE_H_
#define KV_FD_RQUEUE_H_

#include <pthread.h>
#include <semaphore.h>

typedef struct kv_fd_rqueue_t {
  int* kv_fd_buf;
  size_t front_idx, back_idx;
  size_t len, cap;
} kv_fd_rqueue_t;

/* returns 0 on success, -1 on fail */
int kv_fd_rqueue_init(kv_fd_rqueue_t* const kv_fd_rqueue, size_t size);

/* returns 0 on success, -1 on fail */
int kv_fd_rqueue_free(kv_fd_rqueue_t *const kv_fd_rqueue);

/* returns 0 on success, -1 on fail */
int kv_fd_renqueue(kv_fd_rqueue_t* const kv_fd_rqueue, int kv_fd);

/* returns integer at front of queue on success, -1 on fail */
int kv_fd_rdequeue(kv_fd_rqueue_t* const kv_fd_rqueue);

#endif // KV_FD_RQUEUE_H_

