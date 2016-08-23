#ifndef __AMIGA_KEYB_IF_H__
#define __AMIGA_KEYB_IF_H__

void amiga_keyb_if_init(void);
uint8_t amiga_keyb_if_getKey(void);
void amiga_keyb_if_registerChangeCallback(void (*)(const uint8_t amigaKey, const bool up));
void amiga_keyb_if_registerResetStartCallback(void (*newResetStartCallback)(void));
void amiga_keyb_if_registerResetEndCallback(void (*newResetEndCallback)(void));

#endif
