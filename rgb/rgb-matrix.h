// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H

// Dimmer RGB colors
#define RGB_BSTEEL	55, 80, 115
#define RGB_DPURPLE	60, 0, 115
#define RGB_DPINK	115, 20, 45
#define RGB_DTEAL	5, 35, 35
#define RGB_FLUOR	75, 122, 22
#define RGB_CAPS	RGB_RED
#define RGB_MODS	RGB_GREEN
#define RGB_NUM		RGB_BLUE
#define RGB_SYM		RGB_CYAN
#define RGB_CMK		RGB_DPURPLE
#define RGB_SYS		RGB_FLUOR
#define RGB_LAYER	RGB_BSTEEL

// Base RGB mode. ChieftainDots paints Caps/Colemak directly in the indicator
// callback so split state feedback stays simple and predictable.
#define DEF_MODE RGB_MATRIX_NONE

// Custom indicators
#ifdef KEYBOARD_crkbd_rev1
#	define MOD_FLAG LED_FLAG_UNDERGLOW
#	define CAP_FLAG LED_FLAG_UNDERGLOW
#else
#	define MOD_FLAG LED_FLAG_MODIFIER
#	define CAP_FLAG LED_FLAG_KEYLIGHT
#endif
