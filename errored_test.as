;this test contains errors for the assembler to catch

MAIN: add r0 #94

jmp &WrongLabel

;invalid addressing mode for inc:

inc #55

;invalid label name:
2AnotherBadLabel: add r3 r2


;extern file will not be created:
.extern EXT

;technically correct, but no entry will be created:
.entry MAIN

;invalid addressing mode for clr
clr &MAIN

/*I Am Not A Comment!*/

