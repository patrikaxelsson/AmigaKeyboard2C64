An adapter for connecting Amiga keyboards to the C64.

Some features worth mentioning:
- Can be used simultaneously with the regular C64 keyboard - just connect it in parallel.
- If connected to /RESET of the C64 user port, it will reset the C64 when pressing Ctrl-Amiga-Amiga.

It works by simulating the keyboard matrix which the C64 reads directly using two of the 8-bit CIA ports. It is assumed that the C64 always uses the 8 COL pins as outputs and the 8 ROW pins as inputs when reading the matrix.

The motivation for creating it was not for using the C64 with real Amiga keyboards, but to be able to control the C64 using synergy with an [avr-amiga-controller](https://github.com/patrikaxelsson/avr-amiga-controller/tree/master/firmware).

It is made to be simple and cheap to build, an Atmel ATMega88, a connector for the Amiga keyboard and some wiring is enough.

Hookup guide:

| ATMega88      | C64 keyb port | C64 user port | Amiga keyboard |
| ------------- | ------------- | ------------- | -------------- |
| GND           | GND       (1) |               | GND            |
| +5V           | +5V       (4) |               | +5V            |
| PB0           | ROW0     (12) |               |                |
| PB1           | ROW1     (11) |               |                |
| PB2           | ROW2     (10) |               |                |
| PB3           | ROW3      (5) |               |                |
| PB4           | ROW4      (8) |               |                |
| PB5           | ROW5      (7) |               |                |
| PB6           | ROW6      (6) |               |                |
| PB7           | ROW7      (9) |               |                |
| PC0           |               |               | KCLK           |
| PC1           |               |               | KDAT           |
| PC2           | /RESTORE  (3) |               |                |
| PC3           |               | /RESET    (3) |                |
| PD0           | COL0     (13) |               |                |
| PD1           | COL1     (19) |               |                |
| PD2           | COL2     (18) |               |                |
| PD3           | COL3     (17) |               |                |
| PD4           | COL4     (16) |               |                |
| PD5           | COL5     (15) |               |                |
| PD6           | COL6     (14) |               |                |
| PD7           | COL7     (20) |               |                |
