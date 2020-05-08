#include "square.h"
#include "node.h"
#include "parse.tab.h"
#include <stdlib.h>
#include <string.h>

FILE *yyin, *yyout;

#define SQU_ERROR_RUNTIME 0
#define SQU_ERROR_RETURN 1

static char*
strdup0(const char *s)
{
  size_t len = strlen(s);
  char *p;

  p = (char*)malloc(len+1);
  if (p) {
    strcpy(p, s);
  }
  return p;
}

static char*
strndup0(const char *s, size_t n)
{
  size_t i;
  const char *p = s;
  char *new;

  for (i=0; i<n && *p; i++,p++)
    ;
  new = (char*)malloc(i+1);
  if (new) {
    memcpy(new, s, i);
    new[i] = '\0';
  }
  return new;
}

node*
node_value_new(node* v)
{
  /* TODO */
  return NULL;
}

squ_array*
squ_array_new() {
  /* TODO: error check */
  squ_array* arr = malloc(sizeof(squ_array));
  arr->len = 0;
  arr->max = 0;
  arr->data = NULL;
  return arr;
}

void
squ_array_add(squ_array* arr, void* data) {
  if (arr->len == arr->max) {
    arr->max = arr->len + 10;
    arr->data = realloc(arr->data, sizeof(void*) * arr->max);
  }
  /* TODO: error check */
  arr->data[arr->len] = data;
  arr->len++;
}

node*
node_array_new()
{
  /* TODO: error check */
  node* np = malloc(sizeof(node));
  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_ARRAY;
  np->value.v.p = squ_array_new();
  return np;
}

node*
node_array_of(node* np)
{
  if (np == NULL)
    np = node_array_new();
  return np;
}

void
node_array_add(node* arr, node* np)
{
  node_array* arr0 = arr->value.v.p;
  squ_array_add(arr0, np);
}

void
node_array_free(node* np)
{
  int i;
  node_array* arr = np->value.v.p;
  for (i = 0; i < arr->len; i++)
    node_free(arr->data[i]);
  free(arr);
  free(np);
}

node*
node_pair_new(node* key, node* value)
{
  node_pair* npair = malloc(sizeof(node_pair));
  npair->key = key;
  npair->value = value;

  node* np = malloc(sizeof(node));
  np->type = NODE_PAIR;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = npair;
  return np;
}

node*
node_map_new()
{
  node_array* arr = malloc(sizeof(node_array));
  /* TODO: error check */
  arr->len = 0;
  arr->max = 0;
  arr->data = NULL;

  node* np = malloc(sizeof(node));
  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_MAP;
  np->value.v.p = arr;
  return np;
}

node*
node_map_of(node* np)
{
  if (np == NULL)
    np = node_map_new();
  return np;
}

void
node_map_free(node* np)
{
  int i;
  node_array* arr = np->value.v.p;
  for (i = 0; i < arr->len; i++) {
    node* pair = arr->data[i];
    node_pair* npair = pair->value.v.p;
    node_free(npair->key);
    node_free(npair->value);
    free(npair);
    free(pair);
  }
  free(arr);
  free(np);
}

node*
node_let_new(node* lhs, node* rhs)
{
  node_let* nlet = malloc(sizeof(node_let));
  nlet->lhs = lhs;
  nlet->rhs = rhs;

  node* np = malloc(sizeof(node));
  np->type = NODE_LET;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nlet;
  return np;
}

node*
node_op_new(char* op, node* lhs, node* rhs)
{
  node_op* nop = malloc(sizeof(node_op));
  nop->lhs = lhs;
  nop->op = op;
  nop->rhs = rhs;

  node* np = malloc(sizeof(node));
  np->type = NODE_OP;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nop;
  return np;
}

node*
node_block_new(node* args, node* stmt_seq)
{
  node_block* block = malloc(sizeof(node_block));
  block->args = args;
  block->stmt_seq = stmt_seq;

  node* np = malloc(sizeof(node));
  np->type = NODE_BLOCK;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = block;
  return np;
}

node*
node_call_new(node* cond, node* ident, node* args, node* blk)
{
  node_call* ncall = malloc(sizeof(node_call));
  ncall->cond = cond;
  ncall->ident = ident;
  ncall->args = args;
  ncall->blk = blk;

  node* np = malloc(sizeof(node));
  np->type = NODE_CALL;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = ncall;
  return np;
}

node*
node_double_new(squ_double d)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_DOUBLE;
  np->value.v.d = d;
  return np;
}

node*
node_string_new(squ_string s)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_STRING;
  np->value.v.s = strdup0(s);
  return np;
}

node*
node_string_len_new(squ_string s, size_t l)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_VALUE;
  np->value.t = SQU_VALUE_STRING;
  np->value.v.s = strndup0(s, l);
  return np;
}

node*
node_ident_new(squ_id id)
{
  node* np = malloc(sizeof(node));

  np->type = NODE_IDENT;
  np->value.t = SQU_VALUE_FIXNUM;
  np->value.v.id = id;
  return np;
}

squ_id
node_ident_of(char* s)
{
  /* TODO: get id of the identifier which named as s */
  return (squ_id) strdup0(s);
}

node*
node_null()
{
  static node nd = { NODE_VALUE, { SQU_VALUE_NIL, {0} } };
  return &nd;
}

node*
node_true()
{
  static node nd = { NODE_VALUE, { SQU_VALUE_BOOL, {1} } };
  return &nd;
}

node*
node_false()
{
  static node nd = { NODE_VALUE, { SQU_VALUE_BOOL, {0} } };
  return &nd;
}

node*
node_if_new(node* cond, node* stmt_seq, node* opt_else)
{
  node_if* nif = malloc(sizeof(node_if));
  nif->cond = cond;
  nif->stmt_seq = stmt_seq;
  nif->opt_else = opt_else;

  node* np = malloc(sizeof(node));
  np->type = NODE_IF;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = nif;
  return np;
}

node*
node_return_new(node* value)
{
  node* np = malloc(sizeof(node));
  np->type = NODE_RETURN;
  np->value.t = SQU_VALUE_USER;
  np->value.v.p = value;
  return np;
}

node*
node_import_new(squ_id name)
{
  node_import* nimp = malloc(sizeof(nimp));
  nimp -> name = name;
  
  node* np = malloc(sizeof(node));
  np -> type = NODE_IMPORT;
  np -> value.t = SQU_VALUE_USER;
  np -> value.v.p = nimp;
  np -> value.v.id = nimp -> name;

  return np;
}

node*
node_break_new(node* value)
{
  static node nd = { NODE_BREAK };
  return &nd;
}

int
squ_parse_init(parser_state *p)
{
  p->nerr = 0;
  p->lval = NULL;
  p->fname = NULL;
  p->lineno = 1;
  p->tline = 1;
  p->ctx.exc = NULL;
  p->ctx.env = kh_init(value);
  return 0;
}

int
squ_parse_file(parser_state* p, const char* fname)
{
  int r;
  FILE* fp = fopen(fname, "rb");
  if (fp == NULL) {
    perror("fopen");
    return 1;
  }
  r = squ_parse_input(p, fp, fname);
  fclose(fp);
  return r;
}

int
squ_parse_input(parser_state* p, FILE *f, const char *fname)
{
  int n;

  /*yydebug = 1;*/
  yyin = f;
  n = yyparse(p);
  if (n == 0 && p->nerr == 0) {
    printf("syntax OK\n");
    return 0;
  }
  else{
    printf("Line:%d Syntax error!",p -> lineno);
  }
  return 1;
}

void
node_free(node* np) {
  if (!np) {
    return;
  }

  switch (np->type) {
  case NODE_ARGS:
    node_array_free(np);
    break;
  case NODE_IF:
    node_free(((node_if*)np->value.v.p)->cond);
    node_free(((node_if*)np->value.v.p)->stmt_seq);
    node_free(((node_if*)np->value.v.p)->opt_else);
    free(np);
    break;
  case NODE_EMIT:
    node_free((node*) np->value.v.p);
    free(np);
    break;
  case NODE_OP:
    node_free(((node_op*) np->value.v.p)->lhs);
    node_free(((node_op*) np->value.v.p)->rhs);
    free(np);
    break;
  case NODE_BLOCK:
    node_free(((node_block*) np->value.v.p)->args);
    node_free(((node_block*) np->value.v.p)->stmt_seq);
    free(np);
    break;
  case NODE_CALL:
    node_free(((node_call*) np->value.v.p)->cond);
    node_free(((node_call*) np->value.v.p)->ident);
    node_free(((node_call*) np->value.v.p)->args);
    node_free(((node_call*) np->value.v.p)->blk);
    free(np);
    break;
  case NODE_RETURN:
    node_free((node*) np->value.v.p);
    free(np);
    break;
  case NODE_IDENT:
    free(np);
    break;
  case NODE_VALUE:
    switch (np->value.t) {
    case SQU_VALUE_DOUBLE:
      free(np);
      break;
    case SQU_VALUE_STRING:
      free(np->value.v.s);
      free(np);
      break;
    case SQU_VALUE_BOOL:
      break;
    case SQU_VALUE_NIL:
      break;
    case SQU_VALUE_ARRAY:
      node_array_free(np);
      break;
    case SQU_VALUE_MAP:
      node_map_free(np);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

void
squ_parse_free(parser_state* p)
{
  node_free(p->lval);
}