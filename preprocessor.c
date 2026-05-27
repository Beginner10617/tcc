#include "preprocessor.h"
#include "stdio.h"
#include "stdlib.h"

void buffer_init(Buffer *b) {
  b->size = 0;
  b->cap = 1;
  b->data = malloc(sizeof(char) * b->cap);
  if (b->data == NULL) {
    printf("Unable to initialise buffer\n");
    exit(EXIT_FAILURE);
  }
}

void buffer_free(Buffer *b) {
  free(b->data);
  free(b);
}

void buffer_append_char(Buffer *b, char c) {
  b->size++;
  if (b->size > b->cap) {
    while (b->size > b->cap)
      b->cap *= 2;
    b->data = realloc(b->data, b->cap);
    if (b->data == NULL) {
      printf("Unable to append char to buffer\n");
      exit(EXIT_FAILURE);
    }
  }
  b->data[b->size - 1] = c;
}

void buffer_append_cstr(Buffer *b, const char *s) {
  size_t i = 0;
  while (s[i] != '\0')
    buffer_append_char(b, s[i]);
}

char *read_file(const char *filePath) {
  FILE *f = fopen(filePath, "rb");
  if (!f)
    return NULL;

  fseek(f, 0, SEEK_END);
  long size = ftell(f);
  rewind(f);

  char *buf = malloc(size + 1);
  if (!buf) {
    fclose(f);
    return NULL;
  }

  size_t read = fread(buf, 1, size, f);
  fclose(f);

  buf[read] = '\0';
  return buf;
}

/*
... = multiple spaces allowed (would be ignored)
preprocess(src) :
 create and initialise Buffer out;
 read all preprocess lines - '...#include..."xyz.h"';
 if any such lines encountered :
  tmp = read_file("xyz.h");
  out = out + preprocess(tmp);
  i.e. concat ( a = a + b -> buffer_append_cstr(a, b.data) );
  else : out = out + cstr_form_of_the_line;
 */
