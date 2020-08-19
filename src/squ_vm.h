#ifndef _VM_H_
#define _VM_H_

#include "opcode.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <memory.h>

void pop_top();
void binary_add();
void binary_sub();
void binary_mult();
void binary_div();
void binary_mod();
void print_newline();
void print_item();
void squ_end();

#endif /* _VM_H_ */