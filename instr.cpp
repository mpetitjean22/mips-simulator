/*--------------------------------------------------------------------*/
/* instr.h                                                            */
/* Author: S L O T H                                                  */
/*--------------------------------------------------------------------*/

/* R TYPE INSTRUCTIONS */

/* AHHHH WHAT ARE WE DOING ABOUT PC??? */

void instrADD(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   /* QUEST FOR LOCHNESS: DO WE CHECK ANYWHERE TO MAKE SURE THE
      REGISTER NUM IS LESS THAN 32??? */

   int32_t sum;
   sum = (int32_t)generalRegRead(regs, rs) + (int32_2)generalRegRead(regs, rt);
   generalRegWrite(regs, rd, (uint32_t)sum);
}

void instrADDU(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   uint32_t sum;
   sum = generalRegRead(regs, rs) + generalRegRead(regs, rt);
   generalRegWrite(regs, rd, sum);
}

void instrAND(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   uint32_t andVal;
   andVal = generalRegRead(regs, rs) & generalRegRead(regs, rt);
   generalRegWrite(regs, rd, andVal);
}

void instrNOR(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   unint32_t result;
   result = ~(generalRegRead(regs, rs) | generalRegRead(regs, rt));
   generalRegWrite(regs, rd, result);
}

void instrOR(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   unint32_t result;
   result = (generalRegRead(regs, rs) | generalRegRead(regs, rt));
   generalRegWrite(regs, rd, result);
}

void instrSLT(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   if((int32_t)generalRegRead(regs, rs) < (int32_t)generalRegRead(regs, rt)){
      generalRegWrite(regs, rd, (uint32_t)1);
   }
   else{
      generalRegWrite(regs, rd, (uint32_t)0);
   }
}

void instrSLTU(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   if(generalRegRead(regs, rs) < generalRegRead(regs, rt)){
      generalRegWrite(regs, rd, (uint32_t)1);
   }
   else{
      generalRegWrite(regs, rd, (uint32_t)0);
   }
}

/* SHAMT SHOULD BE UNISGNED???? */
void instrSLL(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   uint32_t result;
   result = generalRegRead(regs, rt) << shamt;
   generalRegWrite(regs, rd, result);
}

void instrSRL(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   uint32_t result;
   result = generalRegRead(regs, rt) >> shamt;
   generalRegWrite(regs, rd, result);
}

void instrSUB(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   int32_t sum;
   sum = (int32_t)generalRegRead(regs, rs) - (int32_2)generalRegRead(regs, rt);
   generalRegWrite(regs, rd, (uint32_t)sum);
}

void instrSUBU(uint32_t instr, int rs, int rt, int rd, int shamt)
{
   uint32_t sum;
   sum = generalRegRead(regs, rs) - generalRegRead(regs, rt);
   generalRegWrite(regs, rd, sum);
}

/* TO BE DONE WHEN WE FIGURE OUT PC */
void instrJR(uint32_t instr, int rs, int rt, int rd, int shamt)
{

}
/*--------------------------------------------------------------------*/

/* J TYPE INSTRUCTIONS- gotta figure out pc */
void instrJ(uint32_t instr)
{

}

void instrJAL(uint32_t instr)
{

}

/*--------------------------------------------------------------------*/

/* I TYPE INSTRUCTIONS */

/* signed extends the immediate to 32 bits */
uint32_t somethingToSignExtend(uint16_t imm){
   /* ooofff i hate bits... double check this */
   uint32_t newMasked;
   /* signed */
   if((1<<15 & imm)>>15){
      newMasked = (uint32_t)(0xFFFF0000 | imm);
   }
   /* unsigned */
   else{
      newMasked = (uint32_t)(0x00000000 | imm);
   }
   return newMasked;
}

void instrADDI(uint32_t instr)
{
   int rs, rt;
   uint16_t imm;
   int32_t result;

   iTypeDecode(instr, &rs, &rt, &imm);
   result = (int32_t)generalRegRead(regs,rs) + (int32_t)somethingToSignExtend(imm);
   generalRegWrite(regs, rt, (uint32_t)result);
}

void instrADDIU(uint32_t instr)
{
   int rs, rt;
   uint16_t imm;
   uint32_t result;

   iTypeDecode(instr, &rs, &rt, &imm);
   result = generalRegRead(regs,rs) + somethingToSignExtend(imm);
   generalRegWrite(regs, rt, result);
}

void instrANDI(uint32_t instr)
{
   int rs, rt;
   uint16_t imm;
   uint32_t result;

   iTypeDecode(instr, &rs, &rt, &imm);
   result = (uint32_t)(generalRegRead(regs, rs) & imm);
   generalRegWrite(regs, rt, result);
}

void instrBEQ(uint32_t instr)
{
   // this isnt done obviously im just showing this as an example
}

void instrBNE(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}

void instrLBU(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}

void instrLHU(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}

void instrLUI(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}

void instrLW(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}

void instrORI(uint32_t instr)
{
   int rs, rt;
   uint16_t imm;
   uint32_t result;

   iTypeDecode(instr, &rs, &rt, &imm);
   result = (uint32_t)(generalRegRead(regs, rs) | imm);
   generalRegWrite(regs, rt, result);
}

void instrSLTI(uint32_t instr)
{
   int rs, rt;
   uint16_t imm;

   iTypeDecode(instr, &rs, &rt, &imm);
   if((int32_t)generalRegRead(regs, rs) < (int32_t)somethingToSignExtend(imm)){
      generalRegWrite(regs, rt, (uint32_t)1);
   }
   else{
      generalRegWrite(regs, rt, (uint32_t)0);
   }
}

void instrSLTIU(uint32_t instr)
{
   int rs, rt;
   uint16_t imm;

   iTypeDecode(instr, &rs, &rt, &imm);
   if(generalRegRead(regs, rs) < somethingToSignExtend(imm)){
      generalRegWrite(regs, rt, (uint32_t)1);
   }
   else{
      generalRegWrite(regs, rt, (uint32_t)0);
   }
}

void instrSB(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}

void instrSH(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}

void instrSW(uint32_t instr)
{
       // this isnt done obviously im just showing this as an example
}
