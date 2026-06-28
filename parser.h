#ifndef PARSER
#define PARSER
#include "lexer.h"
typedef enum {
  TY_INT,
  TY_CHAR,
  TY_FLOAT,
  TY_VOID,

  TY_PTR,
  TY_ARR,
  TY_FUNC,
  TY_STRUCT,
  TY_UNION,
  TY_ENUM,
} TypeKind;
typedef enum { QF_SIGNED, QF_UNSIGNED, QF_CONST } TypeQualifier;
typedef struct parameter parameter;
typedef struct Type Type;
typedef struct {
  size_t len;
} Arr;
typedef struct {
  Type *ret;
  parameter *params;
  size_t param_count;
  bool variadic;
} Func;
typedef struct {
  parameter *members;
  size_t member_count;
} Record; // used for union and structs
typedef struct {
  parameter *names;
  size_t count;
} Enum;

struct Type {
  TypeKind kind;
  TypeQualifier qualifier;
  size_t size;
  size_t align;
  // for ptr and array
  Type *base;
  union {
    Arr arr;
    Func func;
    Record record;
    Enum enum_;
  };
};
#endif
