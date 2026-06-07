#include "debug.h"
#include <stdio.h>

void debug_print_hashmap(HashMap *map) {
  if (map == NULL) {
    printf("HashMap: NULL\n");
    return;
  }

  printf("HashMap {\n");
  printf("  bucket_count = %zu\n", map->bucket_count);

  for (size_t i = 0; i < map->bucket_count; i++) {
    printf("  [%zu] ", i);

    Entry *curr = map->buckets[i];

    if (curr == NULL) {
      printf("(empty)\n");
      continue;
    }

    while (curr != NULL) {
      printf("{ key=\"%s\", value=%p }", curr->key ? curr->key : "(null)",
             curr->value);

      if (curr->next != NULL)
        printf(" -> ");

      curr = curr->next;
    }

    printf("\n");
  }

  printf("}\n");
}
