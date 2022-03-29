#include <stdio.h>
#include <stdlib.h>

#include "type.h"
#include "config.h"
#include "assembler.h"

/* ************************************************************************
 *
 * EXTERNAL VARIABLES
 *
 * ************************************************************************/

extern wanderlust_config e_configuration;

/* ************************************************************************
 *
 * FILE SCOPE VARIABLES
 *
 * ***********************************************************************/

#define OPCODE_SIZE 0x100
#define MEMORY_SIZE 0x10000

static unsigned char f_memory [MEMORY_SIZE];

static u16 f_cursor = 0x8000;

static u16 f_editor = 0x8000;

/* ************************************************************************
 *
 * FILE SCOPE LOOK UP TABLES
 *
 * ***********************************************************************/

static unsigned char f_length [OPCODE_SIZE] = {
  1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
  1, 3, 1, 1, 1, 1, 2, 1, 1, 1, 1, 1, 1, 1, 2, 1,
  1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1,
  1, 3, 3, 1, 1, 1, 2, 1, 1, 1, 3, 1, 1, 1, 2, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
  1, 1, 3, 3, 3, 1, 2, 1, 1, 1, 3, 3, 3, 3, 2, 1,
  1, 1, 3, 2, 3, 1, 2, 1, 1, 1, 3, 2, 3, 3, 2, 1,
  1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1,
  1, 1, 3, 1, 3, 1, 2, 1, 1, 1, 3, 1, 3, 3, 2, 1
};

static unsigned char f_opcode [OPCODE_SIZE] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x00, 0x09, 0x0A, 0x0B, 0x04, 0x05, 0x06, 0x0F,

  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x17,
  0x00, 0x09, 0x0A, 0x0B, 0x04, 0x05, 0x06, 0x1F,

  0x00, 0x01, 0x22, 0x03, 0x04, 0x05, 0x06, 0x27,
  0x00, 0x09, 0x2A, 0x0B, 0x04, 0x05, 0x06, 0x2F,

  0x00, 0x01, 0x32, 0x03, 0x04, 0x05, 0x06, 0x37,
  0x00, 0x09, 0x3A, 0x0B, 0x04, 0x05, 0x06, 0x3F,

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, /* b */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, /* c */

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, /* d */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, /* e */

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, /* h */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, /* l */

  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x76, 0x40, /* m */
  0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, 0x40, /* a */

  0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80, 0x80,
  0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88, 0x88,

  0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90, 0x90,
  0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98, 0x98,

  0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0, 0xA0,
  0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8, 0xA8,

  0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0, 0xB0,
  0xB8, 0xB8, 0xB8, 0xB8, 0xB8, 0xB8, 0xB8, 0xB8,

  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7,
  0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xC7,

  0xD0, 0xC1, 0xD2, 0xD3, 0xD4, 0xC5, 0xD6, 0xC7,
  0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xC7,

  0xE0, 0xC1, 0xE2, 0xE3, 0xE4, 0xC5, 0xE6, 0xC7,
  0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xC7,

  0xF0, 0xC1, 0xF2, 0xF3, 0xF4, 0xC5, 0xF6, 0xC7,
  0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xC7
};

#define REGISTER_SIZE 8
static char f_register [REGISTER_SIZE] = {
  'b', 'c', 'd', 'e', 'h', 'l', 'm', 'a'
};

static unsigned char mem_get_byte (int index)
{
  return f_memory [(f_cursor + index) % MEMORY_SIZE];
}

static unsigned char mem_get_byte_i (unsigned int index)
{
  return f_memory [index % MEMORY_SIZE];
}

#define MNEMONIC_LENGTH 8
static char f_mnemonic [OPCODE_SIZE][MNEMONIC_LENGTH] = {
  "NOP", "lxi" , "stax", "inx" , "inr", "dcr" , "mvi", "rlc",
  "NOP", "dad" , "ldax", "dcx" , "inr", "dcr" , "mvi", "rrc",
  "NOP", "lxi" , "stax", "inx" , "inr", "dcr" , "mvi", "ral",
  "NOP", "dad" , "ldax", "dcx" , "inr", "dcr" , "mvi", "rar",
  "NOP", "lxi" , "shld", "inx" , "inr", "dcr" , "mvi", "daa",
  "NOP", "dad" , "lhld", "dcx" , "inr", "dcr" , "mvi", "cma",
  "NOP", "lxi" , "sta" , "inx" , "inr", "dcr" , "mvi", "stc",
  "NOP", "dad" , "lda" , "dcx" , "inr", "dcr" , "mvi", "cmc",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "mov", "mov",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "mov", "mov",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "mov", "mov",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "mov", "mov",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "mov", "mov",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "mov", "mov",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "hlt", "mov",
  "mov", "mov" , "mov" , "mov" , "mov", "mov" , "mov", "mov",
  "add", "add" , "add" , "add" , "add", "add" , "add", "add",
  "adc", "adc" , "adc" , "adc" , "adc", "adc" , "adc", "adc",
  "sub", "sub" , "sub" , "sub" , "sub", "sub" , "sub", "sub",
  "sbb", "sbb" , "sbb" , "sbb" , "sbb", "sbb" , "sbb", "sbb",
  "ana", "ana" , "ana" , "ana" , "ana", "ana" , "ana", "ana",
  "xra", "xra" , "xra" , "xra" , "xra", "xra" , "xra", "xra",
  "ora", "ora" , "ora" , "ora" , "ora", "ora" , "ora", "ora",
  "cmp", "cmp" , "cmp" , "cmp" , "cmp", "cmp" , "cmp", "cmp",
  "rnz", "pop" , "jnz" , "jmp" , "cnz", "push", "adi", "rst",
  "rz" ,  "ret", "jz"  ,  "jmp", "cz" , "call", "aci", "rst",
  "rnc", "pop" , "jnc" , "out" , "cnc", "push", "sui", "rst",
  "rc" , "ret" , "jc"  , "in"  , "cc" , "call", "sbi", "rst",
  "rpo", "pop" , "jpo" , "xthl", "cpo", "push", "ani", "rst",
  "rpe", "pchl", "jpe" , "xchg", "cpe", "call", "xri", "rst",
  "rp" , "pop" , "jp"  , "di"  , "cp" , "push", "ori", "rst",
  "rm" , "sphl", "jm"  , "ei"  , "cm" , "goto", "cpi", "rst"
};

static void mem_set_byte (unsigned int index, unsigned char byte)
{
  u16 i = (f_cursor + index) % MEMORY_SIZE;

  switch (f_length [f_memory [i]]) {
    case 2:
      f_memory [i    ] = byte;
      f_memory [i + 1] = 0;
      break;
    case 3:
      f_memory [i    ] = byte;
      f_memory [i + 1] = 0;
      f_memory [i + 2] = 0;
      break;
    default:
      f_memory [i    ] = byte;
      break;
  }
}

static void mem_set_byte_increment (unsigned int index, unsigned char byte)
{
  u16 i = (f_cursor + index) % MEMORY_SIZE;
  f_memory [i] = byte;
  f_cursor = i + 1;
}

static void mem_set_byte_i (unsigned int index, unsigned char byte)
{
  f_memory [index % MEMORY_SIZE] = byte;
}

#define F(SUFFIX) MEM_PRINT ## SUFFIX

void F(__) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  printf ("%s\n",
	  f_mnemonic [byte]);
}

void F(R_) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  printf ("%s %c\n",
	  f_mnemonic [byte],
	  f_register [args]);
}

void F(RB) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x08;
  unsigned char arg1 = mem_get_byte (index + 1);

  printf ("%s %c, %2X\n",
    f_mnemonic [base],
    f_register [arg0],
    arg1
  );
}

void F(RR) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x08;
  unsigned char arg1 = args % 0x08;

  printf ("%s %c, %c\n",
	  f_mnemonic [base],
	  f_register [arg0],
	  f_register [arg1]);
}

#define REGISTER_PAIR_SIZE 6
#define REGISTER_PAIR_LENGTH 4
const char f_register_pair [REGISTER_PAIR_SIZE][REGISTER_PAIR_LENGTH] = {
  "BC", "DE", "HL", "SP", "PC", "WZ"
};

void F(P_) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x10;

  printf ("%s %s\n",
	  f_mnemonic [base],
	  f_register_pair [arg0]);
}

void F(PW) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x10;
  unsigned char arg1 = mem_get_byte (index + 1);
  unsigned char arg2 = mem_get_byte (index + 2);

  printf ("%s %s, %02x%02x\n",
	  f_mnemonic [base],
	  f_register_pair [arg0],
	  arg1,
	  arg2);
}

#define CONSTANTS_COUNT  8
static char f_constant [CONSTANTS_COUNT] = {
  '0', '1', '2', '3', '4', '5', '6', '7'
};

void F(C_) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x08;

  printf ("%s %c\n",
	  f_mnemonic [base],
	  f_constant [arg0]);  
}

void F(B_) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char arg0 = mem_get_byte (index + 1);

  printf ("%s %02X\n",
	  f_mnemonic [base],
	  arg0);
}

void F(W_) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char arg0 = mem_get_byte (index + 1);
  unsigned char arg1 = mem_get_byte (index + 2);

  printf (
    "%04X %s %02X %02X",
    index,
    f_mnemonic [base],
    arg0,
    arg1
  );
}

void F(_R) (u16 index)
{
  unsigned char byte = mem_get_byte (index);
  unsigned char base = f_opcode [byte];
  unsigned char arg0 = (byte - base) / 8;

  printf ("%s %c\n",
	  f_mnemonic [base],
	  f_register [arg0]
	  );  
}

void F(MC) (u16 index)
{

}

static void (*f_print [])(u16 index) = {
  F(__), F(R_), F(RB), F(RR), F(P_), 
  F(PW), F(C_), F(B_), F(W_), F(_R), F(MC)
};

static unsigned char f_arguments [OPCODE_SIZE] = {
  N(__), N(PW), N(P_), N(P_), N(_R), N(_R), N(RB), N(__),
  N(__), N(P_), N(P_), N(P_), N(_R), N(_R), N(RB), N(__),  
  N(__), N(PW), N(P_), N(P_), N(_R), N(_R), N(RB), N(__),
  N(__), N(P_), N(P_), N(P_), N(_R), N(_R), N(RB), N(__),
  N(__), N(PW), N(W_), N(P_), N(_R), N(_R), N(RB), N(__),
  N(__), N(P_), N(W_), N(P_), N(_R), N(_R), N(RB), N(__),
  N(__), N(PW), N(W_), N(P_), N(_R), N(_R), N(RB), N(__),
  N(__), N(P_), N(W_), N(P_), N(_R), N(_R), N(RB), N(__),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(__), N(RR),
  N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR), N(RR),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_), N(R_),
  N(__), N(P_), N(W_), N(W_), N(W_), N(P_), N(B_), N(C_),
  N(__), N(__), N(W_), N(W_), N(W_), N(W_), N(B_), N(C_),
  N(__), N(P_), N(W_), N(B_), N(W_), N(P_), N(B_), N(C_),
  N(__), N(__), N(W_), N(B_), N(W_), N(W_), N(B_), N(C_),
  N(__), N(P_), N(W_), N(__), N(W_), N(P_), N(B_), N(C_),
  N(__), N(__), N(W_), N(__), N(W_), N(W_), N(B_), N(C_),
  N(__), N(P_), N(W_), N(__), N(W_), N(P_), N(B_), N(C_),
  N(__), N(__), N(W_), N(__), N(W_), N(MC), N(B_), N(C_),
};

static void mem_print (unsigned int index)
{
  f_print [f_arguments [mem_get_byte (index)]] (index);
}

u08 assembler_get_type (u08 code)
{
  return f_arguments [code];
}

u08 assembler_get_byte (u16 index)
{
  return mem_get_byte (index);
}

nil assembler_set_byte (u16 index, u08 byte)
{
  mem_set_byte (index, byte);
}

nil assembler_set_byte_increment (u16 index, u08 byte)
{
  mem_set_byte_increment (index, byte);
}

nil assembler_continue (u16 index)
{
  f_cursor += f_length [mem_get_byte_i (f_cursor)];
  f_editor += 1;

  if (0) {
    mem_print (f_cursor);
  }
}

#define W(SUFFIX) WL_ ## SUFFIX

#include <stdio.h>

static void W (__) (u16 index, char* buffer)
{
  sprintf (
    buffer,
    "%04X %s",
    index,
    f_mnemonic [mem_get_byte_i (index)]
  );
}

static void W (R_) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  
  sprintf (
    buffer,
    "%04X %s %c",
    index,
    f_mnemonic [byte],
    f_register [args]
  );
}

static void W (RB) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x08;
  unsigned char arg1 = mem_get_byte_i (index + 1);

  sprintf (
    buffer,
    "%04X %s %c, %2X",
    index,
    f_mnemonic [base],
    f_register [arg0],
    arg1
  );
}

static void W (RR) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x08;
  unsigned char arg1 = args % 0x08;

  sprintf (
    buffer,
    "%04X %s %c, %c",
    index,
    f_mnemonic [base],
    f_register [arg0],
    f_register [arg1]
  );
}

static void W (P_) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x10;

  sprintf (
    buffer,
    "%04X %s %s",
    index,
    f_mnemonic [base],
    f_register_pair [arg0]
  );
}

static void W (PW) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x10;
  unsigned char arg1 = mem_get_byte_i (index + 1);
  unsigned char arg2 = mem_get_byte_i (index + 2);

  if (0) {
    mem_set_byte_i (index, 0);
  }

  sprintf (
    buffer,
    "%04X %s %s, %02x %02x",
    index,
    f_mnemonic [base],
    f_register_pair [arg0],
    arg1,
    arg2
  );
}

static void W(C_) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = byte - base;
  unsigned char arg0 = args / 0x08;

  sprintf (
    buffer,
    "%04X %s %c",
    index,
    f_mnemonic [base],
    f_constant [arg0]
  );
}

static void W(B_) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char arg0 = mem_get_byte_i (index + 1);

  sprintf (
    buffer,
    "%04X %s %02X",
    index,
    f_mnemonic [base],
    arg0
  );
}

static void W(W_) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char arg0 = mem_get_byte_i (index + 1);
  unsigned char arg1 = mem_get_byte_i (index + 2);

  sprintf (
    buffer,
    "%04X %s %02X %02X",
    index,
    f_mnemonic [base],
    arg0,
    arg1
  );
}

static void W(_R) (u16 index, char* buffer)
{
  unsigned char byte = mem_get_byte_i (index);
  unsigned char base = f_opcode [byte];
  unsigned char args = (byte - base) / 8;
  
  sprintf (
    buffer,
    "%04X %s %c",
    index,
    f_mnemonic [byte],
    f_register [args]
  );
}

static void W(MC) (u16 index, char* buffer)
{

}

static void (* f_writeline []) (u16, char*) = {
  W(__), W(R_), W(RB), W(RR), W(P_), 
  W(PW), W(C_), W(B_), W(W_), W(_R), W(MC)
};

#define ASSEMBLER_WRITELINE_MIN_BUFFLEN 0x14

u08 assembler_writeline (u16 index, const u08* buffer, u32 buflen)
{
  u08 byte  = mem_get_byte_i (index);
  u08 i     = 0;

  if (! buffer) {
    return 0;
  }

  if (buflen < ASSEMBLER_WRITELINE_MIN_BUFFLEN) {
    return 0;
  }

  for (i = 0;i < ASSEMBLER_WRITELINE_MIN_BUFFLEN; i++) {
    *((char*)(buffer + i)) = ' ';
  }

  f_writeline [f_arguments [byte]] (index, (char*)buffer);

  return f_length [byte];
}

u08 assembler_writeline_data (u16 index, const u08* buffer, u32 buflen)
{
  u08 i = 0;

  if (! buffer) {
    return 0;
  }

  if (buflen < ASSEMBLER_WRITELINE_MIN_BUFFLEN) {
    return 0;
  }

  for (i = 0;i < ASSEMBLER_WRITELINE_MIN_BUFFLEN; i++) {
    *((char*)(buffer + i)) = ' ';
  }

  sprintf (
    (char*)buffer,
    "%04X %02X",
    index,
    f_memory [index]
  );

  return 1;
}

u08 assembler_writeline_offset (u16 index, const u08* buffer, u32 buflen, u16 offset)
{
  u08 byte  = mem_get_byte_i (f_editor + index - offset);
  u08 i     = 0;

  if (! buffer) {
    return 0;
  }

  if (buflen < ASSEMBLER_WRITELINE_MIN_BUFFLEN) {
    return 0;
  }

  for (i = 0;i < ASSEMBLER_WRITELINE_MIN_BUFFLEN; i++) {
    *((char*)(buffer + i)) = ' ';
  }

  f_writeline [f_arguments [byte]] (f_editor + index - offset, (char*)buffer);

  return f_length [byte];
}

u08 assembler_writeline_byte (u16 index, const u08* buffer, u32 buflen, u16 offset)
{
  u08 byte  = mem_get_byte_i (index - offset);
  u08 i     = 0;

  if (! buffer) {
    return 0;
  }

  if (buflen < ASSEMBLER_WRITELINE_MIN_BUFFLEN) {
    return 0;
  }

  for (i = 0;i < ASSEMBLER_WRITELINE_MIN_BUFFLEN; i++) {
    *((char*)(buffer + i)) = ' ';
  }

  sprintf (
    (char*)buffer,
    "%04X %02Xh",
    index - offset,
    byte
  );

  return f_length [byte];
}

s08 assembler_initialize (lua_State * L)
{
  f_cursor = f_editor = 0x8000;

  return e_configuration.success;
}

nil assembler_terminate ()
{
  f_cursor = f_editor = 0x8000;
}

u16 assembler_get_cursor ()
{
  return f_cursor;
}

nil assembler_set_cursor (u16 cursor)
{
  u08 byte = mem_get_byte_i (cursor - 3);

  if (f_length [byte] == 3) 
  {
    f_cursor = cursor;
  } 
  else 
  {
    byte = mem_get_byte_i (cursor - 2);

    if (f_length [byte] == 2) 
    {
      f_cursor = cursor;
    } 
    else if (f_length [byte] == 3) 
    {
      f_cursor = cursor + 1;
    } 
    else 
    {
      byte = mem_get_byte_i (cursor - 1);
    
      if (f_length [byte] == 3) 
      {
        f_cursor = cursor + 2;
      } 
      else if (f_length [byte] == 2) 
      {
        f_cursor = cursor + 1;
      } 
      else 
      {
        f_cursor = cursor;
      }
    }
  }
}

nil assembler_inc_cursor (u16 offset)
{
  f_cursor += offset;
}

s32 assembler_inr_cursor (lua_State * L)
{
   f_cursor += f_length [mem_get_byte_i (f_cursor)];

   return e_configuration.success;
}

nil assembler_dec_cursor (u16 offset)
{
  f_cursor += f_length [mem_get_byte (f_cursor)];
}

s32 assembler_dcr_cursor (lua_State * L)
{
  u08 length = 3;
  u08 byte = mem_get_byte_i (f_cursor - length);

  if (f_length [byte] == length) {
    f_cursor -= 3;
  } else {
    length = 2;
    byte = mem_get_byte_i (f_cursor - length);
    if (f_length [byte] == length) {
      f_cursor -= 2;
    } else {
      f_cursor -= 1;
    }
  }

  return e_configuration.success; 
}

u16 assembler_get_next_index (unsigned index)
{
  return f_cursor + f_length [mem_get_byte_i (index)];
}

u16 assembler_get_prev_index (unsigned index)
{
  u08 len = 3;
  u08 val = mem_get_byte_i (index + len);
  u16 adr = 0;

  if (f_length [val] == len) {
    adr = f_cursor - 3;
  } else {
    len = 2;
    val = mem_get_byte_i (index + len);
    if (f_length [val] == 2) {
      adr = f_cursor - 2;
    } else {
      adr = f_cursor - 1;
    }
  }

  return adr;
}

#include "core.h"

s32 assembler_program_run (lua_State* L)
{
  s32 status = 0;

  core_load (f_memory, MEMORY_SIZE);

  status = core_execute ();

  core_save (f_memory, MEMORY_SIZE);

  return status;
}

#define ASSEMBLER_LOAD_ROM_ARG_COUNT    1
#define ASSEMBLER_LOAD_ROM_ARG_FILENAME 1

s32 assembler_load_rom (lua_State* L)
{
  FILE * f = 0;

  if (lua_gettop (L) < ASSEMBLER_LOAD_ROM_ARG_COUNT)
  {
    goto FAILURE;
  }
  
  if (! lua_isstring (L, ASSEMBLER_LOAD_ROM_ARG_FILENAME))
  {
    goto FAILURE;
  }

  if (! (f = fopen (lua_tostring (L, ASSEMBLER_LOAD_ROM_ARG_FILENAME), "r"))) {
    goto FAILURE;
  }

  fread (
    f_memory,
    sizeof (u08),
    MEMORY_SIZE,
    f
  );

  if (f) 
  {
    fclose (f);
  }

  return e_configuration.success;

  FAILURE:

    if (f) 
    {
      fclose (f);
    }

    return e_configuration.failure;
}