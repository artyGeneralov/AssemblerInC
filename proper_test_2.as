;Another proper test, testing each command

.extern A
.extern B
.entry MAIN

MAIN: mov A, r4
	cmp A, r2
	add r4, r6
LOOP:	sub #14, r2
	lea STR, r0
	clr r1
	not r4
	inc r2
	jmp &LOOP
	bne &END
	jsr &LOOP
	red r4
	prn r1
	rts
END:	stop



DATA: .data 1,2,3,4,5
	.data 6,7,8,9
STR: .string "abc"
	.string "def"
