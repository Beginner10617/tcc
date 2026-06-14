#include "preprocessor.h"
#include "debug.h"
#include "hashmap.h"
#include "stack.h"
#include "stddef.h"
#include "stdio.h"
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
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

ConditionalBlock *create_conditional() {
  ConditionalBlock *out = malloc(sizeof(ConditionalBlock));
  if (out == NULL) {
    printf(ERROR "Unable to create conditional block\n" COLOR_RESET);
    exit(EXIT_FAILURE);
  }
  return out;
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

Buffer preprocess(const char *src, const char *fname, bool debug,
                  HashMap *incomingMacroMap) {
  Buffer out;
  buffer_init(&out);
  size_t bucket_count = 10;
  HashMap *MacroMap;
  stack *ifStack = stack_create();
  if (incomingMacroMap == NULL)
    MacroMap = hashmap_create(bucket_count);
  else
    MacroMap = incomingMacroMap;
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
    if (strncmp(head, "#include", 8) == 0) {
      // #include
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
          printf("%s line %zu : %s\n", fname, row, line);
          printf(ERROR "Expected closing \"\n" COLOR_RESET);
          exit(EXIT_FAILURE);
        }
        filename[j] = '\0';

        char *tmp = read_file(filename);

        Buffer included = preprocess(tmp, filename, false, MacroMap);
        buffer_append_cstr(&out, included.data);
        free(tmp);
      } else {
        printf("%s line %zu : %s\n", fname, row, line);
        printf(ERROR "Expected a \"\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
    } else if (strncmp(head, "#define", 7) == 0) {
      // #define <MACRO-NAME>
      head += 7;
      while (*head && isspace((unsigned char)*head))
        head++;
      if (!isalpha((unsigned char)*head) && *head != '_') {
        printf("%s line %zu : %s\n", fname, row, line);
        printf(ERROR "Invalid macro name\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      size_t j = 0;
      while (*(head + j) &&
             (isalnum((unsigned char)*(head + j)) || *(head + j) == '_'))
        j++;
      char macroName[j + 1];
      j = 0;
      while (*head && (isalnum((unsigned char)*head) || *head == '_')) {
        macroName[j] = *head;
        head++;
        j++;
      }
      macroName[j] = '\0';
      // is it already defined?
      if (hashmap_get(MacroMap, macroName) != NULL) {
        // warn of re-definition
        printf("%s line %zu : %s\n", fname, row, line);
        printf(WARNING "Re-defining macro %s\n" COLOR_RESET, macroName);
      }
      while (*head && isspace((unsigned char)*head))
        head++;
      if (*head) { // ONLY EMPTY MACROS ALLOWED
        printf("%s line %zu : %s\n", fname, row, line);
        printf("Invalid macro definition\n");
        exit(EXIT_FAILURE);
      }

      hashmap_put(MacroMap, macroName, NULL);
      if (load_factor(MacroMap) > 0.7f)
        hashmap_resize(MacroMap, MacroMap->bucket_count * 2);
      // store them as key-value pair in custom hash-map
    } else if (strncmp(head, "#undef", 6) == 0) {
      // #undef <MACRO-NAME>
      head += 6;
      while (*head && isspace((unsigned char)*head))
        head++;
      if (!isalpha((unsigned char)*head) && *head != '_') {
        printf("%s line %zu : %s\n", fname, row, line);
        printf(ERROR "Invalid macro name\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      size_t len = 0;
      while (*(head + len) &&
             (isalnum((unsigned char)*(head + len)) || *(head + len) == '_'))
        len++;
      char macroName[len + 1];
      len = 0;
      while (*head && (isalnum((unsigned char)*head) || *head == '_')) {
        macroName[len] = *head;
        head++;
        len++;
      }
      macroName[len] = '\0';
      if (hashmap_get(MacroMap, macroName) == NULL) {
        printf("%s line %zu : %s\n", fname, row, line);
        printf(ERROR "Macro not defined\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      while (*head && isspace((unsigned char)*head))
        head++;
      if (*head) {
        // TRAILING SPACES
        printf("%s line %zu : %s\n", fname, row, line);
        printf("Invalid macro removal\n");
        exit(EXIT_FAILURE);
      }
      hashmap_remove(MacroMap, macroName);

    } else if (strncmp(head, "#ifdef", 6) == 0) {
      // #ifdef <MACRO-NAME>
      head += 6;
      while (*head && isspace((unsigned char)*head))
        head++;
      if (!isalpha((unsigned char)*head) && *head != '_') {
        printf("%s line %zu : %s\n", fname, row, line);
        printf(ERROR "Invalid macro name\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      size_t len = 0;
      while (*(head + len) &&
             (isalnum((unsigned char)*(head + len)) || *(head + len) == '_'))
        len++;
      char macroName[len + 1];
      len = 0;
      while (*head && (isalnum((unsigned char)*head) || *head == '_')) {
        macroName[len] = *head;
        head++;
        len++;
      }
      macroName[len] = '\0';
      while (*head && isspace((unsigned char)*head))
        head++;
      if (*head) {
        // TRAILING SPACES
        printf("%s line %zu : %s\n", fname, row, line);
        printf("Invalid macro if\n");
        exit(EXIT_FAILURE);
      }
      ConditionalBlock *cb = create_conditional();
      cb->parent_active = true;
      if (ifStack->size)
        cb->parent_active =
            ((ConditionalBlock *)stack_top(ifStack))->current_active;
      cb->conditional_true = hashmap_get(MacroMap, macroName) != NULL;
      cb->seen_else = false;
      cb->current_active = cb->parent_active && cb->conditional_true;
      stack_push(ifStack, cb);
    } else if (strncmp(head, "#ifndef", 7) == 0) {
      // ifndef <MACRO-NAME>
      head += 7;
      while (*head && isspace((unsigned char)*head))
        head++;
      if (!isalpha((unsigned char)*head) && *head != '_') {
        printf("%s line %zu : %s\n", fname, row, line);
        printf(ERROR "Invalid macro name\n" COLOR_RESET);
        exit(EXIT_FAILURE);
      }
      size_t len = 0;
      while (*(head + len) &&
             (isalnum((unsigned char)*(head + len)) || *(head + len) == '_'))
        len++;
      char macroName[len + 1];
      len = 0;
      while (*head && (isalnum((unsigned char)*head) || *head == '_')) {
        macroName[len] = *head;
        head++;
        len++;
      }
      macroName[len] = '\0';
      while (*head && isspace((unsigned char)*head))
        head++;
      if (*head) {
        // TRAILING SPACES
        printf("%s line %zu : %s\n", fname, row, line);
        printf("Invalid macro if\n");
        exit(EXIT_FAILURE);
      }
      ConditionalBlock *cb = malloc(sizeof(ConditionalBlock));
      cb->parent_active = true;
      if (ifStack->size)
        cb->parent_active =
            ((ConditionalBlock *)stack_top(ifStack))->current_active;
      cb->conditional_true = hashmap_get(MacroMap, macroName) == NULL;
      cb->seen_else = false;
      cb->current_active = cb->parent_active && cb->conditional_true;
      stack_push(ifStack, cb);
    } else if (strncmp(head, "#else", 5)) {

    } else if (strncmp(head, "#endif", 6)) {

    } else {
      // Normal line of code
      ConditionalBlock *cb = stack_top(ifStack);
      if (!cb || cb->current_active) {
        buffer_append_cstr(&out, line);
        buffer_append_char(&out, '\n');
      }
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
// #define (empty macros only)  DONE
// #undef                       DONE
// #ifdef
// #ifndef
// #else
// #endif
