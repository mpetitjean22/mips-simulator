#include <cstdio>
#include <cstdlib>
#include "MemoryStore.h"

#ifndef REGISTERINFO_INCLUDED
#define REGISTERINFO_INCLUDED
#include "RegisterInfo.h"
#endif

#include "EndianHelpers.h"
#include "decode.h"
#include "register.h"

using namespace std;

Register_T regs;

static void instrADD(uint32_t instr, int rs, int rt, int rd, int shamt)
{
    // this isnt done obviously im just showing this as an example
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
    instrADDI,   instrADDIU, instrSLTI, instrSLTU,   // 0x08-0x0B
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
        // RegisterInfo reg;
        // Fill reg with the current contents of the registers
        // dumpRegisterState(reg);
        // dumpMemoryState(myMem);
        // exit(EXIT_SUCCESS);
    }
    rootDecodeTable[(instr & OPCODE_MASK) >> OPCODE_SHIFT](instr);
}

int main(int argc, char *argv[])
{
    MemoryStore *mem;
    RegisterInfo reg;
    FILE *file;
    uint32_t address;

    if (argc != 2) {
        fprintf(stderr, "%s: Incorrect usage. Provide a MIPS file as an argument.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        perror(argv[0]);
    }

    mem = createMemoryStore();
    address = 0x0;

    /* initialize registers */
    int i;
    reg.at = 0;
    reg.gp = 0;
    reg.sp = 0;
    reg.fp = 0;
    reg.ra = 0;

    for(i=0; i<V_REG_SIZE; i++){
        reg.v[i] = 0;
    }

    for(i=0; i<A_REG_SIZE; i++){
        reg.a[i] = 0;
    }

    for(i=0; i<T_REG_SIZE; i++){
        reg.t[i] = 0;
    }

    for(i=0; i<S_REG_SIZE; i++){
        reg.s[i] = 0;
    }

    for(i =0; i<K_REG_SIZE; i++){
        reg.k[i] = 0;
    }

    while ( !feof(file)) {
        uint32_t temp;
        fread(&temp, 4, 1, file);
        mem->setMemValue(address, ConvertWordToBigEndian(temp), WORD_SIZE);
	    address+=0x4;
    }



    dumpRegisterState(reg);
    dumpMemoryState(mem);

    delete mem;
    return EXIT_SUCCESS;
}
