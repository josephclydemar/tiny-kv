#include "util/logging.h"
#include "util/debugging.h"
#include "core/kv_server.h"

int main(void)
{
  INFO_LOG("MAIN", "Hello world, I'm Server!!");
  pthread_attr_t attr;
  ASSERT("PTHREAD_ATTR_INIT", pthread_attr_init(&attr) == 0);

  for (size_t i = 0; i < MAX_CONN_HANDLERS; ++i) {
    ASSERT("PTHREAD_CREATE", pthread_create(&kv_serv.kv_conn_handlers[i], &attr, kv_conn_handler_fn, NULL) == 0);
  }
  kv_ctrl_fn(NULL);
  return 0;
}
