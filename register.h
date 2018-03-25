#ifndef REGISTER_INCLUDED
#define REGISTER_INCLUDED

#include <stdint.h>

#ifndef REGISTERINFO_INCLUDED
#define REGISTERINFO_INCLUDED
#include "RegisterInfo.h"
#endif

#define NUM_REGS 32

typedef struct {
    uint32_t general[NUM_REGS];
    uint32_t pc;
} Register_T;

void generalRegWrite(Register_T regs, int num, uint32_t value);
uint32_t generalRegRead(Register_T regs, int num);
void convertToRegInfo(Register_T regs, RegisterInfo *reg);
void initReg(Register_T regs);

void pcRegWrite(Register_T regs, uint32_t npc);
uint32_t pcRegRead(Register_T regs);
void pcIncrementFour(Register_T regs);

#endif
