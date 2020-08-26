#ifndef SQU_MATH_H
#define SQU_MATH_H

#include "node.h"

squ_value* squ_sin(squ_ctx* ctx, squ_array* args);
squ_value* squ_cos(squ_ctx* ctx, squ_array* args);
squ_value* squ_tan(squ_ctx* ctx, squ_array* args);

squ_value* squ_sinh(squ_ctx* ctx, squ_array* args);
squ_value* squ_cosh(squ_ctx* ctx, squ_array* args);
squ_value* squ_tanh(squ_ctx* ctx, squ_array* args);

squ_value* squ_asin(squ_ctx* ctx, squ_array* args);
squ_value* squ_acos(squ_ctx* ctx, squ_array* args);
squ_value* squ_atan(squ_ctx* ctx, squ_array* args);

squ_value* squ_sqrt(squ_ctx* ctx, squ_array* args);

squ_value* squ_rand(squ_ctx* ctx, squ_array* args);

void squ_math_init(parser_state*);

#endif /* SQU_MATH_H */