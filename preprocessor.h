#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H
#include "stdbool.h"
#include "stddef.h"
typedef struct {
  char *data;
  size_t size;
  size_t cap;
} Buffer;

typedef enum { OBJECT, FUNCTION } macroType;
typedef struct {
  macroType type;
  char *parameters; // ['a', 'b'] -> "a\0b\0"
  size_t param_count;
  char *replacement; // a > b -> "a\0>\0b\0"
} Macro;
// Use custom hash-map for macro-name : macro mapping
void buffer_init(Buffer *b);
void buffer_free(Buffer *b);

void buffer_append_char(Buffer *b, char c);
void buffer_append_cstr(Buffer *b, const char *s);

char *read_file(const char *filePath);
Buffer preprocess(const char *src);
#endif
