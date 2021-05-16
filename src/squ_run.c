#include "node.h"
#include "vm_stack.h"
#include "squ_vm.h"

/* Square's build-in libraries */
#include "squ_math.h"

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
      return ctx->exc->arg;
    }
    
    v = node_expr(ctx, arr->data[i]);
  }
  return v;
}

void
squ_var_def(squ_ctx* ctx, squ_string var_name, squ_value* v)
{
  int ret;
  khiter_t k;
  k = kh_put(value, ctx->env, var_name, &ret);
  kh_value(ctx->env, k) = v;
}

squ_value*
var_get(squ_ctx* ctx, squ_string name)
{
  khint_t k = kh_get(value, ctx->env, name);
  return kh_value(ctx->env, k);
}

squ_value*
squ_var_get(squ_ctx* ctx, squ_string name)
{
  return var_get(ctx, name);
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

  case NODE_LOOP:
    {
      node_loop* nloop = np->value.v.p;
      squ_value* v = node_expr(ctx, nloop->cond);
      if (ctx->exc != NULL)
      {
        return NULL;
      }
      if(v->t == SQU_VALUE_BOOL)
      {
        while(v->v.b = TRUE)
        {
          node_expr_stmt(ctx, nloop->stmt_seq);
          v = node_expr(ctx, nloop->cond);
          
          if(v->t == SQU_VALUE_BOOL)
          {
            if(v->v.b == FALSE)
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
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i + rhs->v.i;
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d + rhs->v.d;
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
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i - rhs->v.i;
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d - rhs->v.d;
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
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i * rhs->v.i;
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d * rhs->v.d;
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
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_INT;
              v->v.i = v->v.i / rhs->v.i;
              return v;
              break;
            }
          case SQU_VALUE_DOUBLE:
            {
              squ_value* v = var_get(ctx,lhs->v.id);
              v->t = SQU_VALUE_DOUBLE;
              v->v.d = v->v.d / rhs->v.d;
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
          squ_value* v = var_get(ctx,lhs->v.id);
          v->t = SQU_VALUE_INT;
          v->v.i = v->v.i % rhs->v.i;
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
            squ_value* v = var_get(ctx,lhs->v.id);
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
            squ_value* v = var_get(ctx,lhs->v.id);
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
            squ_value* v = var_get(ctx,lhs->v.id);
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
            squ_value* v = var_get(ctx,lhs->v.id);
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
            squ_value* v = var_get(ctx,lhs->v.id);
            new->t = SQU_VALUE_BOOL;
            new->v.b = v->v.i == rhs->v.i;
            return new;
            break;
          }
          case SQU_VALUE_DOUBLE:
           {
            squ_value* new = malloc(sizeof(squ_value));
            squ_value* v = var_get(ctx,lhs->v.id);
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
            squ_value* v = var_get(ctx,lhs->v.id);
            new->t = SQU_VALUE_BOOL;
            new->v.b = v->v.i != rhs->v.i;
            return new;
            break;
          }
          case SQU_VALUE_DOUBLE:
           {
            squ_value* new = malloc(sizeof(squ_value));
            squ_value* v = var_get(ctx,lhs->v.id);
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
      squ_value* v = var_get(ctx, ncall->ident->value.v.id);
      if (v->t == SQU_VALUE_CFUNC) 
      {
        node_array* arr0 = ncall->args->value.v.p;
        squ_array* arr1 = squ_array_new();
        int i;
        for (i = 0; i < arr0->len; i++)
          squ_array_add(arr1, node_expr(ctx, arr0->data[i]));
        return ((squ_cfunc) v->v.p)(ctx, arr1);
      }
      else if(v->t == SQU_VALUE_USER)
      {
        node_array* arr0 = ncall->args->value.v.p;
        squ_array* arr1 = squ_array_new();
        int i;
        for(i = 0; i < arr0->len; i++)
        {
          squ_value* v = node_expr(ctx, arr0->data[i]);
          if (v->t == SQU_VALUE_IDENT)
            squ_var_reset(ctx, lambda->args->value.v.id, squ_var_get(ctx, v->v.id));
          else
            squ_var_reset(ctx, lambda->args->value.v.id, v);
        }
        node_expr_stmt(ctx, lambda->body);
        return &lambda->ret->value;
      }
      else
      {
        squ_raise(ctx, "function not found!");
      }
    }
    break;

  case NODE_LAMBDA:
    {
      node_lambda* nlambda = np->value.v.p;
      if(lambda == NULL)
      {
        lambda = malloc(sizeof(squ_lambda));
      }
      if(nlambda->args != NULL)
      {
        lambda->args = (node*)nlambda->args;
        node_array* arr0 = nlambda->args->value.v.p;
        squ_array* arr1 = squ_array_new();
        int i;
        for(i = 0; i < arr0->len; i++)
        {
          squ_array_add(arr1, node_expr(ctx, arr0->data[i]));
          squ_value* v = node_expr(ctx, arr0->data[i]);
          lambda->args->value.v.id = v->v.id;
          squ_var_def(ctx,v->v.id,NULL);
        }
      }
      if(nlambda->body != NULL)
      {
        lambda->body = (node*)nlambda->body;
        if(nlambda->args_value != NULL)
        {
          lambda->args_value = (node*)nlambda->args_value;
          squ_value* v = node_expr(ctx,nlambda->args_value);
          squ_var_reset(ctx, lambda->args->value.v.id,v);
          node_expr_stmt(ctx, nlambda->body);
        }
      }
      if(nlambda->ret != NULL)
      {
        return &nlambda->ret->value;
      }
    }
    break;

  case NODE_FDEF:
    {
      lambda = malloc(sizeof(squ_lambda));
      node_fdef* nfdef = np->value.v.p;
      if(nfdef->args != NULL)
      {
        lambda->args = (node*)nfdef->args;
        node_array* arr0 = nfdef->args->value.v.p;
        squ_array* arr1 = squ_array_new();
        int i;
        for(i = 0; i < arr0->len; i++)
        {
          squ_array_add(arr1, node_expr(ctx, arr0->data[i]));
          squ_value* v = node_expr(ctx, arr0->data[i]);
          lambda->args->value.v.id = v->v.id;
          squ_var_def(ctx,v->v.id,NULL);
        }
      }
      if(nfdef->blk != NULL)
      {
        lambda->body = (node*)nfdef->blk;
        int r;
        khiter_t k;
        static squ_value v;
        k = kh_put(value,ctx->env,nfdef->ident->value.v.id,&r);
        v.t = SQU_VALUE_USER;
        v.v.p = NULL;
        kh_value(ctx->env,k) = &v;
      }
      if(nfdef->ret != NULL)
      {
        lambda->ret = (node*)nfdef->ret;
      }
    }
    break;
  
  case NODE_PRINT:
    {
      node_print* nprint = np->value.v.p;
      push(node_expr(ctx, nprint->args));
      print_item();
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
      if(v_r->t == SQU_VALUE_IDENT)
        squ_var_def(ctx,v_l->v.id, var_get(ctx,v_r->v.id));
      else
        squ_var_def(ctx,v_l->v.id,v_r);
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
  /* No return value, so we set "NULL" */
  squ_value* ret = NULL;
  int i;
  for (i = 0; i < args->len; i++) 
  {
    squ_value* v = args->data[i];
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
          squ_value* v1 = var_get(ctx,v->v.id);
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
  return ret;
}

squ_value*
squ_puts(squ_ctx* ctx, squ_array* args) {
  return squ_cputs(ctx, stdout, args);
}

static void
squ_output_init(parser_state* p)
{
  squ_var_def(&p->ctx, "println", squ_cfunc_value(squ_puts));
}

squ_value*
squ_input(squ_ctx* ctx, squ_array* args) {
  static squ_value ret;
  if(args->len != 0)
  {
    squ_value* v = args->data[0];
    puts(v->v.s);
  }
  ret.t = SQU_VALUE_STRING;
  scanf("%s\n", &(ret.v.s));
  puts(ret.v.s);
  return &ret;
}

static void
squ_input_init(parser_state* p)
{
  squ_var_def(&p->ctx, "input", squ_cfunc_value(squ_input));
}

int
squ_run(parser_state* p)
{
  squ_output_init(p);
  squ_input_init(p);
  squ_math_init(p);
  node_expr_stmt(&p->ctx, (node*)p->lval);
  
  if(lambda != NULL)
  {
    free(lambda->args);
    free(lambda->body);
    free(lambda->ret);
    free(lambda);
  }

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