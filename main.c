#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#include "amiga_keyb_if.h"
#include "c64_keyb_sim.h"
#include "amiga_keyb_codes.h"
#include "c64_keyb_codes.h"

#include "uart.h"

void updateC64KeyState(uint8_t amigaKey, bool up) {
	switch(amigaKey) {
		case AmigaKey_A:         c64_keyb_sim_setKey(C64Key_A,         up); break;
		case AmigaKey_B:         c64_keyb_sim_setKey(C64Key_B,         up); break;
		case AmigaKey_P:         c64_keyb_sim_setKey(C64Key_P,         up); break;

		case AmigaKey_Space:     c64_keyb_sim_setKey(C64Key_Space,     up); break;
		case AmigaKey_Return:    c64_keyb_sim_setKey(C64Key_Return,    up); break;
    }
}

int main(void) {
	MCUCR |= _BV(JTD);
	MCUCR |= _BV(JTD);
	uart_init();
	FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &uart_output;

	amiga_keyb_if_init();
	amiga_keyb_if_registerChangeCallback(&updateC64KeyState);

	puts("starting!!!");

	c64_keyb_sim_init();

	uint8_t prevAmigaKey = amiga_keyb_if_getKey();
	for(;;) {
		const uint8_t amigaKey = amiga_keyb_if_getKey();
		if (amigaKey != prevAmigaKey) {
			printf("%02x\n", amigaKey);
		}
		prevAmigaKey = amigaKey;
	}

    return 0;
}
