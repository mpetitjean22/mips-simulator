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
#include "instr.h"

/* std huh ;) */
using namespace std;

Register_T regs;
MemoryStore *mem;

int main(int argc, char *argv[])
{
    FILE *file;
    uint32_t address;
    RegisterInfo reg;


    if (argc != 2) {
        fprintf(stderr, "%s: Incorrect usage. Provide a MIPS file as an argument.\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        perror(argv[0]);
    }

    regs = newReg();
    mem = createMemoryStore();
    address = 0x0;

    while ( !feof(file)) {
        uint32_t temp;
        fread(&temp, 4, 1, file);
        mem->setMemValue(address, ConvertWordToBigEndian(temp), WORD_SIZE);
	    address+=0x4;
    }

    while (1) {
        execCurrentInstr();
    }

    convertToRegInfo(regs, &reg);

    dumpRegisterState(reg);
    dumpMemoryState(mem);

    delete mem;
    return EXIT_SUCCESS;
}
