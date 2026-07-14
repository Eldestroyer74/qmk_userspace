// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#pragma once
#include QMK_KEYBOARD_H
#include "features/user_keycodes.h"

// Dimmer RGB colors
#define RGB_BSTEEL	55, 80, 115
#define RGB_DPURPLE	60, 0, 115
#define RGB_DPINK	115, 20, 45
#define RGB_DTEAL	5, 35, 35
#define RGB_FLUOR	75, 122, 22
#define RGB_DAMBER	105, 60, 0
#define RGB_DBLUE	10, 25, 95
#define RGB_DGREEN	0, 75, 35
#define RGB_CAPS	RGB_RED
#define RGB_NUM		RGB_DBLUE
#define RGB_SYM		RGB_DAMBER
#define RGB_CMK		RGB_DPURPLE
#define RGB_SYS		RGB_FLUOR
#define RGB_NAV		RGB_CYAN
#define RGB_EXT		RGB_YELLOW
#define RGB_SNP		RGB_WHITE
#define RGB_MED		RGB_DTEAL
#define RGB_TXT		RGB_DPINK
#define RGB_ESP		RGB_DGREEN
#define RGB_BOOT	RGB_DPINK
#define RGB_LAYER	RGB_BSTEEL

// Base RGB mode. ChieftainDots paints Caps/Colemak directly in the indicator
// callback so split state feedback stays simple and predictable.
#define DEF_MODE RGB_MATRIX_NONE

void rgb_matrix_update_pwm_buffers(void);
