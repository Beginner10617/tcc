#ifndef PRE_PROCESSOR_H
#define PRE_PROCESSOR_H
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
Buffer preprocess(const char *src);

/* Template
Buffer src;
buffer_init(&src);

preprocess_into(&src, "main.c");

Tokenizer t = create_tokenizer(src.data);
TokenStream ts = tokenize(&t);
parse(&ts);

tokenstream_free(&ts);
buffer_free(&src);
*/
#endif
