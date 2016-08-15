#ifndef __C64_KEYBOARD_SIMULATOR__
#define __C64_KEYBOARD_SIMULATOR__

void c64_keyb_sim_init();
void c64_keyb_sim_resetState(void);
void c64_keyb_sim_setKey(uint8_t c64Key, bool up);

#endif
