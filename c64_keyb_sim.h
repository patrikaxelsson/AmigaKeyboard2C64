#ifndef __C64_KEYBOARD_SIMULATOR__
#define __C64_KEYBOARD_SIMULATOR__

void c64_keyb_sim_init(void);
void c64_keyb_sim_resetState(void);
void c64_keyb_sim_setKey(const uint8_t c64Key, const bool up);

#endif
