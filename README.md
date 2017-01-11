An adapter for connecting Amiga keyboards to the C64.

Some features worth mentioning:
- Can be used simultaneously with the regular C64 keyboard.
- If connected to pin 3 of the C64 user port, it will reset the C64 when pressing Ctrl-Amiga-Amiga.

It works by simulating the keyboard matrix which the C64 reads directly using two of the 8-bit CIA ports. It is assumed that the C64 always uses the 8 COL pins as outputs and the 8 ROW pins as inputs when reading the matrix.

The motivation for creating it was not for using the C64 with real Amiga keyboards, but to be able to control the C64 using synergy with an [avr-amiga-controller](https://github.com/patrikaxelsson/avr-amiga-controller/tree/master/firmware).

It is made to be simple and cheap to build, an Atmel ATMega88, a connector for the Amiga keyboard and some wiring is enough.

Hookup guide:

| ATMega88      | C64 Keyb Port | C64 User Port | Amiga Keyboard |
| ------------- | ------------- | ------------- | -------------- |
| GND           | GND           |               | GND            |
| +5V           | +5V           |               | +5V            |
| PB0           | ROW0          |               |                |
| PB1           | ROW1          |               |                |
| PB2           | ROW2          |               |                |
| PB3           | ROW3          |               |                |
| PB4           | ROW4          |               |                |
| PB5           | ROW5          |               |                |
| PB6           | ROW6          |               |                |
| PB7           | ROW7          |               |                |
| PC0           |               |               | KCLK           |
| PC1           |               |               | KDAT           |
| PC2           | /RESTORE      |               |                |
| PC3           |               | /RESET        |                |
| PD0           | COL0          |               |                |
| PD1           | COL1          |               |                |
| PD2           | COL2          |               |                |
| PD3           | COL3          |               |                |
| PD4           | COL4          |               |                |
| PD5           | COL5          |               |                |
| PD6           | COL6          |               |                |
| PD7           | COL7          |               |                |
