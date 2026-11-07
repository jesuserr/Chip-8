; ---------------------------------------------------------------------------
; IBM_Logo.ch8 -- reconstructed source (byte-for-byte match with the ROM)
;
;   ROM size : 132 bytes, loaded at 0x200 (code 0x200-0x229, data 0x22A-0x283)
;
; The program draws six 8x15 sprites side by side at y = 8 (the striped
; IBM logo) and then freezes the picture with an infinite loop.
;
; Mnemonics are generic Chip-8 syntax; adjust names (DRW/DRW/VDRW, DB/.BYTE)
; to whichever assembler you use.
; ---------------------------------------------------------------------------

        ORG     0x200

        CLS                     ; 00E0  clear screen
        LD      I, glyph1       ; A22A  sprite data at 0x22A
        LD      V0, 12          ; 600C  x = 12
        LD      V1, 8           ; 6108  y = 8
        DRW     V0, V1, 15      ; D01F  draw 15 rows
        ADD     V0, 9           ; 7009  x = 21
        LD      I, glyph2       ; A239
        DRW     V0, V1, 15      ; D01F
        LD      I, glyph3       ; A248
        ADD     V0, 8           ; 7008  x = 29
        DRW     V0, V1, 15      ; D01F
        ADD     V0, 4           ; 7004  x = 33
        LD      I, glyph4       ; A257
        DRW     V0, V1, 15      ; D01F
        ADD     V0, 8           ; 7008  x = 41
        LD      I, glyph5       ; A266
        DRW     V0, V1, 15      ; D01F
        ADD     V0, 8           ; 7008  x = 49
        LD      I, glyph6       ; A275
        DRW     V0, V1, 15      ; D01F
halt:   JP      halt            ; 1228  keep the logo on screen forever

; --- sprite data: every glyph is 8 pixels wide, 15 rows tall ----------------

glyph1:                         ; 0x22A
        DB      0xFF, 0x00, 0xFF, 0x00, 0x3C, 0x00, 0x3C, 0x00
        DB      0x3C, 0x00, 0x3C, 0x00, 0xFF, 0x00, 0xFF
glyph2:                         ; 0x239
        DB      0xFF, 0x00, 0xFF, 0x00, 0x38, 0x00, 0x3F, 0x00
        DB      0x3F, 0x00, 0x38, 0x00, 0xFF, 0x00, 0xFF
glyph3:                         ; 0x248
        DB      0x80, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x80, 0x00
        DB      0x80, 0x00, 0xE0, 0x00, 0xE0, 0x00, 0x80
glyph4:                         ; 0x257
        DB      0xF8, 0x00, 0xFC, 0x00, 0x3E, 0x00, 0x3F, 0x00
        DB      0x3B, 0x00, 0x39, 0x00, 0xF8, 0x00, 0xF8
glyph5:                         ; 0x266
        DB      0x03, 0x00, 0x07, 0x00, 0x0F, 0x00, 0xBF, 0x00
        DB      0xFB, 0x00, 0xF3, 0x00, 0xE3, 0x00, 0x43
glyph6:                         ; 0x275
        DB      0xE0, 0x00, 0xE0, 0x00, 0x80, 0x00, 0x80, 0x00
        DB      0x80, 0x00, 0x80, 0x00, 0xE0, 0x00, 0xE0
