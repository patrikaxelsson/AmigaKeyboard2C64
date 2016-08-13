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
		case AmigaKey_C:         c64_keyb_sim_setKey(C64Key_C,         up); break;
		case AmigaKey_D:         c64_keyb_sim_setKey(C64Key_D,         up); break;
		case AmigaKey_E:         c64_keyb_sim_setKey(C64Key_E,         up); break;
		case AmigaKey_F:         c64_keyb_sim_setKey(C64Key_F,         up); break;
		case AmigaKey_G:         c64_keyb_sim_setKey(C64Key_G,         up); break;
		case AmigaKey_H:         c64_keyb_sim_setKey(C64Key_H,         up); break;
		case AmigaKey_I:         c64_keyb_sim_setKey(C64Key_I,         up); break;
		case AmigaKey_J:         c64_keyb_sim_setKey(C64Key_J,         up); break;
		case AmigaKey_K:         c64_keyb_sim_setKey(C64Key_K,         up); break;
		case AmigaKey_L:         c64_keyb_sim_setKey(C64Key_L,         up); break;
		case AmigaKey_M:         c64_keyb_sim_setKey(C64Key_M,         up); break;
		case AmigaKey_N:         c64_keyb_sim_setKey(C64Key_N,         up); break;
		case AmigaKey_O:         c64_keyb_sim_setKey(C64Key_O,         up); break;
		case AmigaKey_P:         c64_keyb_sim_setKey(C64Key_P,         up); break;
		case AmigaKey_Q:         c64_keyb_sim_setKey(C64Key_Q,         up); break;
		case AmigaKey_R:         c64_keyb_sim_setKey(C64Key_R,         up); break;
		case AmigaKey_S:         c64_keyb_sim_setKey(C64Key_S,         up); break;
		case AmigaKey_T:         c64_keyb_sim_setKey(C64Key_T,         up); break;
		case AmigaKey_U:         c64_keyb_sim_setKey(C64Key_U,         up); break;
		case AmigaKey_V:         c64_keyb_sim_setKey(C64Key_V,         up); break;
		case AmigaKey_W:         c64_keyb_sim_setKey(C64Key_W,         up); break;
		case AmigaKey_X:         c64_keyb_sim_setKey(C64Key_X,         up); break;
		case AmigaKey_Y:         c64_keyb_sim_setKey(C64Key_Y,         up); break;
		case AmigaKey_Z:         c64_keyb_sim_setKey(C64Key_Z,         up); break;
		
		case AmigaKey_0:         c64_keyb_sim_setKey(C64Key_0,         up); break;
		case AmigaKey_1:         c64_keyb_sim_setKey(C64Key_1,         up); break;
		case AmigaKey_2:         c64_keyb_sim_setKey(C64Key_2,         up); break;
		case AmigaKey_3:         c64_keyb_sim_setKey(C64Key_3,         up); break;
		case AmigaKey_4:         c64_keyb_sim_setKey(C64Key_4,         up); break;
		case AmigaKey_5:         c64_keyb_sim_setKey(C64Key_5,         up); break;
		case AmigaKey_6:         c64_keyb_sim_setKey(C64Key_6,         up); break;
		case AmigaKey_7:         c64_keyb_sim_setKey(C64Key_7,         up); break;
		case AmigaKey_8:         c64_keyb_sim_setKey(C64Key_8,         up); break;
		case AmigaKey_9:         c64_keyb_sim_setKey(C64Key_9,         up); break;
		
		case AmigaKey_F1:        c64_keyb_sim_setKey(C64Key_F1,        up); break;
		case AmigaKey_F3:        c64_keyb_sim_setKey(C64Key_F3,        up); break;
		case AmigaKey_F5:        c64_keyb_sim_setKey(C64Key_F5,        up); break;
		case AmigaKey_F7:        c64_keyb_sim_setKey(C64Key_F7,        up); break;
		
		case AmigaKey_Minus:     c64_keyb_sim_setKey(C64Key_Minus,     up); break;
		case AmigaKey_Num_Plus:  c64_keyb_sim_setKey(C64Key_Plus,      up); break;
		case AmigaKey_Period:    c64_keyb_sim_setKey(C64Key_Period,    up); break;
		case AmigaKey_Comma:     c64_keyb_sim_setKey(C64Key_Comma,     up); break;
		case AmigaKey_SemiColon: c64_keyb_sim_setKey(C64Key_SemiColon, up); break;
		case AmigaKey_Slash:     c64_keyb_sim_setKey(C64Key_Slash,     up); break;
		case AmigaKey_Equals:    c64_keyb_sim_setKey(C64Key_Equals,    up); break;
		case AmigaKey_Tilde:     c64_keyb_sim_setKey(C64Key_LeftArrow, up); break;
		
		case AmigaKey_BackSpace: c64_keyb_sim_setKey(C64Key_Delete,    up); break;
		case AmigaKey_CursDown:  c64_keyb_sim_setKey(C64Key_CursDown,  up); break;
		case AmigaKey_CursRight: c64_keyb_sim_setKey(C64Key_CursRight, up); break;
		case AmigaKey_Delete:    c64_keyb_sim_setKey(C64Key_ClrHome,   up); break;
		
		case AmigaKey_LShift:    c64_keyb_sim_setKey(C64Key_LShift,    up); break;
		case AmigaKey_RShift:    c64_keyb_sim_setKey(C64Key_RShift,    up); break;
		case AmigaKey_Escape:    c64_keyb_sim_setKey(C64Key_RunStop,   up); break;
		case AmigaKey_LAmiga:    c64_keyb_sim_setKey(C64Key_Commodore, up); break;
		case AmigaKey_Ctrl:      c64_keyb_sim_setKey(C64Key_Control,   up); break;
		
		case AmigaKey_Space:     c64_keyb_sim_setKey(C64Key_Space,     up); break;
		case AmigaKey_Return:    c64_keyb_sim_setKey(C64Key_Return,    up); break;
		
		case AmigaKey_LeftBracket:  c64_keyb_sim_setKey(C64Key_Asterisk, up); break;
		case AmigaKey_RightBracket: c64_keyb_sim_setKey(C64Key_UpArrow,  up); break;
		case AmigaKey_BackSlash:    c64_keyb_sim_setKey(C64Key_Pound,    up); break;
		
		case AmigaKey_F9:        c64_keyb_sim_setKey(C64Key_At,        up); break;
		case AmigaKey_F10:       c64_keyb_sim_setKey(C64Key_Colon,     up); break;
		
		// Special
		case AmigaKey_F2:        c64_keyb_sim_setKey(C64Key_RShift,    up);
		                     c64_keyb_sim_setKey(C64Key_F1,        up); break;
		case AmigaKey_F4:        c64_keyb_sim_setKey(C64Key_RShift,    up);
		                     c64_keyb_sim_setKey(C64Key_F3,        up); break;
		case AmigaKey_F6:        c64_keyb_sim_setKey(C64Key_RShift,    up);
		                     c64_keyb_sim_setKey(C64Key_F5,        up); break;
		case AmigaKey_F8:        c64_keyb_sim_setKey(C64Key_RShift,    up);
		                     c64_keyb_sim_setKey(C64Key_F7,        up); break;
		
		case AmigaKey_CursLeft:  c64_keyb_sim_setKey(C64Key_RShift,    up);
		                     c64_keyb_sim_setKey(C64Key_CursRight, up); break;
		case AmigaKey_CursUp:    c64_keyb_sim_setKey(C64Key_RShift,    up);
		                     c64_keyb_sim_setKey(C64Key_CursDown,  up); break;
		
		// Num Pad
		case AmigaKey_Num_0:     c64_keyb_sim_setKey(C64Key_0,         up); break;
		case AmigaKey_Num_1:     c64_keyb_sim_setKey(C64Key_1,         up); break;
		case AmigaKey_Num_2:     c64_keyb_sim_setKey(C64Key_2,         up); break;
		case AmigaKey_Num_3:     c64_keyb_sim_setKey(C64Key_3,         up); break;
		case AmigaKey_Num_4:     c64_keyb_sim_setKey(C64Key_4,         up); break;
		case AmigaKey_Num_5:     c64_keyb_sim_setKey(C64Key_5,         up); break;
		case AmigaKey_Num_6:     c64_keyb_sim_setKey(C64Key_6,         up); break;
		case AmigaKey_Num_7:     c64_keyb_sim_setKey(C64Key_7,         up); break;
		case AmigaKey_Num_8:     c64_keyb_sim_setKey(C64Key_8,         up); break;
		case AmigaKey_Num_9:     c64_keyb_sim_setKey(C64Key_9,         up); break;
		
		case AmigaKey_Num_Enter:           c64_keyb_sim_setKey(C64Key_Return,   up); break;
		case AmigaKey_Num_Slash:           c64_keyb_sim_setKey(C64Key_Slash,    up); break;
		case AmigaKey_Apostrophe:          c64_keyb_sim_setKey(C64Key_Plus,     up); break;
		case AmigaKey_Num_Minus:           c64_keyb_sim_setKey(C64Key_Minus,    up); break;
		case AmigaKey_Num_Period:          c64_keyb_sim_setKey(C64Key_Period,   up); break;
		case AmigaKey_Num_Asterisk:        c64_keyb_sim_setKey(C64Key_Asterisk, up); break;
		case AmigaKey_Num_LeftParenthese:  c64_keyb_sim_setKey(C64Key_RShift,   up);
		                               c64_keyb_sim_setKey(C64Key_8,        up); break;
		case AmigaKey_Num_RightParenthese: c64_keyb_sim_setKey(C64Key_RShift,   up);
                               c64_keyb_sim_setKey(C64Key_9,        up); break;
    }
}

int main(void) {
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
