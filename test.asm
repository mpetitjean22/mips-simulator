    addi $t0,$zero,32767
    addi $t1,$zero,32760
    sll  $t0,$t0,0x10
    add  $t2,$t1,$t0
    
    .align 4
    	.word 0xfeedfeed
    	.word 0x0
    
