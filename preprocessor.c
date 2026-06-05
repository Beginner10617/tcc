#include "preprocessor.h"
#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

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

Buffer preprocess(const char *src) {
  Buffer out;
  buffer_init(&out);
  char tmp[10]; // include, ifndef, endif, define in future
  size_t i = 0, line_start, line_end, line_len;
  while (src[i] != '\0') {
    line_start = i;
    while (src[i] != '\0' || src[i] != '\n')
      i++;
    line_end = i;
    line_len = line_end - line_start;

    char line[line_len + 1];
    line[line_len] = '\0';

    // ignore spaces
    char *head = line;
    while (*head && isspace((unsigned char)*head))
      head++;

    // handle preprocess
    // #include
    if (strncmp(head, "#include", 8) == 0) {
      head += 8;

      while (*head && isspace((unsigned char)*head))
        head++;

      if (*head == '"') {
        head++;
        char filename[line_len + 1];
        size_t j = 0;
        while (*head && isspace((unsigned char)*head)) {
          filename[j] = *head;
          head++;
          j++;
        }

        filename[j] = '\0';

        char *tmp = read_file(filename);

        Buffer included = preprocess(tmp);
        buffer_append_cstr(&out, included.data);
        buffer_free(&included);
        free(tmp);
      }
    } else {
      buffer_append_cstr(&out, line);
      buffer_append_char(&out, '\n');
    }
    if (src[i] == '\n')
      i++;
  }
  return out;
}
// #define (object and function macros)
// #undef
// #ifdef
// #ifndef
// #if
// #elif
// #else
// #endif
// #error
// #pragma once
// Take care of macro expansion recursion
