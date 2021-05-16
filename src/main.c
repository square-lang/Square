#include "square.h"
#include "node.h"
#include <stdio.h>

FILE *yyin, *yyout;

int
squ_parse_file(parser_state* p, const char* file_name)
{
  int r;
  FILE* fp = fopen(file_name, "rb");
  if (fp == NULL) {
    perror("fopen");
    return 1;
  }
  r = squ_parse_input(p, fp, file_name);
  fclose(fp);
  return r;
}

int
squ_parse_input(parser_state* p, FILE *f, const char *file_name)
{
  int n;

  yyin = f;
  n = yyparse(p);
  p->file_name = file_name;
  if (n == 0 && p->nerr == 0) {
    printf("[%s]:  syntax OK\n", p->file_name);
    return 0;
  }
  else{
    printf("Line:%d Syntax error!",p -> lineno);
  }
  return 1;
}

int
squ_parse_init(parser_state *p)
{
  p->nerr = 0;
  p->lval = NULL;
  p->file_name = NULL;
  p->lineno = 1;
  p->tline = 1;
  p->ctx.exc = NULL;
  p->ctx.env = kh_init(value);
  return 0;
}

void
squ_parse_free(parser_state* p)
{
  node_free(p->lval);
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
  case NODE_OP:
    node_free(((node_op*) np->value.v.p)->lhs);
    node_free(((node_op*) np->value.v.p)->rhs);
    free(np);
    break;
  case NODE_LET:
    node_free(((node_let*) np->value.v.p)->lhs);
    node_free(((node_let*) np->value.v.p)->rhs);
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
  case NODE_FDEF:
    node_free(((node_fdef*) np->value.v.p)->ident);
    node_free(((node_fdef*) np->value.v.p)->args);
    node_free(((node_fdef*) np->value.v.p)->blk);
    node_free(((node_fdef*) np->value.v.p)->ret);
    free(np);
    break;
  case NODE_PRINT:
    node_free(((node_print*) np->value.v.p)->args);
    free(np);
  case NODE_LOOP:
    node_free(((node_loop*) np->value.v.p)->cond);
    node_free(((node_loop*) np->value.v.p)->stmt_seq);
    free(np);
    break;
  case NODE_LAMBDA:
    node_free(((node_lambda*)np->value.v.p)->body);
    node_free(((node_lambda*)np->value.v.p)->args);
    node_free(((node_lambda*)np->value.v.p)->args_value);
    node_free(((node_lambda*)np->value.v.p)->ret);
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
    switch (np -> value.t) {
    case SQU_VALUE_DOUBLE:    /* double */
      free(np);
      break;
    case SQU_VALUE_STRING:    /* string */
      free(np->value.v.s);
      free(np);
      break;
    case SQU_VALUE_INT:       /* int */
      free(np);
      break;
    case SQU_VALUE_BOOL:      /* bool */
      break;
    case SQU_VALUE_NULL:      /* null */
      break;
    case SQU_VALUE_ARRAY:     /* array */
      node_array_free(np);
      break;
    default:
      break;
    }
    break;
  default:
    break;
  }
}

int
main(int argc, const char** argv)
{
  int i, n = 0;
  parser_state state;

  squ_parse_init(&state);

  if (argc <= 1) {              /* no args */
    n = squ_parse_input(&state, stdin, "stdin");
  }
  else 
  {
    for (i = 1; i<argc; i++) {
      n += squ_parse_file(&state, argv[i]);
    }
  }

  if (n == 0) {
    squ_run(&state);
  }
  
  squ_parse_free(&state);
  
  return n > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}