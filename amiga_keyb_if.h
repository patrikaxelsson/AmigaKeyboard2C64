#ifndef __AMIGA_KEYB_IF_H__
#define __AMIGA_KEYB_IF_H__

void amiga_keyb_if_init();
uint8_t amiga_keyb_if_getKey();
void amiga_keyb_if_registerChangeCallback(void (*)(uint8_t, bool));

#endif
