#ifndef _VM_STACK_H_
#define _VM_STACK_H_

#include "square.h"

#define MAXSIZE 256

squ_value* stack[MAXSIZE];

void push(squ_value* obj);
squ_value* pop();

int sp = 0;

void
push(squ_value* obj)
{
  if(sp < MAXSIZE)
  {
    stack[sp++] = obj;
  }
  else
  {
    printf("stack full");
  }
}

squ_value*
pop()
{
  if(sp > 0)
  {
    return stack[--sp];
  }
  else
  {
    printf("stack empty");
    return 0;
  }
}

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

#endif /* _VM_STACK_H_ */