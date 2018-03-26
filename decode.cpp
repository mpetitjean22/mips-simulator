/*------------------------------------------------*/
/* decode.cpp                                     */
/* Author: s l o t h                              */
/*------------------------------------------------*/
#include "decode.h"
#include "register.h"

extern Register_T regs;

void rTypeDecode(uint32_t instr, int *rs, int *rt, int *rd, int *shamt,
                 int *funct)
{
    *rs = (instr & RS_MASK) >> RS_SHIFT;
    *rt = (instr & RT_MASK) >> RT_SHIFT;
    *rd = (instr & RD_MASK) >> RD_SHIFT;
    *shamt = (instr & SHAMT_MASK) >> SHAMT_SHIFT;
    *funct = (instr & FUNCT_MASK) >> FUNCT_SHIFT;
}

void iTypeDecode(uint32_t instr, int *rs, int *rt, uint16_t *imm)
{
    *rs = (instr & RS_MASK) >> RS_SHIFT;
    *rt = (instr & RT_MASK) >> RT_SHIFT;
    *imm = (instr & IMM_MASK) >> IMM_SHIFT;
}

void jTypeDecode(uint32_t instr, uint32_t *addr)
{
    *addr = ((pcRegRead(regs) + 4) & J_PC_MASK) | ((instr & ADDR_MASK) << 2);
}
