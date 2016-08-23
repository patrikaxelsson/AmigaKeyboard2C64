#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

#include "amiga_keyb_codes.h"

#if defined (__AVR_ATmega324P__)
	#define KEYB_IN PIND
	#define KEYB_OUT PORTD
	#define KEYB_DDR DDRD
	#define KEYB_CLK PIND2
	#define KEYB_DAT PIND3
	#define KEYB_MASK (_BV(KEYB_DAT) | _BV(KEYB_CLK))
	#define KEYB_CHANGE_VECT PCINT3_vect
	#define KEYB_CHANGE_PORTMASK _BV(PCIE3)
	#define KEYB_CHANGE_MASKREG PCMSK3
	#define KEYB_CHANGE_MASK _BV(PCINT26) 
#elif defined (__AVR_ATmega88__)
	#define KEYB_IN PINC
	#define KEYB_OUT PORTC
	#define KEYB_DDR DDRC
	#define KEYB_CLK PINC0
	#define KEYB_DAT PINC1
	#define KEYB_MASK (_BV(KEYB_DAT) | _BV(KEYB_CLK))
	#define KEYB_CHANGE_VECT PCINT1_vect
	#define KEYB_CHANGE_PORTMASK _BV(PCIE1)
	#define KEYB_CHANGE_MASKREG PCMSK1
	#define KEYB_CHANGE_MASK _BV(PCINT8) 
#endif

static volatile uint8_t keyboardData = 0xff;
static void (*changeCallback)(const uint8_t amigaKey, const bool up);
static void (*resetStartCallback)(void);
static void (*resetEndCallback)(void);

static enum State {
	State_Data,
	State_Handshake,
	State_Reset,
} state = State_Data;

static void initChangeInterrupt(void) {
	PCICR |= KEYB_CHANGE_PORTMASK;
	KEYB_CHANGE_MASKREG |= KEYB_CHANGE_MASK;
}

// As macros to get overflow warning
#define TIMER_MICROS_CYCLES(microSeconds, divisor) ((microSeconds * (uint64_t) F_CPU) / (divisor * 1000000LL))
#define TIMER_MILLIS_CYCLES(milliSeconds, divisor) ((milliSeconds * (uint64_t) F_CPU) / (divisor * 1000L))

static inline void startHandshakeEndTimer(void) {
	// Set prescaler to clk/8 - counts up once per microsecond at 8MHz
	TCCR0B = _BV(CS01);
	// Set prescaler to clk/64
	//TCCR0B = _BV(CS01) | _BV(CS00);
}
static inline void stopHandshakeEndTimer(void) {
	TCCR0B = 0x00;
	TCNT0 = 0;
}
static void initHandshakeEndTimer(void) {
	TCCR0A = _BV(WGM01); // Enable output compare mode
	stopHandshakeEndTimer();
	OCR0A = TIMER_MICROS_CYCLES(85, 8);
	TIMSK0 |= _BV(OCIE0A);
}

static inline void startResetTimer(void) {
	// Set prescaler to clk/1024
	//TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);
	TCCR2B = _BV(CS22) | _BV(CS21) | _BV(CS20);
}
static inline void stopResetTimer(void) {
	//TCCR1B = 0x00;
	//TCNT1 = 0;
	TCCR2B = 0x00;
	TCNT2 = 0;
}

static void initResetTimer(void) {
	//TCCR1A = 0x00; // Enable output compare mode
	TCCR2A = _BV(WGM21); // Enable output compare mode
	stopResetTimer();
	// If KEYB_CLK is kept low more than 16ms, we assume the keyboard is in hard reset mode
	OCR2A = TIMER_MILLIS_CYCLES(16, 1024);
	//TIMSK1 |= _BV(OCIE1A);
	TIMSK2 |= _BV(OCIE2A);
}

ISR (KEYB_CHANGE_VECT) {
	static uint8_t bitPos = 6;
	static uint8_t tmpData = 0x00;

	if(bit_is_clear(KEYB_IN, KEYB_CLK)) {
		startResetTimer();

		if(State_Data == state) {
			tmpData |= ((bit_is_clear(KEYB_IN, KEYB_DAT) ? 1 : 0) << bitPos);
			if(7 == bitPos) {
				keyboardData = tmpData;
				//changeCallback(tmpData & 0x7f, tmpData & 0x80);
				tmpData = 0x00;

				// Start handshake by sinking KEYB_DAT
				KEYB_OUT &= ~_BV(KEYB_DAT); // Disable pull-up and set to zero/low if used for output
				KEYB_DDR |= _BV(KEYB_DAT); // Set to output
				state = State_Handshake;
				startHandshakeEndTimer();
			}
			bitPos--;
			bitPos &= 7; // Wrap -> 7
		}
	}
	else {
		stopResetTimer();
	
		if(State_Reset == state) {
			resetEndCallback();
			state = State_Data;
		}
	}
}

ISR (TIMER0_COMPA_vect) {
	stopHandshakeEndTimer();

	// End handshake by releasing KEYB_DAT
	KEYB_DDR &= ~_BV(KEYB_DAT); // Set to input
	KEYB_OUT |= _BV(KEYB_DAT); // Enable pull-up
	state = State_Data;
}

//ISR (TIMER1_COMPA_vect) {
ISR (TIMER2_COMPA_vect) {
	stopResetTimer();
	state = State_Reset;
	resetStartCallback();
}

static void dummyChangeCallback(const uint8_t amigaKey, const bool up) {
}

static void dummyResetStartCallback(void) {
}

static void dummyResetEndCallback(void) {
}

void amiga_keyb_if_init() {
	changeCallback = &dummyChangeCallback;
	resetStartCallback = &dummyResetStartCallback;
	resetEndCallback = &dummyResetEndCallback;

	KEYB_DDR &= ~KEYB_MASK;
	KEYB_OUT |= KEYB_MASK; //Pullup

	initHandshakeEndTimer();
	initResetTimer();
	initChangeInterrupt();

	sei();
}

uint8_t amiga_keyb_if_getKey() {
	return keyboardData;
}

void amiga_keyb_if_registerChangeCallback(void (*newChangeCallback)(const uint8_t amigaKey, const bool up)) {
	changeCallback = newChangeCallback;
}

void amiga_keyb_if_registerResetStartCallback(void (*newResetStartCallback)(void)) {
	resetStartCallback = newResetStartCallback;
}

void amiga_keyb_if_registerResetEndCallback(void (*newResetEndCallback)(void)) {
	resetEndCallback = newResetEndCallback;
}
