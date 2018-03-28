    addi $t0,$zero,32767
    addi $t1,$zero,32760
    sll  $t0,$t0,0x10
    add  $t2,$t1,$t0
    addiu $t3, $zero, 0x1234
    sll $t3, 4
    sw $t3, 380($zero)
    lbu $t4, 380($zero)
    lhu $t5, 380($zero)

    .align 4
    	.word 0xfeedfeed
    	.word 0x0
