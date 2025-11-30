#include "logging.h"
#include "debugging.h"

int main(void)
{
  DEBUG_LOG("MAIN", "Hello world, I'm Server!!");
  ASSERT("MAIN", 9 == 4);
  return 0;
}
