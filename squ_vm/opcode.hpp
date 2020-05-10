#ifndef OPCODE_H
#define OPCODE_H

typedef enum {
  ADD,
  SUB,
  MUL,
  DIV,
  MOD,
  LT,
  LE,
  GT,
  GE,
  PUSH,
  POP,
  PRINT,
  END
} InstructionSet;

#endif