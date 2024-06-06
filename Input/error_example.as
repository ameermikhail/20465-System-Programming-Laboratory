; file error_example.as

.entry LENGTH
.extern W

MAIN: mov     r3, LENGTH, r4
LOOP: jmp     L1, L2
    mcr    m1 m2
        sub     r1, r4
        bne     L3
    endmcr
    add     r2 r3 r4
    prn     -5, 4, 8
    bne     W
    m1
L1: inc     K, 6
.entry LOOP
    jmp     W
END: stop
STR:    .string "abcdef"
LENGTH: .data   6,-9,15
K:      .data   22
.extern L3
