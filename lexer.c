#include "lexer.h"
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#define ERROR "\x1b[31m"
#define WARNING "\033[33m"
#define COLOR_RESET "\x1b[0m"

Tokenizer CreateTokenizer(const char *src, const char *filename) {
  Tokenizer out;
  out.filename = filename;
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
  tokenstream->size++;
}

bool Tokenizer_current_is(Tokenizer *tokenizer, const char *name) {
  size_t len = strlen(name);
  if (len > tokenizer->index)
    return false;
  const char *origin = tokenizer->src + tokenizer->index - len;
  return strncmp(origin, name, len) == 0;
}

// helper function
void error_message(size_t row, const char *line, const char *fname,
                   const char *message) {
  printf("%s line %zu : ", fname, row);
  size_t index = 0;
  while (line[index] && line[index] != '\n') {
    printf("%c", line[index]);
    index++;
  }
  printf("\n");
  printf(ERROR "%s\n" COLOR_RESET, message);
  exit(EXIT_FAILURE);
}

TokenStream Tokenize(Tokenizer *tokenizer) {
  TokenStream out;
  TokenStream_init(&out);
  // Tokenization loop
  // tmp states
  char peek;
  const char *curr_line = tokenizer->src;
  Token tmp;
  size_t row = 0;
  while (TokenPeek(tokenizer, 0)) {
    peek = TokenPeek(tokenizer, 0);
    tmp.start = tokenizer->src + tokenizer->index;
    if (isalpha((unsigned char)peek)) {
      tmp.len = 1;
      TokenConsume(tokenizer);
      while (isalnum((unsigned char)TokenPeek(tokenizer, 0)) ||
             TokenPeek(tokenizer, 0) == '_') {
        TokenConsume(tokenizer);
        tmp.len++;
      }
      if (Tokenizer_current_is(tokenizer, "extern")) {
        tmp.type = TOK_EXTERN;
      } else if (Tokenizer_current_is(tokenizer, "signed")) {
        tmp.type = TOK_SIGNED;
      } else if (Tokenizer_current_is(tokenizer, "unsigned")) {
        tmp.type = TOK_UNSIGNED;
      } else if (Tokenizer_current_is(tokenizer, "if")) {
        tmp.type = TOK_IF;
      } else if (Tokenizer_current_is(tokenizer, "else")) {
        tmp.type = TOK_ELSE;
      } else if (Tokenizer_current_is(tokenizer, "while")) {
        tmp.type = TOK_WHILE;
      } else if (Tokenizer_current_is(tokenizer, "do")) {
        tmp.type = TOK_DO;
      } else if (Tokenizer_current_is(tokenizer, "for")) {
        tmp.type = TOK_FOR;
      } else if (Tokenizer_current_is(tokenizer, "return")) {
        tmp.type = TOK_RETURN;
      } else if (Tokenizer_current_is(tokenizer, "short")) {
        tmp.type = TOK_SHORT_KW;
      } else if (Tokenizer_current_is(tokenizer, "auto")) {
        tmp.type = TOK_AUTO_KW;
      } else if (Tokenizer_current_is(tokenizer, "register")) {
        tmp.type = TOK_REGISTER_KW;
      } else if (Tokenizer_current_is(tokenizer, "typedef")) {
        tmp.type = TOK_TYPEDEF;
      } else if (Tokenizer_current_is(tokenizer, "int")) {
        tmp.type = TOK_INT_KW;
      } else if (Tokenizer_current_is(tokenizer, "long")) {
        tmp.type = TOK_LONG_KW;
      } else if (Tokenizer_current_is(tokenizer, "char")) {
        tmp.type = TOK_CHAR_KW;
      } else if (Tokenizer_current_is(tokenizer, "float")) {
        tmp.type = TOK_FLOAT_KW;
      } else if (Tokenizer_current_is(tokenizer, "double")) {
        tmp.type = TOK_DOUBLE_KW;
      } else if (Tokenizer_current_is(tokenizer, "void")) {
        tmp.type = TOK_VOID;
      } else if (Tokenizer_current_is(tokenizer, "struct")) {
        tmp.type = TOK_STRUCT;
      } else if (Tokenizer_current_is(tokenizer, "union")) {
        tmp.type = TOK_UNION;
      } else if (Tokenizer_current_is(tokenizer, "enum")) {
        tmp.type = TOK_ENUM;
      } else {
        tmp.type = TOK_IDENTIFIER;
      }
    } else if (peek == '(') {
      tmp.len = 1;
      tmp.type = TOK_LPAREN;
      TokenConsume(tokenizer);
    } else if (peek == ')') {
      tmp.len = 1;
      tmp.type = TOK_RPAREN;
      TokenConsume(tokenizer);
    } else if (peek == '{') {
      tmp.len = 1;
      tmp.type = TOK_LBRACE;
      TokenConsume(tokenizer);
    } else if (peek == '}') {
      tmp.len = 1;
      tmp.type = TOK_RBRACE;
      TokenConsume(tokenizer);
    } else if (peek == '[') {
      tmp.len = 1;
      tmp.type = TOK_LBRACKET;
      TokenConsume(tokenizer);
    } else if (peek == ']') {
      tmp.len = 1;
      tmp.type = TOK_RBRACKET;
      TokenConsume(tokenizer);
    } else if (peek == ',') {
      tmp.len = 1;
      tmp.type = TOK_COMMA;
      TokenConsume(tokenizer);
    } else if (peek == ';') {
      tmp.len = 1;
      tmp.type = TOK_SEMICOLON;
      TokenConsume(tokenizer);
    } else if (peek == '+') {
      if (TokenPeek(tokenizer, 1) == '+') {
        tmp.len = 2;
        tmp.type = TOK_PLUSPLUS;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_PLUS;
      }
      TokenConsume(tokenizer);
    } else if (peek == '-') {
      if (TokenPeek(tokenizer, 1) == '-') {
        tmp.len = 2;
        tmp.type = TOK_MINUSMINUS;
        TokenConsume(tokenizer);
      } else if (TokenPeek(tokenizer, 1) == '>') {
        tmp.len = 2;
        tmp.type = TOK_ARROW;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_MINUS;
      }
      TokenConsume(tokenizer);
    } else if (peek == '*') {
      tmp.len = 1;
      tmp.type = TOK_STAR;
      TokenConsume(tokenizer);
    } else if (peek == '/') {
      if (TokenPeek(tokenizer, 1) == '/') {
        // single line comment
        TokenConsume(tokenizer);
        TokenConsume(tokenizer);
        while (TokenPeek(tokenizer, 0) && TokenPeek(tokenizer, 0) != '\n')
          TokenConsume(tokenizer);
        TokenConsume(tokenizer); // consume newline
        continue;                // don't add tmp
      } else if (TokenPeek(tokenizer, 1) == '*') {
        // multi line comment
        TokenConsume(tokenizer);
        TokenConsume(tokenizer);
        while (TokenPeek(tokenizer, 0) && TokenPeek(tokenizer, 0) == '*' &&
               TokenPeek(tokenizer, 1) && TokenPeek(tokenizer, 1) == '/')
          TokenConsume(tokenizer);
        // consume * and /
        TokenConsume(tokenizer);
        TokenConsume(tokenizer);
        continue; // dont' add tmp
      }
      tmp.len = 1;
      tmp.type = TOK_SLASH;
      TokenConsume(tokenizer);
    } else if (peek == '%') {
      tmp.len = 1;
      tmp.type = TOK_PERCENT;
      TokenConsume(tokenizer);
    } else if (peek == '=') {
      if (TokenPeek(tokenizer, 1) == '=') {
        tmp.len = 2;
        tmp.type = TOK_EQEQ;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_EQ;
      }
      TokenConsume(tokenizer);
    } else if (peek == '!') {
      if (TokenPeek(tokenizer, 1) == '=') {
        tmp.len = 2;
        tmp.type = TOK_NEQ;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_NOT;
      }
      TokenConsume(tokenizer);
    } else if (peek == '<') {
      if (TokenPeek(tokenizer, 1) == '<') {
        tmp.len = 2;
        tmp.type = TOK_LSHIFT;
        TokenConsume(tokenizer);
      } else if (TokenPeek(tokenizer, 1) == '=') {
        tmp.len = 2;
        tmp.type = TOK_LTE;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_LT;
      }
      TokenConsume(tokenizer);
    } else if (peek == '>') {
      if (TokenPeek(tokenizer, 1) == '>') {
        tmp.len = 2;
        tmp.type = TOK_RSHIFT;
        TokenConsume(tokenizer);
      } else if (TokenPeek(tokenizer, 1) == '=') {
        tmp.len = 2;
        tmp.type = TOK_GTE;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_GT;
      }
      TokenConsume(tokenizer);
    } else if (peek == '&') {
      if (TokenPeek(tokenizer, 1) == '&') {
        tmp.len = 2;
        tmp.type = TOK_ANDAND;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_AMP;
      }
      TokenConsume(tokenizer);
    } else if (peek == '|') {
      if (TokenPeek(tokenizer, 1) == '|') {
        tmp.len = 2;
        tmp.type = TOK_OROR;
        TokenConsume(tokenizer);
      } else {
        tmp.len = 1;
        tmp.type = TOK_PIPE;
      }
      TokenConsume(tokenizer);
    } else if (peek == '^') {
      tmp.len = 1;
      tmp.type = TOK_CARET;
      TokenConsume(tokenizer);
    } else if (peek == '~') {
      tmp.len = 1;
      tmp.type = TOK_TILDE;
      TokenConsume(tokenizer);
    } else if (peek == '.') {
      tmp.len = 1;
      tmp.type = TOK_DOT;
      TokenConsume(tokenizer);
    } else if (peek == '?') {
      tmp.len = 1;
      tmp.type = TOK_QUESTION;
      TokenConsume(tokenizer);
    } else if (peek == ':') {
      tmp.len = 1;
      tmp.type = TOK_COLON;
      TokenConsume(tokenizer);
    } else if (peek == '\'') {
      // char literal
      TokenConsume(tokenizer);
      // resolution of valid/invalid char will
      // be done during parsing, since handling
      // escape sequences is not easy
      tmp.type = TOK_CHAR;
      while (TokenPeek(tokenizer, 0)) {
        char c = TokenPeek(tokenizer, 0);
        if (c == '\n') {
          // error - newline in char literal
          error_message(row, curr_line, tokenizer->filename,
                        "Newline inside char literal");
        } else if (c == '\\') {
          TokenConsume(tokenizer);
          if (!TokenPeek(tokenizer, 0)) {
            // error - undetermined escape sequence
            error_message(row, curr_line, tokenizer->filename,
                          "Undetermined esape sequence");
          }
          TokenConsume(tokenizer);
          continue;
        } else if (c == '\'')
          break;
        else
          TokenConsume(tokenizer);
      }
      if (TokenPeek(tokenizer, 0) != '\'') {
        // error - Unterminated char literal
        error_message(row, curr_line, tokenizer->filename,
                      "Unterminated char literal");
      }
      TokenConsume(tokenizer);
      tmp.len = tokenizer->src + tokenizer->index - tmp.start;
    } else if (peek == '"') {
      // string literal
      TokenConsume(tokenizer);

      tmp.type = TOK_STRING;
      while (TokenPeek(tokenizer, 0)) {
        char c = TokenPeek(tokenizer, 0);
        if (c == '\n') {
          // error - newline in char literal
          error_message(row, curr_line, tokenizer->filename,
                        "Newline inside string literal");
        } else if (c == '\\') {
          TokenConsume(tokenizer);
          if (!TokenPeek(tokenizer, 0)) {
            // error - undetermined escape sequence
            error_message(row, curr_line, tokenizer->filename,
                          "Undetermined esape sequence");
          }
          TokenConsume(tokenizer);
          continue;
        } else if (c == '"')
          break;
        else
          TokenConsume(tokenizer);
      }
      if (TokenPeek(tokenizer, 0) != '"') {
        // error - Unterminated char literal
        error_message(row, curr_line, tokenizer->filename,
                      "Unterminated string literal");
      }
      TokenConsume(tokenizer);
      tmp.len = tokenizer->src + tokenizer->index - tmp.start;
    } else if (isdigit((unsigned char)peek)) {
      // integer or float literal
      bool is_float = false;
      tmp.len = 0;
      while (isdigit((unsigned char)TokenPeek(tokenizer, 0)) ||
             TokenPeek(tokenizer, 0) == '.') {
        if (!is_float && TokenPeek(tokenizer, 0) == '.')
          is_float = true;
        else if (TokenPeek(tokenizer, 0) == '.')
          // multiple dots
          error_message(row, curr_line, tokenizer->filename,
                        "Invalid number literal\n");
        TokenConsume(tokenizer);
        tmp.len++;
      }
      if (is_float && (TokenPeek(tokenizer, 0) == 'f') ||
          TokenPeek(tokenizer, 0) == 'F') {
        TokenConsume(tokenizer); // consume the f
        tmp.len++;
      }
      tmp.type = TOK_NUMBER;

    } else if (isspace((unsigned char)peek)) {
      if (peek == '\n') {
        curr_line = tokenizer->src + tokenizer->index + 1;
        row++;
      }
      TokenConsume(tokenizer);
      continue; // Don't store tmp
    } else {
      error_message(row, curr_line, tokenizer->filename, "Invalid token");
    }
    TokenStream_tokencopy(&out, tmp);
  }
  tmp.start = tokenizer->src + tokenizer->index;
  tmp.len = 1;
  tmp.type = TOK_EOF;
  TokenStream_tokencopy(&out, tmp);
  return out;
}
