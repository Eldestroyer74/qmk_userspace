// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "spanish_compose.h"

static bool compose_armed;

static uint16_t tap_keycode(uint16_t keycode) {
	if (IS_QK_LAYER_TAP(keycode)) {
		return QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
	}
	if (IS_QK_MOD_TAP(keycode)) {
		return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
	}
	return keycode;
}

static bool is_shifted(void) {
	uint8_t mods = get_mods();
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
	send_keyboard_report();
	register_code(KC_LALT);
	for (uint8_t i = 0; code[i]; ++i) {
		tap_windows_alt_digit(code[i]);
	}
	unregister_code(KC_LALT);
	set_mods(mods);
	send_keyboard_report();
}

static bool send_spanish_key(uint16_t keycode) {
	bool shifted = is_shifted();

	switch (tap_keycode(keycode)) {
		case KC_A:
			send_windows_alt_code(shifted ? "0193" : "0225");
			return true;
		case KC_E:
			send_windows_alt_code(shifted ? "0201" : "0233");
			return true;
		case KC_I:
			send_windows_alt_code(shifted ? "0205" : "0237");
			return true;
		case KC_O:
			send_windows_alt_code(shifted ? "0211" : "0243");
			return true;
		case KC_U:
			send_windows_alt_code(shifted ? "0218" : "0250");
			return true;
		case KC_N:
			send_windows_alt_code(shifted ? "0209" : "0241");
			return true;
		case KC_SCLN:
			send_windows_alt_code(shifted ? "0220" : "0252");
			return true;
	}

	return false;
}

bool process_spanish_compose(uint16_t keycode, keyrecord_t *record) {
	if (keycode == ES_COMP) {
		if (record->event.pressed) {
			compose_armed = true;
		}
		return false;
	}

	if (!compose_armed || !record->event.pressed) {
		return true;
	}

	compose_armed = false;
	return !send_spanish_key(keycode);
}
