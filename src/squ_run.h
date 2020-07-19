#ifndef SQU_RUN
#define SQU_RUN

#include "node.h"

#define CHECK_CTX if(ctx->exc != NULL) return NULL

squ_value* node_expr(squ_ctx*, node*);

squ_value*
node_expr_stmt(squ_ctx* ctx, node* np)
{
  int i;
  node_array* arr = np->value.v.p;
  squ_value* v = NULL;
  for (i = 0; i < arr->len; i++) {
    if (ctx->exc != NULL) {
      return NULL;
    }
    
    v = node_expr(ctx, arr->data[i]);
  }
  return v;
}

static void
node_if_run(squ_ctx* ctx, node_if* nif)
{
  squ_value* v = node_expr(ctx, nif->cond);
  if (v->t == SQU_VALUE_BOOL && v->v.b) 
  {
    node_expr_stmt(ctx, nif->stmt_seq);
  }
  else if (nif->opt_else != NULL)
  {
    node_expr_stmt(ctx, nif->opt_else);
  }
}

static void
node_loop_run(squ_ctx* ctx, node_loop* nloop)
{
  squ_value* v = node_expr(ctx, nloop->cond);

  if(v->t == SQU_VALUE_BOOL)
  {
    while(v->v.b)
    {
      node_expr_stmt(ctx, nloop->stmt_seq);
      v = node_expr(ctx, nloop->cond);
          
      if(v->t == SQU_VALUE_BOOL)
      {
        if(!v->v.b)
        {
          break;
        }
        else
        {
          continue;
        }
      }
      else
      {
        squ_raise(ctx,"The condtion should be bool");
        break;
      }
    }
  }
}

#endif /* SQU_RUN */