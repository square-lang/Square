#ifndef _VM_STACK_H_
#define _VM_STACK_H_

#include "square.h"

#define S_SIZE 100
#define MAXSIZE 1024

squ_value* stack[MAXSIZE];

#define freestack(s)    free(s)

void push(squ_value* obj);
squ_value* pop();

#endif /* _VM_STACK_H_ */