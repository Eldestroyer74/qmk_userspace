// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H

extern void render_bongocat(void);
extern void render_mod_status(void);

oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
	if (is_keyboard_master()) {
		return is_keyboard_left() ? rotation : OLED_ROTATION_180;
	}
	return OLED_ROTATION_270;
}

bool oled_task_user(void) {
	if (is_keyboard_master()) {
		render_bongocat();
	} else {
		render_mod_status();
	}
	return false;
}
