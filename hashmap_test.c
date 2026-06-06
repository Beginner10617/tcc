#include "hashmap.h"
#include "stdio.h"
int main() {

  printf("Creating hashmap...\n");

  HashMap *map = hashmap_create(4);

  int a = 10;
  int b = 20;
  int c = 30;
  int d = 40;
  int e = 50;

  printf("\n=== INSERTION TESTS ===\n");

  hashmap_put(map, "apple", &a);
  hashmap_put(map, "banana", &b);
  hashmap_put(map, "cherry", &c);
  hashmap_put(map, "date", &d);

  printf("Inserted 4 elements.\n");

  printf("\n=== RETRIEVAL TESTS ===\n");

  int *value;

  value = hashmap_get(map, "apple");
  printf("apple  -> %d\n", value ? *value : -1);

  value = hashmap_get(map, "banana");
  printf("banana -> %d\n", value ? *value : -1);

  value = hashmap_get(map, "cherry");
  printf("cherry -> %d\n", value ? *value : -1);

  value = hashmap_get(map, "date");
  printf("date   -> %d\n", value ? *value : -1);

  value = hashmap_get(map, "missing");
  printf("missing -> %s\n", value ? "FOUND (unexpected)" : "NULL");

  printf("\n=== LOAD FACTOR TEST ===\n");

  printf("Load factor = %.2f\n", load_factor(map));

  printf("\n=== UPDATE TEST ===\n");

  hashmap_put(map, "banana", &e);

  value = hashmap_get(map, "banana");
  printf("banana updated -> %d\n", value ? *value : -1);

  printf("\n=== REMOVE TESTS ===\n");

  hashmap_remove(map, "apple");

  value = hashmap_get(map, "apple");
  printf("apple after delete -> %s\n", value ? "FOUND" : "NULL");

  hashmap_remove(map, "nonexistent");

  printf("Deleting nonexistent key completed.\n");

  printf("\n=== RESIZE TEST ===\n");

  printf("Load factor before resize = %.2f\n", load_factor(map));

  hashmap_resize(map, 16);

  printf("Resized to 16 buckets.\n");

  printf("\n=== VERIFY DATA AFTER RESIZE ===\n");

  value = hashmap_get(map, "banana");
  printf("banana -> %d\n", value ? *value : -1);

  value = hashmap_get(map, "cherry");
  printf("cherry -> %d\n", value ? *value : -1);

  value = hashmap_get(map, "date");
  printf("date -> %d\n", value ? *value : -1);

  printf("\n=== HASH FUNCTION TEST ===\n");

  printf("hash(\"apple\")  = %lu\n", hash("apple"));
  printf("hash(\"banana\") = %lu\n", hash("banana"));

  printf("\nAll tests completed.\n");

  return 0;
}
