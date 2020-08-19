#include <stdlib.h>
#include "square.h"

BOOL
is_squ_int(squ_value* v1, squ_value* v2)
{
    if(v2 == NULL)
    {
        return v1->t == SQU_VALUE_INT;
    }
    return v1->t == SQU_VALUE_INT && v2->t == SQU_VALUE_INT;
}

BOOL
is_squ_string(squ_value* v1, squ_value* v2)
{
    if(v2 == NULL)
    {
        return v1->t == SQU_VALUE_STRING;
    }
    return v1->t == SQU_VALUE_STRING && v2->t == SQU_VALUE_STRING;
}

BOOL
is_squ_bool(squ_value* v1, squ_value* v2)
{
    if(v2 == NULL)
    {
        return v1->t == SQU_VALUE_BOOL;
    }
    return v1->t == SQU_VALUE_BOOL && v2->t == SQU_VALUE_BOOL;
}

BOOL
is_squ_double(squ_value* v1, squ_value* v2)
{
    if(v2 == NULL)
    {
        return v1->t == SQU_VALUE_DOUBLE;
    }
    return v1->t == SQU_VALUE_DOUBLE && v2->t == SQU_VALUE_DOUBLE;
}