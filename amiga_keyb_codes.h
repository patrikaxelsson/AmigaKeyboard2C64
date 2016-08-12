#ifndef __AMIGA_KEYB_CODES_H__
#define __AMIGA_KEYB_CODES_H__

enum AmigaKey {
	AmigaKey_A                      = 0x20,
	AmigaKey_B                      = 0x35,
	AmigaKey_P                      = 0x19,
	AmigaKey_Space                  = 0x40,
	AmigaKey_Return                 = 0x44,

	//Special
	AmigaKey_ResetWarning           = 0x78,
	AmigaKey_LastKeyCodeBad         = 0xf9,
	AmigaKey_OutputBufferOverflow   = 0xfa,
	AmigaKey_SelftestFailed         = 0xfc,
	AmigaKey_InitiatePowerUpStream  = 0xfd,
	AmigaKey_TerminatePowerUpStream = 0xfe,
};

#endif
