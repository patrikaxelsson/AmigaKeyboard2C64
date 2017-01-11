An adapter for connecting Amiga keyboards to the C64.

It works by simulating the keyboard matrix which the C64 reads directly using two of the 8-bit CIA ports. It is assumed that the C64 always uses the 8 COL pins as outputs and the 8 ROW pins as inputs when reading the matrix.

It is made to be simple and cheap to build, an Atmel ATMega88 and some wiring is enough.

The motivation for creating it was not for using the C64 with real Amiga keyboards, but to be able to control the C64 using synergy with an [avr-amiga-controller](https://github.com/patrikaxelsson/avr-amiga-controller/tree/master/firmware).

Some features:
- Can be used together, simultaneously with the regular C64 keyboard.
- Will reset the C64 when pressing Ctrl-Amiga-Amiga if connected to pin 3 on the C64 user port.
