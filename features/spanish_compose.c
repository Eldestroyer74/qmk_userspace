// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "spanish_compose.h"

static bool is_shifted(void) {
	uint8_t mods = get_mods() | get_weak_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif
	return (mods & MOD_MASK_SHIFT) || host_keyboard_led_state().caps_lock;
}

static void tap_windows_alt_digit(char digit) {
	tap_code(KC_KP_1 + (10 + digit - '1') % 10);
}

static void send_windows_alt_code(const char *code) {
	uint8_t mods = get_mods();

	clear_mods();
	clear_weak_mods();
	send_keyboard_report();
	register_code(KC_LALT);
	for (uint8_t i = 0; code[i]; ++i) {
		tap_windows_alt_digit(code[i]);
	}
	unregister_code(KC_LALT);
	set_mods(mods);
	send_keyboard_report();
}

bool process_spanish_compose(uint16_t keycode, keyrecord_t *record) {
	if (!record->event.pressed) {
		return true;
	}

	bool shifted = is_shifted();

	switch (keycode) {
		case ES_A:
			send_windows_alt_code(shifted ? "0193" : "0225");
			return false;
		case ES_E:
			send_windows_alt_code(shifted ? "0201" : "0233");
			return false;
		case ES_I:
			send_windows_alt_code(shifted ? "0205" : "0237");
			return false;
		case ES_O:
			send_windows_alt_code(shifted ? "0211" : "0243");
			return false;
		case ES_U:
			send_windows_alt_code(shifted ? "0218" : "0250");
			return false;
		case ES_N:
			send_windows_alt_code(shifted ? "0209" : "0241");
			return false;
		case ES_UDIA:
			send_windows_alt_code(shifted ? "0220" : "0252");
			return false;
		case ES_IQUE:
			send_windows_alt_code("0191");
			return false;
		case ES_IEXL:
			send_windows_alt_code("0161");
			return false;
	}

	return true;
}
