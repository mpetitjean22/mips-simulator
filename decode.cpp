/*------------------------------------------------*/
/* decode.cpp                                     */
/* Author: s l o t h                              */
/*------------------------------------------------*/
#include "decode.h"

static void rTypeDecode(uint32_t instr, int *rs, int *rt, int *rd, int *shamt,
                        int *funct)
{
  *rs = (instr & RS_MASK) >> RS_SHIFT;
  *rt = (instr & RT_MASK) >> RT_SHIFT;
  *rd = (instr & RD_MASK) >> RD_SHIFT;
  *shamt = (instr & SHAMT_MASK) >> SHAMT_SHIFT;
  *funct = (instr & FUNCT_MASK) >> FUNCT_SHIFT;
}

static void iTypeDecode(uint32_t instr, int *rs, int *rt, uint16_t *imm)
{
  *rs = (instr & RS_MASK) >> RS_SHIFT;
  *rt = (instr & RT_MASK) >> RT_SHIFT;
  *imm = (instr & IMM_MASK) >> IMM_SHIFT;
}

static void jTypeDecode(uint32_t instr, uint32_t *addr)
{
  /* this is commented out because we havent established a method of storing PC                                                                                                              
     yet so this cant really be done cause i want to do JumpAddr conversion here                                                                                                               
     *addr = ((PC+4) & J_PC_MASK) | ((instr & ADDR_MASK) << 2);                                                                                                                                 
     */
}
