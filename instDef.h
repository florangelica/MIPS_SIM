#ifndef INSTRDEF_H_
#define INSTRDEF_H_

// Purpose: defines the opcodes (or function codes) for required instructions

// R TYPE INSTRUCTIONS
// Opcode = 0
// function code

#define ADD     0x20 //0b100000
#define ADDU    0x21 //0b100001
#define AND     0x24 //0b100100
#define JR      0x08 //0b001000
#define MOVN    0x0a //0b001010
#define MOVZ    0x0b //0b001011
#define NOR     0x27 //0b100111
#define OR      0x25 //0b100101
#define SLL     0x00 //0b000000
#define SRL     0x02 //0b000010
#define SLT     0x2a //0b101010
#define SLTU    0x2b //0b101011
#define SUB     0x22 //0b100010
#define SUBU    0x23 //0b100011
#define XOR     0x26 //0b100110

// I TYPE INSTRUCTIONS
// Opcodeds:

#define ADDI    0x08 //0b001000
#define ADDIU   0x09 //0b001001
#define ANDI    0x0c //0b001100
#define BEQ     0x04 //0b000100
#define BGTZ    0x07 //0b000111
#define BLEZ    0x06 //0b000110
#define BLTZ    0x01 //0b000001
#define BNE     0x05 //0b000101
#define LB      0x20 //0b100000
#define LBU     0x24 //0b100100
#define LHU     0x25 //0b100101
#define LUI     0x07 //0b000111
#define LW      0x23 //0b100011
#define ORI     0x0d //0b001101
#define SB      0x28 //0b101000
#define SH      0x29 //0b101001
#define SLTI    0x0a //0b001010
#define SLTIU   0x0b //0b001011
#define SW      0x2b //0b101011
#define XORI    0x0e //0b001110

// J TYPE INSTRUCTIONS
// OPCODE for J and JAL
// * note JR is interpreted as an R TYPE
#define J       0x02 //0b000010
#define JAL     0x03 //0b000011
#endif
