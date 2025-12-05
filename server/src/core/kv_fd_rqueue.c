#include "core/kv_fd_rqueue.h"
#include <stdlib.h>

int kv_fd_rqueue_init(kv_fd_rqueue_t* const kv_fd_rqueue, size_t size)
{
  kv_fd_rqueue->front_idx = 0;
  kv_fd_rqueue->back_idx = 0;
  kv_fd_rqueue->len = 0;
  kv_fd_rqueue->cap = size;
  kv_fd_rqueue->kv_fd_buf = malloc(sizeof(int) * size);
  if (kv_fd_rqueue->kv_fd_buf == NULL) return -1;
  return 0;
}

int kv_fd_rqueue_free(kv_fd_rqueue_t *const kv_fd_rqueue)
{
  if (kv_fd_rqueue->kv_fd_buf == NULL) return -1;
  kv_fd_rqueue->front_idx = 0;
  kv_fd_rqueue->back_idx = 0;
  kv_fd_rqueue->len = 0;
  kv_fd_rqueue->cap = 0;
  free(kv_fd_rqueue->kv_fd_buf);
  return 0;
}

int kv_fd_renqueue(kv_fd_rqueue_t* const kv_fd_rqueue, int kv_fd)
{
  if (
    kv_fd_rqueue->len > 0 &&
    kv_fd_rqueue->front_idx == kv_fd_rqueue->back_idx
  ) return -1;
  kv_fd_rqueue->kv_fd_buf[kv_fd_rqueue->back_idx] = kv_fd;
  kv_fd_rqueue->back_idx = (kv_fd_rqueue->back_idx + 1) % kv_fd_rqueue->cap;
  kv_fd_rqueue->len++;
  return 0;
}

int kv_fd_rdequeue(kv_fd_rqueue_t* const kv_fd_rqueue)
{
  if (kv_fd_rqueue->len <= 0) return -1;
  int fd = kv_fd_rqueue->kv_fd_buf[kv_fd_rqueue->front_idx];
  kv_fd_rqueue->front_idx = (kv_fd_rqueue->front_idx + 1) % kv_fd_rqueue->cap;
  kv_fd_rqueue->len--;
  return fd;
}

