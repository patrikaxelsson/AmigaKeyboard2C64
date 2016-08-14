#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define KEYB_COLS_IN PINA
#define KEYB_COLS_OUT PINA
#define KEYB_COLS_DDR DDRA
#define KEYB_ROWS_IN PINC
#define KEYB_ROWS_OUT PORTC
#define KEYB_ROWS_DDR DDRC

// This array contains the state of the keys for all combinations of columns.
// There are only 8 different rows of column state, but as the C64 can pull
// any combination of the 8 column pins low, there are actually 256
// combinations of row state. As the ISR code must set the row output very
// quickly, the best way was to use a table containing all combinations,
// where the column input is the index.

// Aligned on even 256-Bytes to save an instruction in the ISR below
static uint8_t rowState[256] __attribute__((aligned(0x100))) = {0};

/*ISR (PCINT0_vect) {
	KEYB_ROWS_DDR = rowState[KEYB_COLS_IN];
}*/

// Must be in assembler to be quick enough to work at 8MHz. This one takes 23
// cycles, while the commented out one above would take 34 cycles when compiled
// by gcc (counted push and pop as 2 cycles and other instructions as 1 cycle).
ISR (PCINT0_vect, ISR_NAKED) {
	asm (
		"push r24                \n"
		"in   r24, __SREG__      \n"
		"push r24                \n"
		"push r30                \n"
		"push r31                \n"
		
		"in   r30, %[COLS_IN]    \n" // Read rowState array index from cols input
		"ldi  r31, hi8(rowState) \n" // rowState is aligned on even 256-Bytes, so only high byte is needed
		"ld   r24, Z             \n"
		"out  %[ROWS_DDR], r24   \n"
		
		"pop  r31                \n"
		"pop  r30                \n"
		"pop  r24                \n"
		"out  __SREG__, r24      \n"
		"pop r24                 \n"
		"reti                    \n"
		:
		:
			[COLS_IN]  "I" (_SFR_IO_ADDR(KEYB_COLS_IN)),
			[ROWS_DDR] "M" (_SFR_IO_ADDR(KEYB_ROWS_DDR))
	);
}

void c64_keyb_sim_init() {
	KEYB_COLS_DDR = 0x00; //Input

	KEYB_ROWS_DDR = 0x00; //Input
	KEYB_ROWS_OUT = 0x00; //Output zero whenever set to output

	PCICR |= _BV(PCIE0);
	PCMSK0 = 0xff; // Generate interrupt on all COLS
	sei();
}

void c64_keyb_sim_setKey(uint8_t c64Key, bool up) {
	const uint8_t column = c64Key >> 4;
	const uint8_t rowData = 1 << (c64Key & 0x7);
	uint16_t i;
	
	for(i = 0; i < 256; i++) {
		if(i & (1 << column)) {
			if(up) {
				rowState[(uint8_t)~i] &= ~rowData;
			}
			else {
				rowState[(uint8_t)~i] |= rowData;
			}
		}
	}
}
