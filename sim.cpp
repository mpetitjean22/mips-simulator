#include <cstdio>
#include <cstdlib>
#include "MemoryStore.h"
#include "RegisterInfo.h"

using namespace std;

int main(int argc, char *argv[])
{
    MemoryStore *mem;
    RegisterInfo reg;
    FILE *file;

    if (argc != 2) {
        fprintf(stderr, "%s: Incorrect usage. Provide a MIPS file as an argument.", argv[0]);
        exit(EXIT_FAILURE);
    }

    if ((file = fopen(argv[1], "r")) == NULL) {
        perror(argv[0]);
    }
    while ( !feof(file)) {
        uint32_t temp;

    }
    mem = createMemoryStore();






    dumpRegisterState(reg);
    dumpMemoryState(mem);

    delete mem;
    return EXIT_SUCCESS;
}
