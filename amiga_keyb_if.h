/* amiga_keyb_if, an Amiga keyboard interface.
 * Copyright (C) 2016  Patrik Axelsson, David Eriksson
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 * Also add information on how to contact you by electronic and paper mail.
 */

#ifndef __AMIGA_KEYB_IF_H__
#define __AMIGA_KEYB_IF_H__

void amiga_keyb_if_init(void);
uint8_t amiga_keyb_if_getKey(void);
void amiga_keyb_if_registerChangeCallback(void (*)(const uint8_t amigaKey, const bool up));
void amiga_keyb_if_registerResetStartCallback(void (*newResetStartCallback)(void));
void amiga_keyb_if_registerResetEndCallback(void (*newResetEndCallback)(void));

#endif
