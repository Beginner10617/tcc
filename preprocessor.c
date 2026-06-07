#include "preprocessor.h"
#include "ctype.h"
#include "debug.h"
#include "hashmap.h"
#include "stddef.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#define ERROR "\x1b[31m"
#define WARNING "\033[33m"
#define COLOR_RESET "\x1b[0m"

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
  while (s[i] != '\0') {
    buffer_append_char(b, s[i]);
    i++;
  }
}

char *read_file(const char *filePath) {
  FILE *f = fopen(filePath, "r");
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

Buffer preprocess(const char *src, bool debug) {
  Buffer out;
  buffer_init(&out);
  size_t bucket_count = 10;
  HashMap *MacroMap = hashmap_create(bucket_count);
  // include, ifndef, endif, define in future
  size_t i = 0, line_start, line_end, line_len, row = 0;
  while (src[i] != '\0') {
    row++;
    line_start = i;
    while (src[i] != '\0' && src[i] != '\n') {
      i++;
    }
    line_end = i;
    line_len = line_end - line_start;
    char line[line_len + 1];
    for (size_t j = 0; j < line_len; j++)
      line[j] = src[line_start + j];
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
        while (*head && *head != '"') {
          filename[j] = *head;
          head++;
          j++;
        }
        if (*head != '"') {
          printf("line %zu : %s\n", row, line);
          printf(ERROR "Expected closing \"\n" COLOR_RESET);
          exit(EXIT_FAILURE);
        }
        filename[j] = '\0';

        char *tmp = read_file(filename);

        Buffer included = preprocess(tmp, false);
        buffer_append_cstr(&out, included.data);
        free(tmp);
      } else {
        printf("line %zu : %s\n", row, line);
        printf(ERROR "Expected a \"\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
    } else if (strncmp(head, "#define", 7) == 0) {
      // #define <MACRO-NAME>
      head += 7;
      while (*head && isspace((unsigned char)*head))
        head++;
      char macroName[line_len + 1];
      size_t j = 0;
      if (!isalpha((unsigned char)*head) && *head != '_') {
        printf("line %zu : %s\n", row, line);
        printf(ERROR "Invalid macro name\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      while (*head && (isalnum((unsigned char)*head) || *head == '_')) {
        macroName[j] = *head;
        head++;
        j++;
      }
      macroName[j] = '\0';
      // is it already defined?
      if (hashmap_get(MacroMap, macroName) != NULL) {
        // warn of re-definition
        printf("line %zu : %s\n", row, line);
        printf(WARNING "Re-defining macro %s\n" COLOR_RESET, macroName);
      }
      while (*head && isspace((unsigned char)*head))
        head++;
      if (*head) { // ONLY EMPTY MACROS ALLOWED
        printf("line %zu : %s\n", row, line);
        printf("Invalid macro definition\n");
        exit(EXIT_FAILURE);
      }

      hashmap_put(MacroMap, macroName, NULL);
      // store them as key-value pait in custom hash-map
    } else if (strncmp(head, "#undef", 6) == 0) {
      // #undef <MACRO-NAME>
      head += 6;
      while (*head && isspace((unsigned char)*head))
        head++;

    } else {
      // Normal line of code
      buffer_append_cstr(&out, line);
      buffer_append_char(&out, '\n');
    }
    if (src[i] == '\n')
      i++;
  }
  if (debug) {
    debug_print_hashmap(MacroMap);
    printf("Completed pre-processing, output:\n");
    printf("%s\n", out.data);
  }
  return out;
}
// #define (empty ,acros only)
// #undef
// #ifdef
// #ifndef
// #else
// #endif
// #pragma once
