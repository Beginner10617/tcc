#ifndef LEXER_H
#define LEXER_H
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
typedef enum {
  TOK_EOF,
  TOK_EXTERN,

  // identifiers + literals
  TOK_IDENTIFIER,
  TOK_NUMBER,
  TOK_CHAR,
  TOK_STRING,

  // qualifiers
  TOK_SIGNED,
  TOK_UNSIGNED,

  // keywords
  TOK_IF,
  TOK_ELSE,
  TOK_WHILE,
  TOK_DO,
  TOK_FOR,
  TOK_RETURN,
  TOK_AUTO_KW,
  TOK_REGISTER_KW,
  TOK_TYPEDEF,
  TOK_SHORT_KW,
  TOK_INT_KW,
  TOK_LONG_KW,
  TOK_CHAR_KW,
  TOK_FLOAT_KW,
  TOK_DOUBLE_KW,
  TOK_VOID,
  TOK_STRUCT,
  TOK_UNION,
  TOK_ENUM,

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
  TOK_LSHIFT,
  TOK_RSHIFT,

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
  const char *filename;
  const char *src;
  size_t index, size;
} Tokenizer;

Tokenizer CreateTokenizer(const char *src, const char *filename);
char TokenPeek(Tokenizer *tokenizer, size_t off); // return \0 is end
char TokenConsume(Tokenizer *tokenizer);
TokenStream Tokenize(Tokenizer *tokenizer);
// helper function
bool Tokenizer_current_is(Tokenizer *tokenizer, const char *name);
#endif
