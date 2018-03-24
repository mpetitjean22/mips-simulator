/*--------------------------------------------------------------------*/
/* instr.cpp                                                          */
/* Author: S L O T H                                                  */
/*--------------------------------------------------------------------*/

/* R types */
void instrADD(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrADDU(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrAND(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrADD(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrNOR(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrOR(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrSLT(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrSLTU(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrSLL(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrSRL(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrSUB(uint32_t instr, int rs, int rt, int rd, int shamt);
void instrSUBU(uint32_t instr, int rs, int rt, int rd, int shamt);

/* J types */
void instrJ(uint32_t instr, uint32_t addr);
void instrJAL(uint32_t instr, uint32_t addr);

/* I types */
void instrADDI(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrADDIU(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrANDI(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrBEQ(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrBNE(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrLBU(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrLHU(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrLUI(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrLW(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrORI(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrSLTI(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrSLTIU(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrSB(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrSH(uint32_t instr, int *rs, int *rt, uint16_t *imm);
void instrSW(uint32_t instr, int *rs, int *rt, uint16_t *imm);
