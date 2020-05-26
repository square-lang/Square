#include "square.h"
#include "node.h"

squ_value*
squ_exit(squ_ctx* ctx, squ_array* args)
{
  squ_value* v;
  v = args->data[0];
  if(v -> t == SQU_VALUE_INT)
  {
    exit(v->v.i);
  }
  else
  {
    squ_raise(ctx,"The args must be int");
  }
  return NULL;
}

squ_value*
squ_cinput(squ_ctx* ctx, FILE* out , squ_array* args)
{
  int i;
  squ_string c;
  /* no args */
  if(args->len == 0)   
  {
    gets(c);
  }
  else
  {
    for (i = 0; i < args->len; i++) 
    {
      squ_value* v;
      v = args->data[i];
      if(v != NULL)
      {
        switch(v->t)
        {
        case SQU_VALUE_STRING:
          fprintf(out,"%s",v->v.s);
          gets(c);
          break;
        case SQU_VALUE_IDENT:
          break;
        default:
          break;
        }
      }
    }
  }
  puts(c);
  return NULL;
}

squ_value*
squ_input(squ_ctx* ctx, squ_array* args)
{
  return squ_cinput(ctx,stdout,args);
}

squ_int
squ_open(squ_ctx* ctx, squ_array* args)
{
  squ_string path;
  FILE* fp;
  squ_string mode;
  squ_value* v;
  switch (args->len)
  {
  case 1:
    v = args->data[0];
    if(v != NULL && v->t == SQU_VALUE_STRING)
    {
      path = v->v.s;
      fp = fopen(path,"r");
      if(fp == NULL)
      {
        return -1;
      }
    }
    break;
  case 2:
    v = args->data[0];
    if(v != NULL && v->t == SQU_VALUE_STRING)
    {
      path = v->v.s;
    }
    else
    {
      squ_raise(ctx,"The first args must be string");
      return -1;
    }
    v = args->data[1];
    if(v != NULL && v->t == SQU_VALUE_STRING)
    {
      mode = v->v.s;
      fp = fopen(path,mode);
      if(fp == NULL)
      {
        return -1;
      }
    }
    break;
  default:
    break;
  }
  return 0;
}

squ_int
squ_write(squ_ctx* ctx, squ_array* args)
{
  return 0;
}