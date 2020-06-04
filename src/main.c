#include "square.h"
#include "node.h"
#include <stdio.h>

FILE *yyin, *yyout;

static void
dump_node(node* np, int indent) {
  int i;
  for (i = 0; i < indent; i++)
    putchar(' ');

  if (!np) {
    printf("NULL\n");
    return;
  }

  switch (np->type) {
  case NODE_ARGS:
    {
      node_array* arr0 = np->value.v.p;
      for (i = 0; i < arr0->len; i++)
        dump_node(arr0->data[i], indent+1);
    }
    break;
  case NODE_IF:
    {
      printf("IF:\n");
      dump_node(((node_if*)np->value.v.p)->cond, indent+1);
      for (i = 0; i < indent; i++)
        putchar(' ');
      printf("THEN:\n");
      dump_node(((node_if*)np->value.v.p)->stmt_seq, indent+1);
      node* opt_else = ((node_if*)np->value.v.p)->opt_else;
      if (opt_else != NULL) {
        for (i = 0; i < indent; i++)
          putchar(' ');
        printf("ELSE:\n");
        dump_node(opt_else, indent+1);
      }
    }
    break;
  case NODE_LOOP:
    {
      printf("LOOP:\n");
      dump_node(((node_loop*)np->value.v.p)->cond,indent+1);
      for (i = 0; i < indent; i++)
        putchar(' ');
      dump_node(((node_loop*)np->value.v.p)->stmt_seq,indent+1);
    }
    break;
  case NODE_OP:
    printf("OP:\n");
    dump_node(((node_op*) np->value.v.p)->lhs, indent+1);
    for (i = 0; i < indent+1; i++)
      putchar(' ');
    puts(((node_op*) np->value.v.p)->op);
    dump_node(((node_op*) np->value.v.p)->rhs, indent+1);
    break;
  case NODE_BLOCK:
    printf("BLOCK:\n");
    dump_node(((node_block*) np->value.v.p)->args, indent+1);
    dump_node(((node_block*) np->value.v.p)->stmt_seq, indent+1);
    break;
  case NODE_LET:
    printf("LET:\n");
    dump_node(((node_let*) np->value.v.p)->lhs,indent+1);
    dump_node(((node_let*) np->value.v.p)->rhs,indent+1);
    break;
  case NODE_CALL:
    printf("CALL:\n");
    dump_node(((node_call*) np->value.v.p)->cond, indent+2);
    dump_node(((node_call*) np->value.v.p)->ident, indent+2);
    dump_node(((node_call*) np->value.v.p)->args, indent+2);
    dump_node(((node_call*) np->value.v.p)->blk, indent+2);
    break;
  case NODE_RETURN:
    printf("RETURN:\n");
    dump_node(((node_return*) np->value.v.p)->rv, indent+1);
    break;
  case NODE_IDENT:
    printf("IDENT: %s\n", (void*)np->value.v.id);
    break;
  case NODE_IMPORT:
    printf("IMPORT: %s\n",(void*)np->value.v.id);
    break;
  case NODE_VALUE:
    switch (np->value.t) {
    case SQU_VALUE_DOUBLE:
      printf("VALUE(NUMBER): %f\n", np->value.v.d);
      break;
    case SQU_VALUE_STRING:
      printf("VALUE(STRING): %s\n", np->value.v.s);
      break;
    case SQU_VALUE_BOOL:
      printf("VALUE(BOOL): %s\n", np->value.v.b ? "true" : "false");
      break;
    case SQU_VALUE_INT:
      printf("VALUE(INT): %d\n",np->value.v.i);
      break;
    case SQU_VALUE_NULL:
      printf("VALUE(NULL): null\n");
      break;
    case SQU_VALUE_ARRAY:
      printf("VALUE(ARRAY):\n");
      {
        node_array* arr0 = np->value.v.p;
        for (i = 0; i < arr0->len; i++)
          dump_node(arr0->data[i], indent+1);
      }
      break;
    case SQU_VALUE_MAP:
      printf("VALUE(MAP):\n");
      {
        node_array* arr0 = np->value.v.p;
        for (i = 0; i < arr0->len; i++) {
          node* pair = arr0->data[i];
          node_pair* pair0 = pair->value.v.p;
          dump_node(pair0->key, indent+1);
          dump_node(pair0->value, indent+1);
        }
      }
      break;
    default:
      printf("VALUE(UNKNOWN): %p\n", np->value.v.p);
      break;
    }
    break;
  default:
    printf("UNKNWON(%d)\n", np->type);
    break;
  }
}

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
    node_free(np);
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
  case NODE_LOOP:
    node_free(((node_loop*) np->value.v.p)->cond);
    node_free(((node_loop*) np->value.v.p)->stmt_seq);
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
    case SQU_VALUE_MAP:       /* map */
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

int
main(int argc, const char** argv)
{
  int i, n = 0;
  parser_state state;

  squ_parse_init(&state);

  if (argc == 1) {              /* no args */
    n = squ_parse_input(&state, stdin, "stdin");
  }
  else 
  {
    for (i=1; i<argc; i++) {
      n += squ_parse_file(&state, argv[i]);
    }
  }

  if (n == 0) {
    dump_node(state.lval, 0);
    squ_run(&state);
  }

  squ_parse_free(&state);
  return n > 0 ? EXIT_FAILURE : EXIT_SUCCESS;
}