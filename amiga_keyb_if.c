#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#define KEYB_IN PIND
#define KEYB_OUT PORTD
#define KEYB_DDR DDRD
#define KEYB_CLK PIND2
#define KEYB_DAT PIND3
#define KEYB_MASK (_BV(KEYB_DAT) | _BV(KEYB_CLK))

static volatile uint8_t keyboardData = 0xff;
static void (*changeCallback)(uint8_t, bool) = 0;

ISR (PCINT3_vect) {
	static uint8_t tmpData = 0x00;
	static uint8_t bitPos = 6;

	if (bit_is_clear(KEYB_IN, KEYB_CLK)) {
		tmpData |= ((bit_is_clear(KEYB_IN, KEYB_DAT) ? 1 : 0) << bitPos);
		if(7 == bitPos) {
			keyboardData = tmpData;
			changeCallback(tmpData & 0x7f, tmpData & 0x80);
			tmpData = 0x00;
		}
		bitPos--;
		bitPos &= 7; // Wrap -> 7
	}
	else {
		if (6 == bitPos) {
			KEYB_OUT &= ~_BV(KEYB_DAT); // Disable pull-up and set to zero/low if used for output
			KEYB_DDR |= _BV(KEYB_DAT); // Set to output

			// Let timer0 compare interrupt end handshake in 85us
			TCNT0 = 0;
			OCR0A = 85;
			TIMSK0 |= _BV(OCIE0A);
		}
	}
}

ISR (TIMER0_COMPA_vect) {
	KEYB_DDR &= ~_BV(KEYB_DAT); // Set to input
	KEYB_OUT |= _BV(KEYB_DAT); // Enable pull-up
	TIMSK0 &= ~_BV(OCIE0A);
}

static void dummyChangeCallback(uint8_t amigaKey, bool up) {
}

void amiga_keyb_if_init() {
	changeCallback = dummyChangeCallback;

	KEYB_DDR &= ~KEYB_MASK;
	KEYB_OUT |= KEYB_MASK; //Pullup

	PCICR |= _BV(PCIE3);
	PCMSK3 |= _BV(PCINT26);

	// Setup timer0 for use for handshake wait
	TCCR0A = _BV(1 << WGM01); // Enable output compare mode
	TCCR0B = _BV(1 << CS01); // Set prescaler clk/8 - counts up once per microsecond at 8MHz
	
	sei();
}

uint8_t amiga_keyb_if_getKey() {
	return keyboardData;
}

void amiga_keyb_if_registerChangeCallback(void (*newChangeCallback)(uint8_t, bool)) {
	changeCallback = newChangeCallback;
}
