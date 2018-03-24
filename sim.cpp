#include <cstdio>
#include <cstdlib>
#include "MemoryStore.h"
#include "RegisterInfo.h"
#include "EndianHelpers.h"

using namespace std;

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

    while ( !feof(file)) {
        uint32_t temp;

	fread(&temp, 4, 1, file);
	mem->setMemValue(address, ConvertWordToBigEndian(temp), WORD_SIZE);
	address+=0x4; 
    }



    //dumpRegisterState(reg);
    dumpMemoryState(mem);

    delete mem;
    return EXIT_SUCCESS;
}
