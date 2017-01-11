An adapter for connecting Amiga keyboards to the C64.

It works by simulating the keyboard matrix which the C64 reads directly using two 8-bit ports. It is assumed that the C64 always uses the 8 COLs pins as outputs and the 8 ROWs pins as inputs.

It is made to be simple and cheap to build, an Atmel ATMega88 and some wiring is enough.

The motivation for creating it was not for using the C64 with real Amiga keyboards, but to be able to control the C64 using synergy and an [avr-amiga-controller](https://github.com/patrikaxelsson/avr-amiga-controller/tree/master/firmware).
