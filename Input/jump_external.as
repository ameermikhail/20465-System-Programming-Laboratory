;file jump_external.as
.entry SET
.extern U
.define size = 1
MAIN:	mov     r6, SET[size]
LOOP:	jmp     U
        cmp     N, #size
        bne     U
        prn    	#0
        mov		CHAIN[2], CHAIN[5]
        sub		r7, r6
L6:	inc     U2
.entry LOOP
	bne	LOOP
END:	hlt
.define	len = 2
CHAIN:  .string "abcde"
SET:	.data	0, -4, len
N:	    .data	6
.extern U2
