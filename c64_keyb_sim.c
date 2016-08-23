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
#elif defined (__AVR_ATmega88__)
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
// There are only 8 different column states, but as the C64 can pull
// any combination of the 8 column pins low, there are actually 256
// combinations of column state. As the ISR code must set the row output very
// quickly, the best way was to use a table containing all combinations,
// where the column input is the index.

// Aligned on even 256-Bytes to save an instruction in the ISR below
static uint8_t colStates[512] __attribute__((aligned(0x100))) = {0};
register uint8_t currentColStatesHighByte asm ("r27");

/*ISR (PCINT0_vect) {
	KEYB_ROWS_DDR = colStates[KEYB_COLS_IN];
}*/

// Must be in assembler to be quick enough to work. This one takes 4 cycles
// (without counting reti) by using reserved registers and no instructions
// that affects SREG so we don't need to push and pop anything. The commented
// out version above in C would take 34 cycles when compiled by gcc (without
// counting reti).

// Still approximately minimum 10MHz CPU frequency is needed to make picky
// things like the 1541 Ultimate II menu work. The ROM keyboard code is much
// more forgiving.
ISR (KEYB_CHANGE_VECT, ISR_NAKED) {
	asm (
		"in   XL, %[COLS_IN]    \n" // Read colStates array index from cols input
		"ld   r2, X             \n" // The current colStates array is always in upper byte of register pair X
		"out  %[ROWS_DDR], r2   \n" // Output on port is inverted when setting DDR as KEYB_ROWS_OUT is 0x00
		
		"reti                   \n"
		:
		:
			[COLS_IN]  "I" (_SFR_IO_ADDR(KEYB_COLS_IN)),
			[ROWS_DDR] "M" (_SFR_IO_ADDR(KEYB_ROWS_DDR))
	);
}

static uint8_t *getCurrentColStates(void) {
	return (uint8_t *) (currentColStatesHighByte << 8);
}

static void setCurrentColStates(uint8_t *nextColStates) {
	currentColStatesHighByte = (uint16_t) nextColStates >> 8;
	// Update current output in case we have a keypress during the time the columns
	// selected doesn't change - like when code has set a column, and is looking
	// for a specific row output in an infinite loop without changing the columns.
	KEYB_ROWS_DDR = nextColStates[KEYB_COLS_IN];
}

static uint8_t *getNextColStates(void) {
	if(getCurrentColStates() == colStates)
		return colStates + 256;
	else
		return colStates;
}

void c64_keyb_sim_init() {
	setCurrentColStates(colStates);
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
	uint8_t *nextColStates = getNextColStates();

	uint8_t i = 0;
	do {
		nextColStates[i] = 0;
	} while(i++ < 255);

	setCurrentColStates(nextColStates);
}

void c64_keyb_sim_setKey(uint8_t c64Key, bool up) {
	if(C64Key_Restore == c64Key) {
		setRestore(up);
	}
	else {
		// Up is more complicated than down as it needs to clear all combinations
		// where this column is present, but cannot clear any combinations where
		// other keys on the same row are pressed. The up algorithm could be used
		// for both down and up, but it is quite a bit slower, so used an as simple
		// as possible for down as if any time would be more critical, it is pressing
		// down.
		//
		// Benchmark at 8MHz:
		// setKey down: 623
		// setKey up:   936

		const uint8_t col = c64Key >> 4;
		const uint8_t colMask = 1 << (col);
		const uint8_t rowMask = 1 << (c64Key & 0x7);
		uint8_t *currentColStates = getCurrentColStates();
		uint8_t *nextColStates = getNextColStates();
		if(up) {
			const uint8_t colState0 = 0 != col ? currentColStates[(uint8_t) ~(1 << 0)] : currentColStates[(uint8_t) ~(1 << 0)] & ~rowMask; 
			const uint8_t colState1 = 1 != col ? currentColStates[(uint8_t) ~(1 << 1)] : currentColStates[(uint8_t) ~(1 << 1)] & ~rowMask;
			const uint8_t colState2 = 2 != col ? currentColStates[(uint8_t) ~(1 << 2)] : currentColStates[(uint8_t) ~(1 << 2)] & ~rowMask;
			const uint8_t colState3 = 3 != col ? currentColStates[(uint8_t) ~(1 << 3)] : currentColStates[(uint8_t) ~(1 << 3)] & ~rowMask;
			const uint8_t colState4 = 4 != col ? currentColStates[(uint8_t) ~(1 << 4)] : currentColStates[(uint8_t) ~(1 << 4)] & ~rowMask;
			const uint8_t colState5 = 5 != col ? currentColStates[(uint8_t) ~(1 << 5)] : currentColStates[(uint8_t) ~(1 << 5)] & ~rowMask;
			const uint8_t colState6 = 6 != col ? currentColStates[(uint8_t) ~(1 << 6)] : currentColStates[(uint8_t) ~(1 << 6)] & ~rowMask;
			const uint8_t colState7 = 7 != col ? currentColStates[(uint8_t) ~(1 << 7)] : currentColStates[(uint8_t) ~(1 << 7)] & ~rowMask;
	
			uint8_t i = 0;
			do {
				uint8_t thisColState = 0x00;
				if((1 << 0) & i) thisColState |= colState0;
				if((1 << 1) & i) thisColState |= colState1;
				if((1 << 2) & i) thisColState |= colState2;
				if((1 << 3) & i) thisColState |= colState3;
				if((1 << 4) & i) thisColState |= colState4;
				if((1 << 5) & i) thisColState |= colState5;
				if((1 << 6) & i) thisColState |= colState6;
				if((1 << 7) & i) thisColState |= colState7;

				nextColStates[(uint8_t) ~i] = thisColState;
			} while(i++ < 255);
		}
		// Down
		else {
			uint8_t i = 0;
			do {
				// Observe that the index is inverted to match the inverted KEYB_COLS_IN input
				nextColStates[(uint8_t) ~i] = colMask & i ? currentColStates[(uint8_t) ~i] | rowMask : currentColStates[(uint8_t) ~i];
			} while(i++ < 255);
		}
		setCurrentColStates(nextColStates);
	}
}
