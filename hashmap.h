#ifndef HASH_MAP
#define HASH_MAP
#include "stddef.h"
#include <stdbool.h>
typedef struct Entry {
  char *key;
  void *value;
  struct Entry *next;
} Entry;

typedef struct {
  Entry **buckets;
  size_t bucket_count;
} HashMap;
// constructor
HashMap *hashmap_create(size_t bucket_count);
// re-hash
void hashmap_resize(HashMap *map, size_t new_bucket_count);
// compute load factor
float load_factor(HashMap *map);
// hash function
unsigned long hash(const char *str);
// adding elements
void hashmap_put(HashMap *map, const char *key, void *value);
// accessing elements
void *hashmap_get(HashMap *map, const char *key);
bool hashmap_contains(HashMap *map, const char *key);
// deletion
void hashmap_remove(HashMap *map, const char *key);
// destructor
void hashmap_destroy(HashMap *map);
#endif
