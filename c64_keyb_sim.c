#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "c64_keyb_codes.h"

#if defined (__AVR_ATmega324P__)
	#define KEYB_COLS_IN PINA
	#define KEYB_COLS_DDR DDRA
	#define KEYB_ROWS_IN PINC
	#define KEYB_ROWS_OUT PORTC
	#define KEYB_ROWS_DDR DDRC
	#define KEYB_RESTORE_OUT PORTD
	#define KEYB_RESTORE_DDR DDRD
	#define KEYB_RESTORE_MASK _BV(PIND4)
	#define KEYB_CHANGE_VECT PCINT0_vect
	#define KEYB_CHANGE_PORTMASK _BV(PCIE0)
	#define KEYB_CHANGE_MASKREG PCMSK0
#elif defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__)
	#define KEYB_COLS_IN PIND
	#define KEYB_COLS_DDR DDRD
	#define KEYB_ROWS_IN PINB
	#define KEYB_ROWS_OUT PORTB
	#define KEYB_ROWS_DDR DDRB
	#define KEYB_RESTORE_OUT PORTC
	#define KEYB_RESTORE_DDR DDRC
	#define KEYB_RESTORE_MASK _BV(PINC2)
	#define KEYB_CHANGE_VECT PCINT2_vect
	#define KEYB_CHANGE_PORTMASK _BV(PCIE2)
	#define KEYB_CHANGE_MASKREG PCMSK2
#endif
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
ISR (KEYB_CHANGE_VECT, ISR_NAKED) {
	asm (
		"push r24                \n"
		"in   r24, __SREG__      \n"
		"push r24                \n"
		"push r30                \n"
		"push r31                \n"
		
		"in   r30, %[COLS_IN]    \n" // Read rowState array index from cols input
		"ldi  r31, hi8(rowState) \n" // rowState is aligned on even 256-Bytes, so only high byte is needed
		"ld   r24, Z             \n"
		"out  %[ROWS_DDR], r24   \n" // Output on port is inverted when setting DDR as KEYB_ROWS_OUT is 0x00
		
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
	KEYB_COLS_DDR = 0x00; // Input

	KEYB_ROWS_DDR = 0x00; // Input
	KEYB_ROWS_OUT = 0x00; // Output zero whenever set to output

	KEYB_RESTORE_DDR &= ~KEYB_RESTORE_MASK; // Input
	KEYB_RESTORE_OUT &= ~KEYB_RESTORE_MASK; // Output zero whenever set to output

	PCICR |= KEYB_CHANGE_PORTMASK;
	KEYB_CHANGE_MASKREG = 0xff; // Generate interrupt on all COLS
	sei();
}

static void setRestore(bool up) {
	if(up) {
		KEYB_RESTORE_DDR &= ~KEYB_RESTORE_MASK;
	}
	else {
		KEYB_RESTORE_DDR |= KEYB_RESTORE_MASK;
	}
}

void c64_keyb_sim_resetState(void) {
	uint8_t i = 0;
	do {
		rowState[i] = 0;
	} while(i++ < 255);
}

void c64_keyb_sim_setKey(uint8_t c64Key, bool up) {
	if(C64Key_Restore == c64Key) {
		setRestore(up);
	}
	else {
		const uint8_t colMask = 1 << (c64Key >> 4);
		const uint8_t rowMask = 1 << (c64Key & 0x7);
		// Up is more complicated than down as it needs to clear all combinations
		// where this column is present, but cannot clear any combinations where
		// other keys on the same row are pressed. The up algorithm could be used
		// for both down and up, but it is quite a bit slower, so used an as simple
		// as possible for down as if any time would be more critical, it is pressing
		// down.
		// Benchmark of current solution vs old which didn't work correctly on up in
		// microseconds:
		// Old setKey down: 420
		// Old setKey up:   436
		// New setKey down: 372
		// New setKey up:   919
		if(up) {
			rowState[(uint8_t) ~colMask] &= ~rowMask;
			// Observe that the index is inverted to match the inverted KEYB_COLS_IN input
			const uint8_t rowStateCol0 = rowState[(uint8_t) ~(1 << 0)];
			const uint8_t rowStateCol1 = rowState[(uint8_t) ~(1 << 1)];
			const uint8_t rowStateCol2 = rowState[(uint8_t) ~(1 << 2)];
			const uint8_t rowStateCol3 = rowState[(uint8_t) ~(1 << 3)];
			const uint8_t rowStateCol4 = rowState[(uint8_t) ~(1 << 4)];
			const uint8_t rowStateCol5 = rowState[(uint8_t) ~(1 << 5)];
			const uint8_t rowStateCol6 = rowState[(uint8_t) ~(1 << 6)];
			const uint8_t rowStateCol7 = rowState[(uint8_t) ~(1 << 7)];
	
			uint8_t i = 0;
			do {
				uint8_t currentRowState = 0x00;
				if((1 << 0) & i) currentRowState |= rowStateCol0;
				if((1 << 1) & i) currentRowState |= rowStateCol1;
				if((1 << 2) & i) currentRowState |= rowStateCol2;
				if((1 << 3) & i) currentRowState |= rowStateCol3;
				if((1 << 4) & i) currentRowState |= rowStateCol4;
				if((1 << 5) & i) currentRowState |= rowStateCol5;
				if((1 << 6) & i) currentRowState |= rowStateCol6;
				if((1 << 7) & i) currentRowState |= rowStateCol7;

				rowState[(uint8_t) ~i] = currentRowState;
			} while(i++ < 255);
		}
		// Down
		else {
			uint8_t i = 0;
			do {
				if(colMask & i) {
					// Observe that the index is inverted to match the inverted KEYB_COLS_IN input
					rowState[(uint8_t) ~i] |= rowMask;
				}
			} while(i++ < 255);
		}
	}
}
