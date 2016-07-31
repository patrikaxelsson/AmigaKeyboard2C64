#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define KCLK PINA0
#define KDAT PINA1

static volatile uint8_t keyboardData = 0xff;

ISR (PCINT0_vect) {
	static uint8_t tmpData = 0x00;
	static int8_t pos = 6;

	if (0 == (PINA & _BV(KCLK))) {
		if (0 == (PINA & _BV(KDAT))) {
			tmpData |= (1 << pos);
		}
		if(7 == pos) {
			DDRA = 0x02;
			PORTA = 0x01;
			_delay_us(85);
			DDRA = 0x00;
			PORTA = 0xff;

			keyboardData = tmpData;
			tmpData = 0x00;
		}
		pos--;
		if (-1 == pos) {
			pos = 7;
		}
	}
}

void amiga_keyboard_init() {
	DDRA = 0x00;
	PORTA = 0xFF; //Pullup
	//DDRA=0xFF; //Knappar (in)
	DDRB = 0xff; //Lysdioder
	PORTB = 0xff;

	PCICR |= _BV(PCIE0);
	PCMSK0 |= _BV(PCINT0);
	sei();
}

uint8_t amiga_keyboard_getkey() {
	return keyboardData;
}
