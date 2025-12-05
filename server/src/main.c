#include "util/debugging.h"
#include "core/kv_server.h"
#include <pthread.h>

int main(void)
{
  pthread_attr_t attr;
  ASSERT("PTHREAD_ATTR_INIT", pthread_attr_init(&attr) == 0);

  ASSERT("PTHREAD_CREATE",
    pthread_create(
      &kv_serv.kv_ctrl_thread,
      &attr,
      kv_ctrl_fn,
      "listening"
    ) == 0
  );
  for (size_t i = 0; i < MAX_CONN_HANDLERS; ++i) {
    ASSERT("PTHREAD_CREATE",
      pthread_create(
        &kv_serv.kv_conn_handler_threads[i],
        &attr,
        kv_conn_handler_fn,
        "waiting for task"
      ) == 0
    );
  }

  for (size_t i = 0; i < MAX_CONN_HANDLERS; ++i) {
    ASSERT("PTHREAD_JOIN",
      pthread_join(
        kv_serv.kv_conn_handler_threads[i],
        NULL
      ) == 0
    );
  }
  ASSERT("PTHREAD_JOIN", pthread_join(kv_serv.kv_ctrl_thread, NULL) == 0);
  return 0;
}
