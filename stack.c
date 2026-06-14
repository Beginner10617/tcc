#include "stack.h"
#include "stdio.h"
#include "stdlib.h"
#define ERROR "\x1b[31m"
#define WARNING "\033[33m"
#define COLOR_RESET "\x1b[0m"

stack *stack_create() {
  stack *out = malloc(sizeof(stack));
  if (out == NULL) {
    printf(ERROR "Unable to create stack\n" COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  out->size = 0;
  out->cap = 1;
  out->data = malloc(sizeof(void *) * out->cap);
  if (out->data == NULL) {
    printf(ERROR "Unable to create stack\n" COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  return out;
}

void stack_push(stack *stk, void *data) {
  if (stk->size < stk->cap) {
    stk->data[stk->size] = data;
    stk->size++;
  } else {
    while (stk->size >= stk->cap)
      stk->cap++;
    stk->data = realloc(stk->data, stk->cap * sizeof(void *));
    if (stk->data == NULL) {
      printf(ERROR "Unable to realloc space for stack\n" COLOR_RESET);
      exit(EXIT_FAILURE);
    }
    stk->data[stk->size] = data;
    stk->size++;
  }
}

void *stack_top(stack *stk) {
  if (stk->size)
    return stk->data[stk->size - 1];
  return NULL;
}

void stack_pop(stack *stk) {
  if (stk->size) {
    stk->size--;
    if (stk->data[stk->size])
      free(stk->data[stk->size]);
  } else
    printf(WARNING "Trying to pop an empty stack\n" COLOR_RESET);
}

void stack_clear(stack *stk) { stk->size = 0; }

void stack_destroy(stack *stk) {
  for (size_t i = 0; i < stk->size; i++) {
    if (stk->data[i])
      free(stk->data[i]);
  }
  free(stk->data);
  free(stk);
}
