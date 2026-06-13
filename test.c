#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "stack.h"

static char *make_string(const char *s) {
  char *copy = malloc(strlen(s) + 1);
  assert(copy);
  strcpy(copy, s);
  return copy;
}

static void test_create_empty(void) {
  stack *stk = stack_create();

  assert(stk != NULL);
  assert(stk->size == 0);

  stack_clear(stk);

  printf("[PASS] create_empty\n");
}

static void test_push_top(void) {
  stack *stk = stack_create();

  stack_push(stk, make_string("A"));
  assert(strcmp((char *)stack_top(stk), "A") == 0);

  stack_push(stk, make_string("B"));
  assert(strcmp((char *)stack_top(stk), "B") == 0);

  stack_push(stk, make_string("C"));
  assert(strcmp((char *)stack_top(stk), "C") == 0);

  stack_clear(stk);

  printf("[PASS] push_top\n");
}

static void test_lifo_order(void) {
  stack *stk = stack_create();
  stack_push(stk, make_string("first"));
  stack_push(stk, make_string("second"));
  stack_push(stk, make_string("third"));

  char *s;

  s = (char *)stack_pop(stk);
  assert(strcmp(s, "third") == 0);

  s = (char *)stack_pop(stk);
  assert(strcmp(s, "second") == 0);

  s = (char *)stack_pop(stk);
  assert(strcmp(s, "first") == 0);

  assert(stk->size == 0);

  stack_clear(stk);

  printf("[PASS] lifo_order\n");
}

static void test_many_pushes(void) {
  stack *stk = stack_create();

  const int N = 10000;

  for (int i = 0; i < N; i++) {
    int *x = malloc(sizeof(int));
    assert(x);

    *x = i;
    stack_push(stk, x);
  }

  assert(stk->size == (size_t)N);

  for (int i = N - 1; i >= 0; i--) {
    int *x = stack_pop(stk);

    assert(*x == i);
    free(x);
  }

  assert(stk->size == 0);

  stack_clear(stk);

  printf("[PASS] many_pushes\n");
}

static void test_interleaved_operations(void) {
  stack *stk = stack_create();

  stack_push(stk, make_string("A"));
  stack_push(stk, make_string("B"));

  char *s = stack_pop(stk);
  assert(strcmp(s, "B") == 0);
  free(s);

  stack_push(stk, make_string("C"));

  assert(strcmp((char *)stack_top(stk), "C") == 0);

  s = stack_pop(stk);
  assert(strcmp(s, "C") == 0);
  free(s);

  s = stack_pop(stk);
  assert(strcmp(s, "A") == 0);
  free(s);

  assert(stk->size == 0);

  stack_clear(stk);

  printf("[PASS] interleaved_operations\n");
}

static void test_clear_nonempty(void) {
  stack *stk = stack_create();

  for (int i = 0; i < 100; i++) {
    int *x = malloc(sizeof(int));
    *x = i;
    stack_push(stk, x);
  }

  stack_clear(stk);

  printf("[PASS] clear_nonempty\n");
}

int main(void) {
  test_create_empty();
  test_push_top();
  test_lifo_order();
  test_many_pushes();
  test_interleaved_operations();
  test_clear_nonempty();

  printf("\nAll tests passed.\n");
  return 0;
}
