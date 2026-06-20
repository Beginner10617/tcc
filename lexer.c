#include "lexer.h"
#include <stdlib.h>
#include <string.h>
Tokenizer CreateTokenizer(const char *src) {
  Tokenizer out;
  out.src = src;
  out.index = 0;
  out.size = strlen(src);
  return out;
}

char TokenPeek(Tokenizer *tokenizer, size_t off) {
  if (tokenizer->index + off < tokenizer->size)
    return tokenizer->src[tokenizer->index + off];
  return '\0';
}

char TokenConsume(Tokenizer *tokenizer) {
  if (tokenizer->index >= tokenizer->size)
    return '\0';
  tokenizer->index++;
  return tokenizer->src[tokenizer->index - 1];
}

// helper functions
void TokenStream_init(TokenStream *tokenstream) {
  tokenstream->size = 0;
  tokenstream->cap = 1;
  tokenstream->tokens = malloc(sizeof(Token));
}
void TokenStream_tokencopy(TokenStream *tokenstream, Token token) {
  if (tokenstream->size >= tokenstream->cap) {
    while (tokenstream->size >= tokenstream->cap)
      tokenstream->cap *= 2;
    tokenstream->tokens =
        realloc(tokenstream->tokens, tokenstream->cap * sizeof(Token));
  }
  tokenstream->tokens[tokenstream->size] = token;
}

TokenStream Tokenize(Tokenizer *tokenizer) {
  TokenStream out;
  TokenStream_init(&out);
  // Tokenization loop
  return out;
}
