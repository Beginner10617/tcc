#include "debug.h"
#include "lexer.h"
#include <stdio.h>

void debug_print_hashmap(HashMap *map) {
  if (map == NULL) {
    printf("HashMap: NULL\n");
    return;
  }

  printf("HashMap {\n");
  printf("  bucket_count = %zu\n", map->bucket_count);

  for (size_t i = 0; i < map->bucket_count; i++) {
    printf("  [%zu] ", i);

    Entry *curr = map->buckets[i];

    if (curr == NULL) {
      printf("(empty)\n");
      continue;
    }

    while (curr != NULL) {
      printf("{ key=\"%s\", value=%p }", curr->key ? curr->key : "(null)",
             curr->value);

      if (curr->next != NULL)
        printf(" -> ");

      curr = curr->next;
    }

    printf("\n");
  }

  printf("}\n");
}

void debug_print_Token(Token token) {
  printf("TOKEN(");
  switch (token.type) {
  case TOK_EOF:
    printf("EOF");
    break;
  case TOK_EXTERN:
    printf("EXTERN");
    break;
  case TOK_IDENTIFIER:
    printf("IDENTIFIER");
    break;
  case TOK_INT:
    printf("INT LITERAL");
    break;
  case TOK_FLOAT:
    printf("FLOAT LITERAL");
    break;
  case TOK_CHAR:
    printf("CHAR LITERAL");
    break;
  case TOK_STRING:
    printf("STRING LITERAL");
    break;
  case TOK_IF:
    printf("IF");
    break;
  case TOK_ELSE:
    printf("ELSE");
    break;
  case TOK_WHILE:
    printf("WHILE");
    break;
  case TOK_DO:
    printf("DO");
    break;
  case TOK_FOR:
    printf("FOR");
    break;
  case TOK_RETURN:
    printf("RETURN");
    break;
  case TOK_AUTO_KW:
    printf("AUTO KEYWORD");
    break;
  case TOK_INT_KW:
    printf("INT KEYWORD");
    break;
  case TOK_CHAR_KW:
    printf("CHAR KEYWORD");
    break;
  case TOK_FLOAT_KW:
    printf("FLOAT KEYWORD");
    break;
  case TOK_VOID:
    printf("VOID KEYWORD");
    break;
  case TOK_STRUCT:
    printf("STRUCT");
    break;
  case TOK_ENUM:
    printf("ENUM");
    break;
  case TOK_LPAREN:
    printf("LEFT PAREN");
    break;
  case TOK_RPAREN:
    printf("RIGHT PAREN");
    break;
  case TOK_LBRACE:
    printf("LEFT BRACE");
    break;
  case TOK_RBRACE:
    printf("RIGHT BRACE");
    break;
  case TOK_LBRACKET:
    printf("LEFT BRACKET");
    break;
  case TOK_RBRACKET:
    printf("RIGHT BRACKET");
    break;
  case TOK_COMMA:
    printf("COMMA");
    break;
  case TOK_SEMICOLON:
    printf("SEMICOLON");
    break;
  case TOK_PLUS:
    printf("PLUS");
    break;
  case TOK_MINUS:
    printf("MINUS");
    break;
  case TOK_STAR:
    printf("STAR");
    break;
  case TOK_SLASH:
    printf("SLASH");
    break;
  case TOK_PERCENT:
    printf("PERCENT");
    break;
  case TOK_EQ:
    printf("EQUAL");
    break;
  case TOK_EQEQ:
    printf("DOUBLE EQUAL");
    break;
  case TOK_NEQ:
    printf("NOT EQUAL");
    break;
  case TOK_LT:
    printf("LESS THAN");
    break;
  case TOK_GT:
    printf("GREATER THAN");
    break;
  case TOK_LTE:
    printf("LESS THAN OR EQUAL");
    break;
  case TOK_GTE:
    printf("GREATER THAN OR EQUAL");
    break;
  case TOK_ANDAND:
    printf("AND");
    break;
  case TOK_OROR:
    printf("OR");
    break;
  case TOK_NOT:
    printf("NOT");
    break;
  case TOK_LSHIFT:
    printf("LEFT SHIFT");
    break;
  case TOK_RSHIFT:
    printf("RIGHT SHIFT");
    break;
  case TOK_AMP:
    printf("AMPERSAND");
    break;
  case TOK_PIPE:
    printf("PIPE");
    break;
  case TOK_CARET:
    printf("CARET");
    break;
  case TOK_TILDE:
    printf("TILDE");
    break;
  case TOK_PLUSPLUS:
    printf("INCREMENT");
    break;
  case TOK_MINUSMINUS:
    printf("DECREMNT");
    break;
  case TOK_ARROW:
    printf("ARROW");
    break;
  case TOK_DOT:
    printf("DOT");
    break;
  case TOK_QUESTION:
    printf("QUESTION MARK");
    break;
  case TOK_COLON:
    printf("COLON");
    break;
  }
  printf(", \"");
  for (size_t i = 0; i < token.len; i++)
    printf("%c", *(token.start + i));
  printf("\")");
}
