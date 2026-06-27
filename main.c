#include "debug.h"
#include "lexer.h"
#include "preprocessor.h"
#include "stdio.h"
#include "stdlib.h"
int main(int argc, char *argv[]) {
  char *filepath = argv[1];
  char *src = read_file(filepath);
  Buffer final_src = preprocess(src, argv[1], false, NULL);

  Tokenizer tokenizer = CreateTokenizer(final_src.data, argv[1]);
  TokenStream tokens = Tokenize(&tokenizer);
  printf("Total %zu tokens:\n", tokens.size);
  for (size_t i = 0; i < tokens.size; i++) {
    debug_print_Token(tokens.tokens[i]);
    printf("\n");
  }
  return EXIT_SUCCESS;
}
