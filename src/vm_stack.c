#include "squ_vm.h"
#include "vm_stack.h"

int strref_count = 0;
int globcount = 0;
int gflag = 0;
squ_string strref[S_SIZE];
int sp = 0;
squ_value* stack[MAXSIZE];

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