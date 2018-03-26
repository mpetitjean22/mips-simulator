/*--------------------------------------------------------------------*/
/* instr.h                                                            */
/* Author: S L O T H                                                  */
/*--------------------------------------------------------------------*/
#include <stdint.h>
#include <cstdio>
#include <cstdlib>
#include "register.h"
#include "MemoryStore.h"
#include "decode.h"

extern Register_T regs;
extern MemoryStore *mem;

/* R TYPE INSTRUCTIONS */

/* AHHHH WHAT ARE WE DOING ABOUT PC??? */

static void instrADD(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    /* QUEST FOR LOCHNESS: DO WE CHECK ANYWHERE TO MAKE SURE THE
       REGISTER NUM IS LESS THAN 32??? */

    int32_t sum, op1, op2;
    op1 = (int32_t)generalRegRead(regs, rs);
    op2 = (int32_t)generalRegRead(regs, rt);
    sum = op1 + op2;

    generalRegWrite(regs, rd, (uint32_t)sum);
    pcIncrementFour(regs);

    /* check for overflow */
    if(((op1>0) && (op2>0) && (sum<0)) ||
        ((op2<0) && (op1<0) && (sum>=0))){
        pcRegWrite(regs, (uint32_t)0x8000);
    }
}

static void instrADDU(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    uint32_t sum;
    sum = generalRegRead(regs, rs) + generalRegRead(regs, rt);
    generalRegWrite(regs, rd, sum);
    pcIncrementFour(regs);
}

static void instrAND(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    uint32_t andVal;
    andVal = generalRegRead(regs, rs) & generalRegRead(regs, rt);
    generalRegWrite(regs, rd, andVal);
    pcIncrementFour(regs);
}

static void instrNOR(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    uint32_t result;
    result = ~(generalRegRead(regs, rs) | generalRegRead(regs, rt));
    generalRegWrite(regs, rd, result);
    pcIncrementFour(regs);
}

static void instrOR(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    uint32_t result;
    result = (generalRegRead(regs, rs) | generalRegRead(regs, rt));
    generalRegWrite(regs, rd, result);
    pcIncrementFour(regs);
}

static void instrSLT(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    if ((int32_t)generalRegRead(regs, rs) < (int32_t)generalRegRead(regs, rt)) {
        generalRegWrite(regs, rd, (uint32_t)1);
    } else {
        generalRegWrite(regs, rd, (uint32_t)0);
    }
    pcIncrementFour(regs);
}

static void instrSLTU(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    if (generalRegRead(regs, rs) < generalRegRead(regs, rt)) {
        generalRegWrite(regs, rd, (uint32_t)1);
    } else {
        generalRegWrite(regs, rd, (uint32_t)0);
    }
    pcIncrementFour(regs);
}

/* SHAMT SHOULD BE UNISGNED???? */
static void instrSLL(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    uint32_t result;
    result = generalRegRead(regs, rt) << shamt;
    generalRegWrite(regs, rd, result);
    pcIncrementFour(regs);
}

static void instrSRL(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    uint32_t result;
    result = generalRegRead(regs, rt) >> shamt;
    generalRegWrite(regs, rd, result);
    pcIncrementFour(regs);
}

static void instrSUB(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    int32_t sum, op1, op2;
    op1 = (int32_t)generalRegRead(regs, rs);
    op2 = (int32_t)generalRegRead(regs, rt);
    sum =  op1 - op2;

    generalRegWrite(regs, rd, (uint32_t)sum);
    pcIncrementFour(regs);

    /* checks for overflow */
    if(((op1>=0) && (op2<0) && (sum<0)) ||
        ((op2<0) && (op1>=0) && (sum>=0))){
        pcRegWrite(regs, (uint32_t)0x8000);
    }
}

static void instrSUBU(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    uint32_t sum;
    sum = generalRegRead(regs, rs) - generalRegRead(regs, rt);
    generalRegWrite(regs, rd, sum);
    pcIncrementFour(regs);
}

static void instrJR(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    /* should we check if its word aligned? */
    pcRegWrite(regs, generalRegRead(regs, rs));
}
/*--------------------------------------------------------------------*/

/* J TYPE INSTRUCTIONS*/
static void instrJ(uint32_t instr)
{
    uint32_t address;
    uint32_t oldPC, npc;
    jTypeDecode(instr, &address);
    oldPC = pcRegRead(regs);
    npc = ((oldPC+4) & 0xf0000000) | (address<<2);
    pcRegWrite(regs, npc);
}

static void instrJAL(uint32_t instr)
{
    uint32_t address;
    uint32_t oldPC, npc;
    jTypeDecode(instr, &address);
    oldPC = pcRegRead(regs);
    npc = ((oldPC+4) & 0xf0000000) | (address<<2);
    generalRegWrite(regs, (uint32_t)31, (uint32_t)oldPC+4);
    pcRegWrite(regs, npc);
}

/*--------------------------------------------------------------------*/

/* I TYPE INSTRUCTIONS */

/* signed extends the immediate to 32 bits */
static uint32_t somethingToSignExtend(uint16_t imm){
    /* ooofff i hate bits... double check this */
    uint32_t newMasked;
    /* signed */
    if (((1 << 15) & imm) >> 15) {
        newMasked = (uint32_t)(0xFFFF0000 | imm);
    } else { /* unsigned */
        newMasked = (uint32_t)(0x00000000 | imm);
    }
    return newMasked;
}

static void instrADDI(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    int32_t result;

    iTypeDecode(instr, &rs, &rt, &imm);
    result = (int32_t)generalRegRead(regs,rs) + (int32_t)somethingToSignExtend(imm);
    generalRegWrite(regs, rt, (uint32_t)result);
    pcIncrementFour(regs);
}

static void instrADDIU(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t result;

    fprintf(stderr, "%s\n", "lmao");

    iTypeDecode(instr, &rs, &rt, &imm);
    result = generalRegRead(regs,rs) + somethingToSignExtend(imm);
    generalRegWrite(regs, rt, result);
    pcIncrementFour(regs);
}

static void instrANDI(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t result;

    iTypeDecode(instr, &rs, &rt, &imm);
    result = (uint32_t)(generalRegRead(regs, rs) & imm);
    generalRegWrite(regs, rt, result);
    pcIncrementFour(regs);
}

static void instrBEQ(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t oldPC, npc;

    oldPC = pcRegRead(regs);
    iTypeDecode(instr, &rs, &rt, &imm);
    if (generalRegRead(regs, rs) == generalRegRead(regs, rt)) {
        npc = oldPC + 4 + (uint32_t)(somethingToSignExtend(imm) << 2);
        pcRegWrite(regs, npc);
    } else{
        pcIncrementFour(regs);
    }

}

static void instrBNE(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t oldPC, npc;

    oldPC = pcRegRead(regs);
    iTypeDecode(instr, &rs, &rt, &imm);
    if (generalRegRead(regs, rs) != generalRegRead(regs, rt)) {
        npc = oldPC + 4 + (uint32_t)(somethingToSignExtend(imm) << 2);
        pcRegWrite(regs, npc);
    } else{
        pcIncrementFour(regs);
    }

}

static void instrLBU(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t memAddress, result;

    iTypeDecode(instr, &rs, &rt, &imm);
    memAddress = (uint32_t)(generalRegRead(regs, rs) + somethingToSignExtend(imm));
    mem->getMemValue(memAddress, result, BYTE_SIZE);
    generalRegWrite(regs, rt, result);
    pcIncrementFour(regs);
}

static void instrLHU(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t memAddress, result;

    iTypeDecode(instr, &rs, &rt, &imm);
    memAddress = (uint32_t)(generalRegRead(regs, rs) + somethingToSignExtend(imm));
    mem->getMemValue(memAddress, result, HALF_SIZE);
    generalRegWrite(regs, rt, result);
    pcIncrementFour(regs);
}

static void instrLUI(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t result;

    iTypeDecode(instr, &rs, &rt, &imm);
    result = (uint32_t)(imm << 16);
    generalRegWrite(regs, rt, result);
    pcIncrementFour(regs);
}

static void instrLW(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t memAddress, result;

    iTypeDecode(instr, &rs, &rt, &imm);
    memAddress = (uint32_t)(generalRegRead(regs, rs) + somethingToSignExtend(imm));
    mem->getMemValue(memAddress, result, WORD_SIZE);
    generalRegWrite(regs, rt, result);
    pcIncrementFour(regs);
}

static void instrORI(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t result;

    iTypeDecode(instr, &rs, &rt, &imm);
    result = (uint32_t)(generalRegRead(regs, rs) | imm);
    generalRegWrite(regs, rt, result);
    pcIncrementFour(regs);
}

static void instrSLTI(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;

    iTypeDecode(instr, &rs, &rt, &imm);
    if ((int32_t)generalRegRead(regs, rs) < (int32_t)somethingToSignExtend(imm)) {
        generalRegWrite(regs, rt, (uint32_t)1);
    } else {
        generalRegWrite(regs, rt, (uint32_t)0);
    }
    pcIncrementFour(regs);
}

static void instrSLTIU(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;

    iTypeDecode(instr, &rs, &rt, &imm);
    if (generalRegRead(regs, rs) < somethingToSignExtend(imm)) {
        generalRegWrite(regs, rt, (uint32_t)1);
    } else {
        generalRegWrite(regs, rt, (uint32_t)0);
    }
    pcIncrementFour(regs);
}

static void instrSB(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t memAddress, val;

    iTypeDecode(instr, &rs, &rt, &imm);
    memAddress = generalRegRead(regs, rs) + somethingToSignExtend(imm);
    val = (uint32_t)(generalRegRead(regs, rt) & 0xff);
    mem->setMemValue(memAddress, val, BYTE_SIZE);
    pcIncrementFour(regs);
}

static void instrSH(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t memAddress, val;

    iTypeDecode(instr, &rs, &rt, &imm);
    memAddress = generalRegRead(regs, rs) + somethingToSignExtend(imm);
    val = (uint32_t)(generalRegRead(regs, rt) & 0xffff);
    mem->setMemValue(memAddress, val, HALF_SIZE);
    pcIncrementFour(regs);
}

static void instrSW(uint32_t instr)
{
    int rs, rt;
    uint16_t imm;
    uint32_t memAddress;

    iTypeDecode(instr, &rs, &rt, &imm);
    memAddress = generalRegRead(regs, rs) + somethingToSignExtend(imm);
    mem->setMemValue(memAddress, generalRegRead(regs, rt), WORD_SIZE);
    pcIncrementFour(regs);
}


static void arithERROR(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    fprintf(stderr, "Incorrect funct encountered with instruction: %8x\n",
            instr);
    exit(EXIT_FAILURE);
}

static void (*arithDecodeTable[64])(uint32_t, int, int, int, int) = {
    instrSLL,   arithERROR, instrSRL,   arithERROR,  // 0x00-0x03
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x04-0x07
    instrJR,    arithERROR, arithERROR, arithERROR,  // 0x08-0x0B
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x0C-0x0F
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x10-0x13
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x14-0x17
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x18-0x1B
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x1C-0x1F
    instrADD,   instrADDU,  instrSUB,   instrSUBU,   // 0x20-0x23
    instrAND,   instrOR,    arithERROR, instrNOR,    // 0x24-0x27
    arithERROR, arithERROR, instrSLT,   instrSLTU,   // 0x28-0x2B
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x2C-0x2F
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x30-0x33
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x34-0x37
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x38-0x3B
    arithERROR, arithERROR, arithERROR, arithERROR,  // 0x3C-0x3F
};

static void arithDecode(uint32_t instr)
{
    int rs, rt, rd, shamt, funct;

    rTypeDecode(instr, &rs, &rt, &rd, &shamt, &funct);
    arithDecodeTable[funct](instr, rs, rt, rd, shamt);
}

static void rootERROR(uint32_t instr)
{
    fprintf(stderr, "Incorrect opcode encountered with instruction: %8x\n",
            instr);
    exit(EXIT_FAILURE);
}


static void (*rootDecodeTable[64])(uint32_t) = {
    arithDecode, rootERROR,  instrJ,    instrJAL,    // 0x00-0x03
    instrBEQ,    instrBNE,   rootERROR, rootERROR,   // 0x04-0x07
    instrADDI,   instrADDIU, instrSLTI, instrSLTIU,  // 0x08-0x0B
    instrANDI,   instrORI,   rootERROR, instrLUI,    // 0x0C-0x0F
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x10-0x13
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x14-0x17
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x18-0x1B
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x1C-0x1F
    rootERROR,   rootERROR,  rootERROR, instrLW,     // 0x20-0x23
    instrLBU,    instrLHU,   rootERROR, rootERROR,   // 0x24-0x27
    instrSB,     instrSH,    rootERROR, instrSW,     // 0x28-0x2B
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x2C-0x2F
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x30-0x33
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x34-0x37
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x38-0x3B
    rootERROR,   rootERROR,  rootERROR, rootERROR,   // 0x3C-0x3F
};

static void rootDecode(uint32_t instr)
{
    if (instr == (uint32_t) 0xfeedfeed) {
        RegisterInfo reg;

        convertToRegInfo(regs, &reg);
        dumpRegisterState(reg);
        dumpMemoryState(mem);
        exit(EXIT_SUCCESS);
    }
    rootDecodeTable[(instr & OPCODE_MASK) >> OPCODE_SHIFT](instr);
}

void execCurrentInstr(void)
{
    uint32_t instr;
    mem->getMemValue(pcRegRead(regs), instr, WORD_SIZE);
    fprintf(stderr, "%x %x\n", instr, pcRegRead(regs));
    rootDecode(instr);
}
