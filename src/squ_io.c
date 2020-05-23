#include "square.h"
#include "node.h"

squ_value*
squ_exit(squ_ctx* ctx, squ_int args)
{
  exit(args);
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