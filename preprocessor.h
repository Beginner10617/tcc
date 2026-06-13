#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H
#include "hashmap.h"
#include "stdbool.h"
#include "stddef.h"
typedef struct {
  char *data;
  size_t size;
  size_t cap;
} Buffer;
void buffer_init(Buffer *b);
void buffer_free(Buffer *b);

void buffer_append_char(Buffer *b, char c);
void buffer_append_cstr(Buffer *b, const char *s);

char *read_file(const char *filePath);
Buffer preprocess(const char *src, const char *fname, bool debug,
                  HashMap *macroMap);
#endif
