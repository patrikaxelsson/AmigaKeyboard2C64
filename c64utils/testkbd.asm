PRA  =  $dc00            ; CIA#1 (Port Register A)
DDRA =  $dc02            ; CIA#1 (Data Direction Register A)

PRB  =  $dc01            ; CIA#1 (Port Register B)
DDRB =  $dc03            ; CIA#1 (Data Direction Register B)

TEMP    = $02
TEMPA	= $03
SCRATCH = $4000
SCREEN  = $0400
.byte $01, $08
.org  $0801
.byte $0c,$08,$d0,$07,$9e,$20,$32,$30,$36,$34,$00,$00,$00,$00,$00	; Basic "SYS 2064"

	sei			; interrupts deactivated
        jsr $ff81               ; kernal clear screen sub-routine

	lda #$30		; $30 = petscii zero, $31 = petscii one and so on...
	sta TEMP

	lda #$00
	sta TEMPA
	lda #$04
	sta TEMPA+1		; set TEMPA to $0400 (start of screen ram)

	ldy #$00
@wr_row_loop:			; Writes "  ROW 01234567" to screen
	lda datarow, y
	sta (TEMPA), y
	iny
	cpy #$0e
	bne @wr_row_loop

        lda #$28
        sta TEMPA

	ldy #$00
	ldx #$07

@wr_col_loop:			; this loop writes "COL :" to the screen 8 times
	lda datacol, y
	sta (TEMPA), y
	iny
	cpy #$05
	bne @wr_col_loop

	ldy #$03
	lda TEMP
	sta (TEMPA), y		; writes the number for each column
	inc TEMP		; increase the number for each column

	lda TEMPA
	clc
	adc #$28		; add 40 to TEMPA move to the next screen-line
	sta TEMPA
	bcc @nocarry
	inc TEMPA+1		; increase highbyte if needed
@nocarry:
	ldy #$00
	dex
	bpl @wr_col_loop

	lda #$2e
	sta TEMPA
	lda #$04
	sta TEMPA+1		; set TEMPA to $0406 (start of where we wants the bits on screen)

	lda #%11111111		; CIA#1 port A = outputs 
	sta DDRA             
	lda #%00000000		; CIA#1 port B = inputs
	sta DDRB             
	lda #%11111110
	sta TEMP

; -------- Read all rows and store results in SCRATCH --------

	ldx #$07
@loop:
	lda TEMP		; TEMP = the row we check
        sta PRA
	sec			; set carry so we get a one in when we rotate left
	rol
	sta TEMP
	lda PRB
	sta SCRATCH, x
	dex
	bpl @loop

; -------- Write the bits from SCRATCH to screen --------

        ldx #$07
	ldy #$00
@loop_o:			; outer loop
	lda #$07
	sta TEMP
	lda SCRATCH, x
@loop_i:			; inner loop
	ror
	pha
	bcc @wr_z
	lda #$31		; One in Petscii
	sta (TEMPA), y
	jmp @done_wr
@wr_z:	
	lda #$30		; Zero in Petscii
	sta (TEMPA), y
@done_wr:
	pla
	iny
	dec TEMP
	bpl @loop_i

	clc
	lda TEMPA
	adc #$20
	sta TEMPA
	bcc @done
	inc TEMPA+1
@done:
	dex
	bpl @loop_o

; -------- prepare everything for looping --------

        ldy #$00
	lda #$2e
        sta TEMPA
        lda #$04
        sta TEMPA+1

        lda #%11111110
        sta TEMP
	ldx #$07
	jmp @loop

	cli			; interrupts activated
	rts			; back to BASIC


datacol:
	.byte $03, $0f, $0c, $20, $3a    ; COL<space>:
datarow:
	.byte $20, $20, $12, $0f, $17, $20, $30, $31, $32, $33, $34, $35, $36, $37	; <space><space>ROW<space>12345678
