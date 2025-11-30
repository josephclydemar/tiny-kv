#ifndef STACK_H_
#define STACK_H_

#include <stddef.h>
#include <stdlib.h>

typedef struct node_t {
  struct node_t* next;
  int node_data;
} node_t;

typedef struct {
  node_t* node_pool;
  size_t top_idx, bottom_idx;
  size_t count, cap;
} stack_t;


void stack_new(stack_t* const stack, size_t mem_size);
node_t* stack_push(stack_t* const stack, int node_data);
node_t* stack_pop(stack_t* const stack);

#endif // STACK_H_

