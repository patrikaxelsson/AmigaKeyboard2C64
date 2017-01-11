/* c64_keyb_sim, a C64 keyboard simulator.
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

#ifndef __C64_KEYB_CODES_H__
#define __C64_KEYB_CODES_H__

enum C64Key {
	C64Key_0            = 0x43,
	C64Key_1            = 0x70,
	C64Key_2            = 0x73,
	C64Key_3            = 0x10,
	C64Key_4            = 0x13,
	C64Key_5            = 0x20,
	C64Key_6            = 0x23,
	C64Key_7            = 0x30,
	C64Key_8            = 0x33,
	C64Key_9            = 0x40,
	
	C64Key_A            = 0x12,
	C64Key_B            = 0x34,
	C64Key_C            = 0x24,
	C64Key_D            = 0x22,
	C64Key_E            = 0x16,
	C64Key_F            = 0x25,
	C64Key_G            = 0x32,
	C64Key_H            = 0x35,
	C64Key_I            = 0x41,
	C64Key_J            = 0x42,
	C64Key_K            = 0x45,
	C64Key_L            = 0x52,
	C64Key_M            = 0x44,
	C64Key_N            = 0x47,
	C64Key_O            = 0x46,
	C64Key_P            = 0x51,
	C64Key_Q            = 0x76,
	C64Key_R            = 0x21,
	C64Key_S            = 0x15,
	C64Key_T            = 0x26,
	C64Key_U            = 0x36,
	C64Key_V            = 0x37,
	C64Key_W            = 0x11,
	C64Key_X            = 0x27,
	C64Key_Y            = 0x31,
	C64Key_Z            = 0x14,
	
	C64Key_RunStop      = 0x77,
	C64Key_Commodore    = 0x75,
	C64Key_Space        = 0x74,
	C64Key_Control      = 0x72,
	C64Key_LeftArrow    = 0x71,
	C64Key_Slash        = 0x67,
	C64Key_UpArrow      = 0x66,
	C64Key_Equals       = 0x65,
	C64Key_RShift       = 0x64,
	C64Key_ClrHome      = 0x63,
	C64Key_SemiColon    = 0x62,
	C64Key_Asterisk     = 0x61,
	C64Key_Pound        = 0x60,
	C64Key_Comma        = 0x57,
	C64Key_At           = 0x56,
	C64Key_Colon        = 0x55,
	C64Key_Period       = 0x54,
	C64Key_Minus        = 0x53,
	C64Key_Plus         = 0x50,
	C64Key_LShift       = 0x17,
	C64Key_CursDown     = 0x07,
	C64Key_CursRight    = 0x02,
	C64Key_Return       = 0x01,
	C64Key_Delete       = 0x00,
	C64Key_F1           = 0x04,
	C64Key_F3           = 0x05,
	C64Key_F5           = 0x06,
	C64Key_F7           = 0x03,
	// Special key outside regular 8x8 matrix
	C64Key_Restore      = 0xf0,
};

#endif
