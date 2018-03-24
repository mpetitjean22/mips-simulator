#include <cstdio>
#include <cstdlib>
#include "MemoryStore.h"
#include "RegisterInfo.h"
#include "EndianHelpers.h"

using namespace std;

enum {OPCODE_MASK = 0xfc000000, OPCODE_SHIFT = 26,
      RS_MASK = 0x3e00000, RS_SHIFT = 21,
      RT_MASK = 0x1f0000, RT_SHIFT = 16,
      RD_MASK = 0xf800, RD_SHIFT = 11,
      SHAMT_MASK = 0x7c0, SHAMT_SHIFT = 6,
      FUNCT_MASK = 0x3f, FUNCT_SHIFT = 0,
      IMM_MASK = 0xffff, IMM_SHIFT = 0,
      ADDR_MASK = 0x3ffffff,
      J_PC_MASK = 0xf0000000};

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
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x00-0x03
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x04-0x07
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x08-0x11
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x12-0x15
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x16-0x19
    instrADD,   arithERROR, arithERROR, arithERROR, // 0x20-0x23
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x24-0x27
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x28-0x31
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x32-0x35
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x36-0x39
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x40-0x43
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x44-0x47
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x48-0x51
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x52-0x55
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x56-0x59
    arithERROR, arithERROR, arithERROR, arithERROR, // 0x60-0x63
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
    arithDecode, rootERROR, rootERROR, rootERROR, // 0x00-0x03
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x04-0x07
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x08-0x11
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x12-0x15
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x16-0x19
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x20-0x23
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x24-0x27
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x28-0x31
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x32-0x35
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x36-0x39
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x40-0x43
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x44-0x47
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x48-0x51
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x52-0x55
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x56-0x59
    rootERROR,   rootERROR, rootERROR, rootERROR, // 0x60-0x63
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
