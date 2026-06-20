#ifndef LEXER_H
#define LEXER_H
#include "stddef.h"
#include "stdio.h"
typedef enum {
  TOK_EOF,

  // identifiers + literals
  TOK_IDENTIFIER,
  TOK_INT,
  TOK_FLOAT,
  TOK_CHAR,
  TOK_STRING,

  // keywords
  TOK_IF,
  TOK_ELSE,
  TOK_WHILE,
  TOK_FOR,
  TOK_RETURN,
  TOK_INT_KW,
  TOK_CHAR_KW,
  TOK_FLOAT_KW,
  TOK_VOID,
  TOK_STRUCT,
  TOK_SIZEOF,

  // punctuation
  TOK_LPAREN,
  TOK_RPAREN,
  TOK_LBRACE,
  TOK_RBRACE,
  TOK_LBRACKET,
  TOK_RBRACKET,
  TOK_COMMA,
  TOK_SEMICOLON,

  // operators
  TOK_PLUS,
  TOK_MINUS,
  TOK_STAR,
  TOK_SLASH,
  TOK_PERCENT,

  TOK_EQ,
  TOK_EQEQ,
  TOK_NEQ,
  TOK_LT,
  TOK_GT,
  TOK_LTE,
  TOK_GTE,

  TOK_ANDAND,
  TOK_OROR,
  TOK_NOT,

  TOK_AMP,
  TOK_PIPE,
  TOK_CARET,
  TOK_TILDE,

  TOK_PLUSPLUS,
  TOK_MINUSMINUS,

  TOK_ARROW,
  TOK_DOT,

  TOK_QUESTION,
  TOK_COLON
} TokenType;

typedef struct {
  TokenType type;
  const char *start;
  size_t len;
} Token;

typedef struct {
  Token *tokens;
  size_t size, cap;
} TokenStream;
// helper functions
void TokenStream_init(TokenStream *tokenstream);
void TokenStream_tokencopy(TokenStream *tokenstream, Token token);

typedef struct {
  const char *src;
  size_t index, size;
} Tokenizer;

Tokenizer CreateTokenizer(const char *src);
char TokenPeek(Tokenizer *tokenizer, size_t off); // return \0 is end
char TokenConsume(Tokenizer *tokenizer);
TokenStream Tokenize(Tokenizer *tokenizer);

#endif
