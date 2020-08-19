#include "squ_vm.h"
#include "vm_stack.h"

void pop_top()
{
    pop();
}

void binary_add()
{
    squ_value* first = pop();
    squ_value* second = pop();
    squ_value* result;
    result->t = SQU_VALUE_INT;
    result->v.i = first->v.i + second->v.i;
    push(result);
}

void binary_sub()
{
    squ_value* first = pop();
    squ_value* second = pop();
    squ_value* result;
    result->t = SQU_VALUE_INT;
    result->v.i = first->v.i - second->v.i;
    push(result);
}

void binary_mult()
{
    squ_value* first = pop();
    squ_value* second = pop();
    squ_value* result;
    result->t = SQU_VALUE_INT;
    result->v.i = first->v.i * second->v.i;
    push(result);
}

void binary_div()
{
    squ_value* first = pop();
    squ_value* second = pop();
    squ_value* result;
    result->t = SQU_VALUE_INT;
    result->v.i = first->v.i / second->v.i;
    push(result);
}

void print_newline()
{
    printf("\n");
}

void print_item()
{
    squ_value* item = pop();
    switch(item->t)
    {
    case SQU_VALUE_STRING:
        puts(item->v.s);
        break;

    case SQU_VALUE_INT:
        printf("%d", item->v.i);
        break;

    case SQU_VALUE_DOUBLE:
        printf("%f", item->v.d);
        break;
        
    default:
        break;
    }
}

void
squ_end()
{
    freestack(stack);
}