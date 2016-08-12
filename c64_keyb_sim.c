#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define KEYB_COLS_IN PINA
#define KEYB_COLS_OUT PINA
#define KEYB_COLS_DDR DDRA
#define KEYB_ROWS_IN PINC
#define KEYB_ROWS_OUT PORTC
#define KEYB_ROWS_DDR DDRC

static uint8_t rowState[256] = {0};

ISR (PCINT0_vect) {
	KEYB_ROWS_DDR = rowState[KEYB_COLS_IN];
}

void c64_keyb_sim_init() {
	KEYB_COLS_DDR = 0x00; //Input
	KEYB_COLS_OUT = 0xff; //Pullup

	KEYB_ROWS_DDR = 0x00; //Input
	KEYB_ROWS_OUT = 0x00; //Output zero whenever set to output

	PCICR |= _BV(PCIE0);
	PCMSK0 = 0xff; // Generate input on all COLS
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
