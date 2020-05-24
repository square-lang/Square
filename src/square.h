#ifndef _SQU_H_
#define _SQU_H_
#include <stdio.h>
#include <stdint.h>
#include "khash.h"

typedef enum{
  SQU_VALUE_BOOL,          /* bool */
  SQU_VALUE_INT,           /* int */
  SQU_VALUE_ARRAY,         /* array */
  SQU_VALUE_MAP,           /* map */
  SQU_VALUE_PTR,           /* point */
  SQU_VALUE_STRING,        /* string */
  SQU_VALUE_DOUBLE,        /* double */
  SQU_VALUE_IDENT,        /* identifier */        
  SQU_VALUE_NULL,          /* null */
  SQU_VALUE_CFUNC,         /* funtion */
  SQU_VALUE_USER,          /* user data */
  SQU_VALUE_ERROR,         /* error */
} squ_value_type;

/* define data type in square */
typedef char* squ_id;   /* identifier */
typedef int squ_bool;      /* bool */
typedef int squ_int;       /* int */
typedef double squ_double; /* double */
typedef char* squ_string;  /* string */

typedef struct {
  int len;       /* length of the array */
  int max;       /* max length of array */
  void** data;   /* array's data */
} squ_array;

/* "Value" in square, include its type and value */
typedef struct {
  squ_value_type t;
  union {
    squ_bool b;       /* bool */
    squ_int i;       /* int */
    squ_double d;    /* double */
    squ_string s;     /* string */
    void* p;     /* point */
    squ_id id;   /* identifier */
  } v;
} squ_value;

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
  size_t nerr;
  void* lval;
  const char* file_name;
  int lineno;
  int tline;
  squ_ctx ctx;
} parser_state;

typedef squ_value* (*squ_cfunc)(squ_ctx*, squ_array*);

/* Build-in function in Square */
squ_value* squ_puts(squ_ctx*, squ_array*);
squ_value* squ_input(squ_ctx*, squ_array*);
squ_value* squ_exit(squ_ctx*, squ_array*);
squ_int squ_open(squ_ctx*, squ_array*);
squ_int squ_write(squ_ctx*, squ_array*);

int squ_parse_init(parser_state*);
void squ_parse_free(parser_state*);
int squ_parse_file(parser_state*, const char*);
int squ_parse_input(parser_state*, FILE* in, const char*);
int squ_parse_string(parser_state*, const char*);
int squ_run(parser_state*);
void squ_raise(squ_ctx*, const char*);

#endif