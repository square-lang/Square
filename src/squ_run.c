#include "node.h"
#include "square.h"

#include <assert.h>
#include <stdio.h>

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

void 
squ_fun_def(parser_state* p,squ_string func_name, void* func_p)
{
  int r;
  khiter_t k;

  static squ_value v;
  k = kh_put(value,p->ctx.env,func_name,&r);
  v.t = SQU_VALUE_CFUNC;
  v.v.p = func_p;
  kh_value(p->ctx.env,k) = &v;
}

void
squ_var_def(squ_ctx* ctx,squ_string var_name,squ_value* v)
{
  int ret;
  khiter_t k;
  k = kh_put(value,ctx->env,var_name,&ret);
  if(ret <= 0)
  {
    return;
  }
  kh_value(ctx->env,k) = v;
}

squ_value*
squ_var_get(squ_ctx* ctx, squ_string name)
{
  khint_t k = kh_get(value, ctx->env, name);
  squ_value* v = kh_value(ctx->env, k);
  return v;
}

void
squ_var_reset(squ_ctx* ctx, squ_string name, squ_value* v)
{
  khint_t k = kh_get(value, ctx->env, name);
  kh_value(ctx->env,k) = v;
}

squ_value*
node_expr(squ_ctx* ctx, node* np)
{
  if (ctx->exc != NULL) {
    return NULL;
  }

  if (np == NULL) {
    return NULL;
  }

  switch (np->type) 
  {
  case NODE_IF:
    {
      node_if* nif = np->value.v.p;
      squ_value* v = node_expr(ctx, nif->cond);
      if (ctx->exc != NULL)
      {
        return NULL;
      }
      if (v->t == SQU_VALUE_BOOL && v->v.b) 
      {
        node_expr_stmt(ctx, nif->stmt_seq);
      }
      else if (nif->opt_else != NULL)
      {
        node_expr_stmt(ctx, nif->opt_else);
      }
    }
    break;
  case NODE_OP:
    {
      node_op* nop = np->value.v.p;
      squ_value* lhs = node_expr(ctx, nop->lhs);
      if (ctx->exc != NULL) 
        return NULL;
      if (*nop->op == '+' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_STRING && rhs->t == SQU_VALUE_STRING) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          char *p = malloc(strlen(lhs->v.s) + strlen(rhs->v.s) + 1);
          strcpy(p, lhs->v.s);
          strcat(p, rhs->v.s);
          new->t = SQU_VALUE_STRING;
          new->v.s = p;
          return new;
        } 
        else if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          new->v.d = lhs->v.d + rhs->v.d;
          return new;
        }
        else if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i + rhs->v.i;
          return new;
        }
        else if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i + rhs->v.i;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d + rhs->v.d;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          }
        }
      }


      if (*nop->op == '-' && *(nop->op+1) == '\0') {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
       else if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          new->v.d = lhs->v.d - rhs->v.d;
          return new;
        }
        else if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i - rhs->v.i;
          return new;
        }
        else if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i - rhs->v.i;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d - rhs->v.d;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          }
        }
      }


      if (*nop->op == '*' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        else if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          new->v.d = lhs->v.d * rhs->v.d;
          return new;
        }
        else if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i * rhs->v.i;
          return new;
        }
        else if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i * rhs->v.i;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d * rhs->v.d;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          }
        }
      }


      if (*nop->op == '/' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        else if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_DOUBLE;
          
          new->v.d = lhs->v.d / rhs->v.d;
          return new;
        }
        else if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;
          new->v.i = lhs->v.i / rhs->v.i;
          return new;
        }
        else if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i / rhs->v.i;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = squ_var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d / rhs->v.d;
              squ_var_reset(ctx,lhs->v.id,v);
              return v;
              break;
            }
          }
        }
      }

      
      if (*nop->op == '%' && *(nop->op+1) == '\0') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_INT;

          new->v.i = lhs->v.i % rhs->v.i;
          return new;
        }
        if (lhs->t == SQU_VALUE_IDENT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* v = squ_var_get(ctx,lhs->v.id);
          v->t = SQU_VALUE_INT;
          v->v.i = v->v.i % rhs->v.i;
          squ_var_reset(ctx,lhs->v.id,v);
          return v;
        } 
      }


      if (*nop->op == '<') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.d <= rhs->v.d;
          else
            new->v.b = lhs->v.d < rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.i <= rhs->v.i;
          else
            new->v.b = lhs->v.i < rhs->v.i;
          return new;
        }
        if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
          {
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            squ_value* new = malloc(sizeof(squ_value));
            new->t = SQU_VALUE_BOOL;
            if(*(nop->op+1) == '=')
            {
              new ->v.b = v->v.i <= rhs->v.i;
            }
            else
            {
              new ->v.b = v->v.i < rhs->v.i; 
            }
            return new;
            break;
          }
          case SQU_VALUE_DOUBLE:
           {
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            squ_value* new = malloc(sizeof(squ_value));
            new->t = SQU_VALUE_BOOL;
            if(*(nop->op+1) == '=')
            {
              new ->v.b = v->v.d <= rhs->v.d;
            }
            else
            {
              new ->v.b = v->v.d < rhs->v.d; 
            }
            return new;
            break;
          }
          case SQU_VALUE_IDENT:
            break;
          default:
            break;
          }
        }
      }


      if (*nop->op == '>') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.d >= rhs->v.d;
          else
            new->v.b = lhs->v.d > rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          if (*(nop->op+1) == '=')
            new->v.b = lhs->v.i >= rhs->v.i;
          else
            new->v.b = lhs->v.i > rhs->v.i;
          return new;
        }
        if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
          {
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            squ_value* new = malloc(sizeof(squ_value));
            new->t = SQU_VALUE_BOOL;
            if(*(nop->op+1) == '=')
            {
              new ->v.b = v->v.i >= rhs->v.i;
            }
            else
            {
              new ->v.b = v->v.i > rhs->v.i; 
            }
            return new;
            break;
          }
          case SQU_VALUE_DOUBLE:
           {
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            squ_value* new = malloc(sizeof(squ_value));
            new->t = SQU_VALUE_BOOL;
            if(*(nop->op+1) == '=')
            {
              new ->v.b = v->v.d >= rhs->v.d;
            }
            else
            {
              new ->v.b = v->v.d > rhs->v.d; 
            }
            return new;
            break;
          }
          case SQU_VALUE_IDENT:
            break;
          default:
            break;
          }
        }
      }


      if (*nop->op == '=' && (*(nop->op+1)) == '=') 
      {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE) 
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.d == rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.i == rhs->v.i;
          return new;
        }
        if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
          {
            squ_value* new = malloc(sizeof(squ_value));
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            new->t = SQU_VALUE_BOOL;
            new->v.b = v->v.i == rhs->v.i;
            return new;
            break;
          }
          case SQU_VALUE_DOUBLE:
           {
            squ_value* new = malloc(sizeof(squ_value));
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            new->t = SQU_VALUE_BOOL;
            new->v.b = v->v.d == rhs->v.d;
            return new;
            break;
           }
          case SQU_VALUE_IDENT:
            break;
          default:
            break;
          }
        }
      }


      if (*nop->op == '!' && (*(nop->op+1)) == '=') {
        squ_value* rhs = node_expr(ctx, nop->rhs);
        if (ctx->exc != NULL) 
          return NULL;
        if (lhs->t == SQU_VALUE_DOUBLE && rhs->t == SQU_VALUE_DOUBLE)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.d != rhs->v.d;
          return new;
        }
        if (lhs->t == SQU_VALUE_INT && rhs->t == SQU_VALUE_INT)
        {
          squ_value* new = malloc(sizeof(squ_value));
          new->t = SQU_VALUE_BOOL;
          new->v.b = lhs->v.i != rhs->v.i;
          return new;
        }
        if(lhs->t == SQU_VALUE_IDENT)
        {
          switch (rhs->t)
          {
          case SQU_VALUE_INT:
          {
            squ_value* new = malloc(sizeof(squ_value));
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            new->t = SQU_VALUE_BOOL;
            new->v.b = v->v.i != rhs->v.i;
            return new;
            break;
          }
          case SQU_VALUE_DOUBLE:
           {
            squ_value* new = malloc(sizeof(squ_value));
            squ_value* v = squ_var_get(ctx,lhs->v.id);
            new->t = SQU_VALUE_BOOL;
            new->v.b = v->v.d != rhs->v.d;
            return new;
            break;
           }
          case SQU_VALUE_IDENT:
            break;
          default:
            break;
          }
        }
      }
      squ_raise(ctx, "invalid operator");
    }
    break;
  case NODE_CALL:
    {
      node_call* ncall = np->value.v.p;
      if (ncall->ident != NULL) 
      {
        khint_t k = kh_get(value, ctx->env, ncall->ident->value.v.id);
        if (k != kh_end(ctx->env)) 
        {
          squ_value* v = kh_value(ctx->env, k);
          if (v->t == SQU_VALUE_CFUNC) 
          {
            node_array* arr0 = ncall->args->value.v.p;
            squ_array* arr1 = squ_array_new();
            int i;
            for (i = 0; i < arr0->len; i++)
              squ_array_add(arr1, node_expr(ctx, arr0->data[i]));
            ((squ_cfunc) v->v.p)(ctx, arr1);
          }
        }
        else 
        {
          squ_raise(ctx, "function not found!");
        }
      } 
      if(ncall->blk != NULL)
      {
        node_block* nblk = ncall->blk->value.v.p;
        node_expr_stmt(ctx, nblk->stmt_seq);
        if (ctx->exc != NULL) {
          squ_value* arg = ctx->exc->arg;
          free(ctx->exc);
          ctx->exc = NULL;
          return arg;
        }
      }
    }
    break;
  case NODE_RETURN:
    {
      node_return* nreturn = np->value.v.p;
      ctx->exc = malloc(sizeof(squ_error));
      ctx->exc->arg = node_expr(ctx, nreturn->rv);
      return NULL;
    }
    break;
  case NODE_LET:
    {
      node_let* nlet = np->value.v.p;
      squ_value* v_r;  /* right */
      squ_value* v_l;  /* left */
      khiter_t k;
      v_r = node_expr(ctx, nlet->rhs);
      v_l = node_expr(ctx, nlet->lhs);
      if(v_l != NULL)
      {
        squ_var_def(ctx,v_l->v.id,v_r);
      }
    }
    break;
  case NODE_VALUE:
    return &np->value;
    break;
  case NODE_IDENT:
  {
    /* TODO: return the identifier's value */
    return &np->value; 
    break;
  }
  default:
    break;
  }
  return NULL;
}

squ_value*
squ_cputs(squ_ctx* ctx, FILE* out, squ_array* args)
{
  
  int i;
  for (i = 0; i < args->len; i++) 
  {
    squ_value* v;
    if (i != 0)
      fprintf(out, ", ");
    v = args->data[i];
    if (v != NULL) 
    {
      switch (v->t) 
      {
      case SQU_VALUE_DOUBLE:
        fprintf(out, "%f\n", v->v.d);
        break;
      case SQU_VALUE_STRING:
        fprintf(out, "%s\n", v->v.s);
        break;
      case SQU_VALUE_IDENT:
        {
          squ_value* v1 = squ_var_get(ctx,v->v.id);
          switch(v1->t)
          {
            case SQU_VALUE_INT:
              fprintf(out,"%d\n",v1->v.i);
              break;
            case SQU_VALUE_DOUBLE:
              fprintf(out,"%f\n",v1->v.d);
              break;
            case SQU_VALUE_NULL:
              fprintf(out,"null\n");
              break;
            case SQU_VALUE_STRING:
              fprintf(out,"%s\n",v1->v.s);
              break;
            case SQU_VALUE_BOOL:
              fprintf(out, v1->v.b ? "true\n" : "false\n");
              break;
          }
          break;
        }
      case SQU_VALUE_NULL:
        fprintf(out, "null\n");
        break;
      case SQU_VALUE_BOOL:
        fprintf(out, v->v.b ? "true\n" : "false\n");
        break;
      case SQU_VALUE_INT:
        fprintf(out,"%d\n",v->v.i);
        break;
      case SQU_VALUE_ERROR:
        fprintf(out, "%s\n", v->v.s);
        break;
      case SQU_VALUE_CFUNC:
        fprintf(out,"<%p>\n",v->v.p);
        break;
      default:
        fprintf(out, "<%p>\n", v->v.p);
        break;
      }
    }
    else
    {
      fprintf(out,"null");
    }
  }
  return NULL;
}

squ_value*
squ_puts(squ_ctx* ctx, squ_array* args) {
  return squ_cputs(ctx, stdout, args);
}

int
squ_run(parser_state* p)
{
  squ_fun_def(p,"cat",squ_puts);
  squ_fun_def(p,"print",squ_puts);
  node_expr_stmt(&p->ctx, (node*)p->lval);
  if (p->ctx.exc != NULL) 
  {
    squ_array* arr = squ_array_new();
    squ_array_add(arr, p->ctx.exc->arg);
    p->ctx.exc = NULL;
  }
  return 0;
}

void
squ_raise(squ_ctx* ctx, const char* msg) {
  ctx->exc = malloc(sizeof(squ_error));
  ctx->exc->arg = malloc(sizeof(squ_value));
  ctx->exc->arg->v.s = strdup(msg);
}