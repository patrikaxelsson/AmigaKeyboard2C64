#include <avr/io.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdbool.h>

#include "amiga_keyb_if.h"
#include "c64_keyb_sim.h"
#include "amiga_keyb_codes.h"
#include "c64_keyb_codes.h"

//#include "uart.h"

#if defined (__AVR_ATmega324P__)
	#define RESET_OUT PORTD
	#define RESET_DDR DDRD
	#define RESET_MASK _BV(PIND5)
#elif defined (__AVR_ATmega48__) || defined (__AVR_ATmega88__)
	#define RESET_OUT PORTC
	#define RESET_DDR DDRC
	#define RESET_MASK _BV(PIND3)
#endif

static const uint8_t amigaToC64Map[][2] = {
	[AmigaKey_A]                   = {C64Key_A,         0xff},
	[AmigaKey_B]                   = {C64Key_B,         0xff},
	[AmigaKey_C]                   = {C64Key_C,         0xff},
	[AmigaKey_D]                   = {C64Key_D,         0xff},
	[AmigaKey_E]                   = {C64Key_E,         0xff},
	[AmigaKey_F]                   = {C64Key_F,         0xff},
	[AmigaKey_G]                   = {C64Key_G,         0xff},
	[AmigaKey_H]                   = {C64Key_H,         0xff},
	[AmigaKey_I]                   = {C64Key_I,         0xff},
	[AmigaKey_J]                   = {C64Key_J,         0xff},
	[AmigaKey_K]                   = {C64Key_K,         0xff},
	[AmigaKey_L]                   = {C64Key_L,         0xff},
	[AmigaKey_M]                   = {C64Key_M,         0xff},
	[AmigaKey_N]                   = {C64Key_N,         0xff},
	[AmigaKey_O]                   = {C64Key_O,         0xff},
	[AmigaKey_P]                   = {C64Key_P,         0xff},
	[AmigaKey_Q]                   = {C64Key_Q,         0xff},
	[AmigaKey_R]                   = {C64Key_R,         0xff},
	[AmigaKey_S]                   = {C64Key_S,         0xff},
	[AmigaKey_T]                   = {C64Key_T,         0xff},
	[AmigaKey_U]                   = {C64Key_U,         0xff},
	[AmigaKey_V]                   = {C64Key_V,         0xff},
	[AmigaKey_W]                   = {C64Key_W,         0xff},
	[AmigaKey_X]                   = {C64Key_X,         0xff},
	[AmigaKey_Y]                   = {C64Key_Y,         0xff},
	[AmigaKey_Z]                   = {C64Key_Z,         0xff},

	[AmigaKey_0]                   = {C64Key_0,         0xff},
	[AmigaKey_1]                   = {C64Key_1,         0xff},
	[AmigaKey_2]                   = {C64Key_2,         0xff},
	[AmigaKey_3]                   = {C64Key_3,         0xff},
	[AmigaKey_4]                   = {C64Key_4,         0xff},
	[AmigaKey_5]                   = {C64Key_5,         0xff},
	[AmigaKey_6]                   = {C64Key_6,         0xff},
	[AmigaKey_7]                   = {C64Key_7,         0xff},
	[AmigaKey_8]                   = {C64Key_8,         0xff},
	[AmigaKey_9]                   = {C64Key_9,         0xff},

	[AmigaKey_F1]                  = {C64Key_F1,        0xff},
	[AmigaKey_F3]                  = {C64Key_F3,        0xff},
	[AmigaKey_F5]                  = {C64Key_F5,        0xff},
	[AmigaKey_F7]                  = {C64Key_F7,        0xff},

	[AmigaKey_Minus]               = {C64Key_Minus,     0xff},
	[AmigaKey_Num_Plus]            = {C64Key_Plus,      0xff},
	[AmigaKey_Period]              = {C64Key_Period,    0xff},
	[AmigaKey_Comma]               = {C64Key_Comma,     0xff},
	[AmigaKey_SemiColon]           = {C64Key_SemiColon, 0xff},
	[AmigaKey_Slash]               = {C64Key_Slash,     0xff},
	[AmigaKey_Equals]              = {C64Key_Equals,    0xff},
	[AmigaKey_Tilde]               = {C64Key_LeftArrow, 0xff},

	[AmigaKey_BackSpace]           = {C64Key_Delete,    0xff},
	[AmigaKey_CursDown]            = {C64Key_CursDown,  0xff},
	[AmigaKey_CursRight]           = {C64Key_CursRight, 0xff},
	[AmigaKey_Delete]              = {C64Key_ClrHome,   0xff},

	[AmigaKey_LShift]              = {C64Key_LShift,    0xff},
	[AmigaKey_RShift]              = {C64Key_RShift,    0xff},
	[AmigaKey_Escape]              = {C64Key_RunStop,   0xff},
	[AmigaKey_LAmiga]              = {C64Key_Commodore, 0xff},
	[AmigaKey_Ctrl]                = {C64Key_Control,   0xff},

	[AmigaKey_Space]               = {C64Key_Space,     0xff},
	[AmigaKey_Return]              = {C64Key_Return,    0xff},

	[AmigaKey_LeftBracket]         = {C64Key_Asterisk,  0xff},
	[AmigaKey_RightBracket]        = {C64Key_UpArrow,   0xff},
	[AmigaKey_BackSlash]           = {C64Key_Pound,     0xff},

	[AmigaKey_F9]                  = {C64Key_At,        0xff},
	[AmigaKey_F10]                 = {C64Key_Colon,     0xff},
	[AmigaKey_Help]                = {C64Key_Restore,   0xff},

	// Special
	[AmigaKey_F2]                  = {C64Key_RShift,    C64Key_F1},
	[AmigaKey_F4]                  = {C64Key_RShift,    C64Key_F3},
	[AmigaKey_F6]                  = {C64Key_RShift,    C64Key_F5},
	[AmigaKey_F8]                  = {C64Key_RShift,    C64Key_F7},

	[AmigaKey_CursLeft]            = {C64Key_RShift,    C64Key_CursRight},
	[AmigaKey_CursUp]              = {C64Key_RShift,    C64Key_CursDown},

	// Num Pad
	[AmigaKey_Num_0]               = {C64Key_0,         0xff},
	[AmigaKey_Num_1]               = {C64Key_1,         0xff},
	[AmigaKey_Num_2]               = {C64Key_2,         0xff},
	[AmigaKey_Num_3]               = {C64Key_3,         0xff},
	[AmigaKey_Num_4]               = {C64Key_4,         0xff},
	[AmigaKey_Num_5]               = {C64Key_5,         0xff},
	[AmigaKey_Num_6]               = {C64Key_6,         0xff},
	[AmigaKey_Num_7]               = {C64Key_7,         0xff},
	[AmigaKey_Num_8]               = {C64Key_8,         0xff},
	[AmigaKey_Num_9]               = {C64Key_9,         0xff},

	[AmigaKey_Num_Enter]           = {C64Key_Return,    0xff},
	[AmigaKey_Num_Slash]           = {C64Key_Slash,     0xff},
	[AmigaKey_Apostrophe]          = {C64Key_Plus,      0xff},
	[AmigaKey_Num_Minus]           = {C64Key_Minus,     0xff},
	[AmigaKey_Num_Period]          = {C64Key_Period,    0xff},
	[AmigaKey_Num_Asterisk]        = {C64Key_Asterisk,  0xff},
	[AmigaKey_Num_LeftParenthese]  = {C64Key_RShift,    C64Key_8},
	[AmigaKey_Num_RightParenthese] = {C64Key_RShift,    C64Key_9},
};

void updateC64KeyState(uint8_t amigaKey, bool up) {
	if(amigaKey < sizeof(amigaToC64Map) / 2) {
		const uint8_t *c64Keys = amigaToC64Map[amigaKey];
		// Not mapped keys have both entries set to zero. This rules out mapping
		// an AmigaKey to two simultaneous presses of C64Key_Delete, which
		// should be an ok limitation =). 
		if(!(0 == c64Keys[0] && 0 == c64Keys[1])) {
			if(0xff != c64Keys[0])
				c64_keyb_sim_setKey(c64Keys[0], up);
			if(0xff != c64Keys[1])
				c64_keyb_sim_setKey(c64Keys[1], up);
		}
	}
}

void startReset(void) {
	RESET_DDR |= RESET_MASK;
	c64_keyb_sim_resetState();
}

void endReset(void) {
	RESET_DDR &= ~RESET_MASK;
}

int main(void) {
	// Assume we get ~13.5MHz by maxing out the calibration of the internal
	// oscillator. Around 200 (which should be a bit above 10MHz is enough)
	// to get the the very picky 1541 Ultimate II menu to work.
	OSCCAL = 255; 
	RESET_DDR &= ~RESET_MASK; // Input
	RESET_OUT &= ~RESET_MASK; // Output zero whenever set to output

	//uart_init();
	//FILE uart_output = FDEV_SETUP_STREAM(uart_putchar, NULL, _FDEV_SETUP_WRITE);
	//stdout = &uart_output;

	c64_keyb_sim_init();

	amiga_keyb_if_init();
	//amiga_keyb_if_registerChangeCallback(&updateC64KeyState);
	amiga_keyb_if_registerResetStartCallback(&startReset);
	amiga_keyb_if_registerResetEndCallback(&endReset);
	
	//puts("starting!!!");

	//TCNT1 = 0;
	//TCCR1B = _BV(CS11) | _BV(CS11);
	//c64_keyb_sim_setKey(0, false);
	//TCCR1B = 0x00;
	//printf("setKey down: %u\n", TCNT1);
	//
	//TCNT1 = 0;
	//TCCR1B = _BV(CS11) | _BV(CS11);
	//c64_keyb_sim_setKey(0, true);
	//TCCR1B = 0x00;
	//printf("setKey up:   %u\n", TCNT1);

	uint8_t prevAmigaKey = amiga_keyb_if_getKey();
	for(;;) {
		const uint8_t amigaKey = amiga_keyb_if_getKey();
		if (amigaKey != prevAmigaKey) {
			updateC64KeyState(amigaKey & 0x7f, amigaKey & 0x80);
			//printf("%02x\n", amigaKey);
		}
		prevAmigaKey = amigaKey;
	}

    return 0;
}
