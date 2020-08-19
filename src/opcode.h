#ifndef _OPCODE_H_
#define _OPCODE_H_

#define LOAD_CONST 0x64
#define POP_TOP 0x1
#define BINARY_MULTIPLY 0x14
#define BINARY_DIVIDE 0x1b
#define BINARY_MODULO 0X16
#define BINARY_ADD 0x17
#define BINARY_SUBTRACT 0x18
#define PRINT_ITEM 0x47
#define PRINT_NEWLINE 0x48
#define POP_BLOCK 0x57
#define STORE_NAME 0X5a
#define RETURN_VALUE 0x53

#define LOAD_NAME 0x65
#define COMPARE_OP 0x6b
#define JUMP_FORWARD 0x6e
#define JUMP_ABSOLUTE 0x71
#define POP_JUMP_IF_FALSE 0x72
#define LOAD_GLOBAL 0x74
#define SETUP_LOOP 0x78
#define LOAD_FAST 0x7c
#define STORE_FAST 0x7d
#define CALL_FUNCTION 0x83
#define MAKE_FUNCTION 0x84

#endif /* _OPCODE_H_ */