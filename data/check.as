MAIN:       mov     @r3 ,       LENGTH
LOOP:       jmp     L1
            prn     -5
            bne     W
            sub     @r1, @r4
            bne     L3
L1:        inc     K
            jmp     W
END:        stop