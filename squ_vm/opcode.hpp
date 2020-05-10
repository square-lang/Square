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
  EQ,
  NEQ,
  PUSH,
  POP,
  PRINT,
  JMP,
  JMPT,      /* Jmp true */
  JMPF,      /* Jmp false */
  LOAD,
  LOADG,
  STORE,
  STOREG,
  CALL,
  RET,
  END
} InstructionSet;

#endif