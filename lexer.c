#include "lexer.h"

Tokenizer CreateTokenizer(const char *src) {
  Tokenizer out;
  out.src = src;
  out.index = 0;
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
