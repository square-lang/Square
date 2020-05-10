/*********************************************************************************

vm.cpp: vm for square

Created at May 9 21:45:47 2020
Author: Stepfen Shawn

Copyright (c) 2020 Stepfen Shanw

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

**************************************************************************************/

#include <iostream>
#include <stdlib.h>
#include "opcode.hpp"
#include "test.h"

#define stackSize 256
#define textSize 256
#define dataSize 256
#define registerSize 32

#ifndef BOOL
#define BOOL int
#endif

#ifndef FALSE
# define FALSE 0
#elif FALSE
# error FALSE must be false
#endif
#ifndef TRUE
# define TRUE 1
#elif !TRUE
# error TRUE must be true
#endif

#define VM_OK 1
#define VM_NG 0

BOOL running = TRUE;
/* Instruction pointer */
int ip = 0;
/* Stack pointer */
int sp = -1;

/* Stack segment */
int stack[stackSize];
/* registers */
int registers[registerSize];
/* Code segment */
int code[textSize];

int 
fetch() {
  return program[ip];
}

void 
eval(int instruction) {
  switch (instruction) {
    case END: 
    {
        running = FALSE;
        break;
    }
    case PUSH:  
    {
        stack[++sp] = program[++ip];
        break;
    }
    case POP: 
    {
        --sp;
        break;
    }
    case ADD: 
    {
	    size_t valueB = stack[sp--];
	    size_t valueA = stack[sp--];
      size_t result = valueA + valueB;
      stack[++sp] = result;
      break;
    }
    case SUB:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	size_t result = valueA - valueB;
    	stack[++sp] = result;
    	break;
    }
    case MUL:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	size_t result = valueA * valueB;
    	stack[++sp] = result;
    	break;
    }
    case DIV:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	size_t result = valueA / valueB;
    	stack[++sp] = result;
    	break;	
    }
    case MOD:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	size_t result = valueA % valueB;
    	stack[++sp] = result;
    	break;
    }
    case LT:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	stack[++sp] = (valueA < valueB) ? TRUE : FALSE;
    	break;
    }
    case LE:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	stack[++sp] = (valueA <= valueB) ? TRUE : FALSE;
    	break;
    }
    case GT:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	stack[++sp] = (valueA > valueB) ? TRUE : FALSE;
    	break;
    }
    case GE:
    {
    	size_t valueB = stack[sp--];
    	size_t valueA = stack[sp--];
    	stack[++sp] = (valueA >= valueB) ? TRUE : FALSE;
    	break;
    }
    case PRINT:
    {
    	std::cout<<stack[sp--]<<std::endl;
    	break;
    }
    case EQ:
    {
      size_t valueB = stack[sp--];
      size_t valueA = stack[sp--];
      stack[++sp] = (valueA == valueB) ? TRUE : FALSE;
      break;
    }
    case NEQ:
    {
      size_t valueB = stack[sp--];
      size_t valueA = stack[sp--];
      stack[++sp] = (valueA != valueB) ? TRUE : FALSE;
      break;
    }
    case JMP:
    {
      ip = code[ip++];
      break;
    }
  }
}

int 
main() {
  while(running)
  {
      eval(fetch());
      ip++;
    }
  return 0;
}