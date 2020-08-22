#include "squ_math.h"
#include <math.h>
#include <assert.h>

squ_value*
squ_sin(squ_ctx* ctx, squ_array* args)
{
    squ_value* ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret->t == SQU_VALUE_INT;
    ret->v.i = sin(v->v.i);

    return ret;
}

void
squ_math_init(parser_state* p)
{
    squ_var_def(&p->ctx, "sin", squ_cfunc_value(squ_sin));
}