#include "common/stack.h"

void stack_new(stack_t* const stack, size_t mem_size)
{
  stack->node_pool = malloc(sizeof(node_t) * mem_size);
  stack->count = 0;
  stack->cap = mem_size;
  stack->top_idx = 0;
  stack->bottom_idx = 0;
}

node_t* stack_push(stack_t* const stack, int node_data)
{
  if (stack->top_idx >= stack->cap) return NULL;
  node_t* node = stack->node_pool + stack->top_idx;
  node->node_data = node_data;
  stack->top_idx++;
  stack->count++;
  return node;
}

node_t* stack_pop(stack_t* const stack)
{
  if (stack->top_idx <= 0) return NULL;
  stack->top_idx--;
  stack->count--;
  return stack->node_pool + stack->top_idx - 1;
}
