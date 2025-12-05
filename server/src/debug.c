#include "util/logging.h"
#include "core/kv_fd_rqueue.h"

int main(void)
{
  kv_fd_rqueue_t kv_fd_rqueue;
  kv_fd_rqueue_init(&kv_fd_rqueue, 7);
  kv_fd_renqueue(&kv_fd_rqueue, 3);
  kv_fd_renqueue(&kv_fd_rqueue, 19);
  kv_fd_rqueue_free(&kv_fd_rqueue);
}
