; CHIP-8 "Hello, World!" in assembly
; Displays "C0DE CAFE"

    CLS

    ; Draw 'C'
    LD V0, 0xC
    LD F, V0
    LD V1, 4
    LD V2, 10
    DRW V1, V2, 5

    ; Draw '0'
    LD V0, 0x0
    LD F, V0
    LD V1, 9
    DRW V1, V2, 5

    ; Draw 'D'
    LD V0, 0xD
    LD F, V0
    LD V1, 14
    DRW V1, V2, 5

    ; Draw 'E'
    LD V0, 0xE
    LD F, V0
    LD V1, 19
    DRW V1, V2, 5

    ; Draw 'C' (space)
    LD V0, 0xC
    LD F, V0
    LD V1, 26
    DRW V1, V2, 5

    ; Draw 'A'
    LD V0, 0xA
    LD F, V0
    LD V1, 31
    DRW V1, V2, 5

    ; Draw 'F'
    LD V0, 0xF
    LD F, V0
    LD V1, 36
    DRW V1, V2, 5

    ; Draw 'E'
    LD V0, 0xE
    LD F, V0
    LD V1, 41
    DRW V1, V2, 5

loop:
    JP loop