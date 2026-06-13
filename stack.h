#ifndef STACK
#define STACK
#include "stddef.h"
typedef struct {
  size_t size, cap;
  void **data;
} stack;
// NOTE : Stack owns the data
stack *stack_create();
void stack_push(stack *stk, void *data);
void *stack_top(stack *stk);
void *stack_pop(stack *stk);
void stack_clear(stack *stk);
#endif
