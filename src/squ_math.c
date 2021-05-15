#include "squ_math.h"

#include <math.h>
#include <assert.h>
#include <stdlib.h>

#define M_PI 3.141593
#define M_E 2.718282

squ_value*
squ_sin(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = sin(v->v.i);
    return &ret;
}

squ_value*
squ_cos(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = cos(v->v.i);
    return &ret;
}

squ_value*
squ_tan(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = tan(v->v.i);
    return &ret;
}

squ_value*
squ_sinh(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = sinh(v->v.i);
    return &ret;
}

squ_value*
squ_cosh(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = cosh(v->v.i);
    return &ret;
}

squ_value*
squ_tanh(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = tanh(v->v.i);
    return &ret;
}

squ_value*
squ_asin(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = asin(v->v.i);
    return &ret;
}

squ_value*
squ_atan(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = atan(v->v.i);
    return &ret;
}

squ_value*
squ_acos(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = acos(v->v.i);
    return &ret;
}

squ_value*
squ_rand(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 0);
    ret.t = SQU_VALUE_INT;
    srand((unsigned int)time(0));
    ret.v.i = rand();
    return &ret;
}

squ_value*
squ_sqrt(squ_ctx* ctx, squ_array* args)
{
    static squ_value ret;
    assert(args->len == 1);
    squ_value* v = args->data[0];
    ret.t = SQU_VALUE_DOUBLE;
    ret.v.d = sqrt((double)(v->v.i));
    return &ret;
}

void
squ_math_init(parser_state* p)
{
    squ_var_def(&p->ctx, "sin", squ_cfunc_value(squ_sin));
    squ_var_def(&p->ctx, "cos", squ_cfunc_value(squ_cos));
    squ_var_def(&p->ctx, "tan", squ_cfunc_value(squ_tan));

    squ_var_def(&p->ctx, "sinh", squ_cfunc_value(squ_sinh));
    squ_var_def(&p->ctx, "cosh", squ_cfunc_value(squ_cosh));
    squ_var_def(&p->ctx, "tanh", squ_cfunc_value(squ_tanh));

    squ_var_def(&p->ctx, "asin", squ_cfunc_value(squ_asin));
    squ_var_def(&p->ctx, "acos", squ_cfunc_value(squ_acos));
    squ_var_def(&p->ctx, "atan", squ_cfunc_value(squ_atan));

    squ_var_def(&p->ctx, "PI", squ_double_value(M_PI));
    squ_var_def(&p->ctx, "E", squ_double_value(M_E));

    squ_var_def(&p->ctx, "rand", squ_cfunc_value(squ_rand));
    squ_var_def(&p->ctx, "sqrt", squ_cfunc_value(squ_sqrt));
}