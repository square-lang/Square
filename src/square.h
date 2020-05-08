#ifndef _SQU_H_
#define _SQU_H_
#include <stdio.h>
#include <stdint.h>
#include "khash.h"

typedef enum {
  SQU_VALUE_BOOL,
  SQU_VALUE_ARRAY,
  SQU_VALUE_MAP,
  SQU_VALUE_STRING,
  SQU_VALUE_DOUBLE,
  SQU_VALUE_FIXNUM,
  SQU_VALUE_NIL,
  SQU_VALUE_CFUNC,
  SQU_VALUE_USER,
  SQU_VALUE_ERROR,
} squ_value_type;

typedef intptr_t squ_id;
typedef double squ_double;
typedef char* squ_string;

typedef struct {
  squ_value_type t;
  union {
    int b;
    int i;
    double d;
    char* s;
    void* p;
    squ_id id;
  } v;
} squ_value;

typedef struct {
  int len;
  int max;
  void** data;
} squ_array;

KHASH_MAP_INIT_STR(value, squ_value*)

typedef khash_t(value) squ_env;

typedef struct {
  int type;
  squ_value* arg;
} squ_error;

typedef struct {
  khash_t(value)* env;
  squ_error* exc;
} squ_ctx;

typedef struct parser_state {
  int nerr;
  void *lval;
  const char *fname;
  int lineno;
  int tline;
  /* TODO: should be separated as another context structure */
  squ_ctx ctx;
} parser_state;

typedef squ_value* (*squ_cfunc)(squ_ctx*, squ_array*);

int squ_parse_init(parser_state*);
void squ_parse_free(parser_state*);
int squ_parse_file(parser_state*, const char*);
int squ_parse_input(parser_state*, FILE* in, const char*);
int squ_parse_string(parser_state*, const char*);
int squ_run(parser_state*);
void squ_raise(squ_ctx*, const char*);

#endif /* _SQU_H_ */
