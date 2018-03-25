#include "register.h"
#include "assert.h"

void generalRegWrite(Register_T regs, int num, uint32_t value)
{
    assert(0 <= num && num < NUM_REGS);
    if (num != 0)
        regs.general[num] = value;
}

uint32_t generalRegRead(Register_T regs, int num)
{
    assert(0 <= num && num < NUM_REGS);
    if (num == 0)
        return 0;
    return regs.general[num];
}

void pcRegWrite(Register_T regs, uint32_t npc){
      regs.pc = npc;
}

void pcIncrementFour(Register_T regs){
      regs.pc += 4; 
}

uint32_t pcRegRead(Register_T regs){
   return regs.pc;
}

void convertToRegInfo(Register_T regs, RegisterInfo *regInfo)
{
    int i;

    regInfo->at = regs.general[1];

    for (i = 0; i < V_REG_SIZE; i++)
        regInfo->v[i] = regs.general[i+2];

    for (i = 0; i < A_REG_SIZE; i++)
        regInfo->a[i] = regs.general[i+4];

    for (i = 0; i < 8; i++)
        regInfo->t[i] = regs.general[i+8];

    for (i = 0; i < 2; i++)
        regInfo->t[i+8] = regs.general[i+24];

    for (i = 0; i < S_REG_SIZE; i++)
        regInfo->s[i] = regs.general[i+16];

    for (i = 0; i < K_REG_SIZE; i++)
        regInfo->k[i] = regs.general[i+26];

    regInfo->gp = regs.general[28];
    regInfo->sp = regs.general[29];
    regInfo->fp = regs.general[30];
    regInfo->ra = regs.general[31];
}

void initReg(Register_T regs)
{
    int i;

    regs.pc = 0;
    for (i = 0; i < NUM_REGS; i++)
        regs.general[i] = 0;
}
