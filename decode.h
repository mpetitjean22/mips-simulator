/*------------------------------------------------*/
/* decode.h                                       */
/* Author: S L O T H                              */
/*------------------------------------------------*/

#ifndef DECODE_INCLUDED
#define DECODE_INCLUDED

#include <stdint.h>

/* constants for masking and shifting bits */
enum {OPCODE_MASK = 0xfc000000, OPCODE_SHIFT = 26,
      RS_MASK = 0x3e00000, RS_SHIFT = 21,
      RT_MASK = 0x1f0000, RT_SHIFT = 16,
      RD_MASK = 0xf800, RD_SHIFT = 11,
      SHAMT_MASK = 0x7c0, SHAMT_SHIFT = 6,
      FUNCT_MASK = 0x3f, FUNCT_SHIFT = 0,
      IMM_MASK = 0xffff, IMM_SHIFT = 0,
      ADDR_MASK = 0x3ffffff,
      J_PC_MASK = 0xf0000000};

/* Decodes r type instructions */
void rTypeDecode(uint32_t instr, int *rs, int *rt, int *rd, int *shamt,
                 int *funct);

/* Decodes i type instructions */
void iTypeDecode(uint32_t instr, int *rs, int *rt, uint16_t *imm);

/* Decoes j type instructions */
void jTypeDecode(uint32_t instr, uint32_t *addr);

#endif
