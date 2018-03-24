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
