#include "criterion/criterion.h"
#include "core/kv_fd_rqueue.h"

/* test suite 1 */
Test(kv_fd_rqueue, kv_fd_renqueue) {
  kv_fd_rqueue_t kv_fd_rqueue;
  kv_fd_rqueue_init(&kv_fd_rqueue, 5);

  cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, 95), 0);
  cr_expect_eq(kv_fd_rqueue.len, 1);
  cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, 14), 0);
  cr_expect_eq(kv_fd_rqueue.len, 2);
  cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, 307), 0);
  cr_expect_eq(kv_fd_rqueue.len, 3);
  cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, 7), 0);
  cr_expect_eq(kv_fd_rqueue.len, 4);
  cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, 13), 0);
  cr_expect_eq(kv_fd_rqueue.len, 5);
  cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, 5), -1);
  cr_expect_eq(kv_fd_rqueue.len, 5);
  cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, 8), -1);
  cr_expect_eq(kv_fd_rqueue.len, 5);

  kv_fd_rqueue_free(&kv_fd_rqueue);
}

/* test suite 2 */
Test(kv_fd_rqueue, kv_fd_rdequeue) {
  kv_fd_rqueue_t kv_fd_rqueue;
  kv_fd_rqueue_init(&kv_fd_rqueue, 5);

  kv_fd_renqueue(&kv_fd_rqueue, 95);
  kv_fd_renqueue(&kv_fd_rqueue, 14);
  kv_fd_renqueue(&kv_fd_rqueue, 307);
  kv_fd_renqueue(&kv_fd_rqueue, 7);
  kv_fd_renqueue(&kv_fd_rqueue, 13);
  kv_fd_renqueue(&kv_fd_rqueue, 5);
  kv_fd_renqueue(&kv_fd_rqueue, 8);
  cr_expect_eq(kv_fd_rqueue.len, 5);

  cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), 95);
  cr_expect_eq(kv_fd_rqueue.len, 4);
  cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), 14);
  cr_expect_eq(kv_fd_rqueue.len, 3);
  cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), 307);
  cr_expect_eq(kv_fd_rqueue.len, 2);
  cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), 7);
  cr_expect_eq(kv_fd_rqueue.len, 1);
  cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), 13);
  cr_expect_eq(kv_fd_rqueue.len, 0);
  cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), -1);
  cr_expect_eq(kv_fd_rqueue.len, 0);
  cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), -1);
  cr_expect_eq(kv_fd_rqueue.len, 0);

  kv_fd_rqueue_free(&kv_fd_rqueue);
}

/* test suite 3 */
Test(kv_fd_rqueue, kv_fd_renqueue_alt_kv_fd_rdequeue) {
  kv_fd_rqueue_t kv_fd_rqueue;
  kv_fd_rqueue_init(&kv_fd_rqueue, 25);

  for (int i = 0; i < 50; ++i) {
    int in = (i + 1) * 5;
    cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, in), 0);
    cr_expect_eq(kv_fd_rqueue.len, 1);
    cr_expect_eq(kv_fd_rdequeue(&kv_fd_rqueue), in);
    cr_expect_eq(kv_fd_rqueue.len, 0);
  }

  cr_expect_eq(kv_fd_rqueue.len, 0);
  for (int i = 1; i < 40; ++i) {
    if (i <= 25) {
      cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, (i + 2) * 3), 0);
    } else {
      cr_expect_eq(kv_fd_renqueue(&kv_fd_rqueue, (i + 1) * 4), -1);
    }
  } 
  cr_expect_eq(kv_fd_rqueue.len, 25);
  for (int i = 0; i < 7; ++i) {
    cr_expect_gt(kv_fd_rdequeue(&kv_fd_rqueue), -1);
  }
  cr_expect_eq(kv_fd_rqueue.len, 18);

  kv_fd_rqueue_free(&kv_fd_rqueue);
}

