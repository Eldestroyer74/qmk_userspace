// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "rgb-matrix.h"

extern bool spanish_language_switch_mode;
extern uint8_t spanish_language_switch_source;

// Assign left and right keys to KB2040 LEDs on each side
#ifdef CONVERT_TO_KB2040
led_config_t g_led_config = { {
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	{ 0, 0, 0, 0, 0 }, { 0, 0, 0, 0, 0 },
	{ 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 },
	{ 1, 1, 1, 1, 1 }, { 1, 1, 1, 1, 1 }
}, {
	{111, 48}, {113, 48}
}, {
	255, 255
} };
#endif


static RGB rgb_for_layer(uint8_t layer) {
	switch (layer) {
		case NUM:
			return (RGB){RGB_NUM};
		case SYM:
			return (RGB){RGB_SYM};
		case NAV:
			return (RGB){RGB_NAV};
		case EXT:
			return (RGB){RGB_EXT};
		case SNP:
			return (RGB){RGB_SNP};
		case MED:
			return (RGB){RGB_MED};
		case SYS:
			return (RGB){RGB_SYS};
		case ESP:
			return (RGB){RGB_ESP};
		case CMK:
			return (RGB){RGB_CMK};
		default:
			return (RGB){RGB_LAYER};
	}
}

enum {
	L_TOP,
	L_HOME,
	L_BOTTOM,
	L_THUMB,
	R_TOP,
	R_HOME,
	R_BOTTOM,
	R_THUMB,
};

// These names describe QMK's row/column matrix, not the visual LED order.
// RGB bugs often come from assuming the split wiring matches key positions.
enum {
	L_OUTER,
	L_PINKY,
	L_RING,
	L_MIDDLE,
	L_INDEX,
	L_INNER,
	R_INNER = 5,
	R_INDEX = 4,
	R_MIDDLE = 3,
	R_RING = 2,
	R_PINKY = 1,
	R_OUTER = 0,
	R_THUMB_INNER = 5,
	R_THUMB_MIDDLE = 4,
	R_THUMB_OUTER = 3,
};

static bool is_left_thumb_gui_position(uint8_t row, uint8_t col) {
	return row == L_THUMB && col == L_MIDDLE;
}

static bool is_left_thumb_alt_position(uint8_t row, uint8_t col) {
	return row == L_THUMB && col == L_INDEX;
}

static bool is_left_thumb_space_position(uint8_t row, uint8_t col) {
	return row == L_THUMB && col == L_INNER;
}

static bool is_spanish_hold_position(uint8_t row, uint8_t col) {
	return row == L_BOTTOM && col == L_OUTER;
}

static bool is_right_spanish_hold_position(uint8_t row, uint8_t col) {
	return row == R_BOTTOM && col == R_OUTER;
}

static bool is_navigation_down_position(uint8_t row, uint8_t col) {
	return row == R_HOME && col == R_MIDDLE;
}

static bool is_navigation_up_position(uint8_t row, uint8_t col) {
	return row == R_TOP && col == R_MIDDLE;
}

static bool is_active_spanish_language_source(uint8_t row, uint8_t col) {
	if (spanish_language_switch_source == 1) {
		return is_spanish_hold_position(row, col);
	}
	if (spanish_language_switch_source == 2) {
		return is_right_spanish_hold_position(row, col);
	}
	return false;
}

static RGB rgb_for_thumb_hint(uint8_t layer, uint8_t row, uint8_t col) {
	if (layer == NUM) {
		if (is_left_thumb_gui_position(row, col)) {
			return (RGB){RGB_NAV};
		}
	}
	if (layer == SYM) {
		if (is_left_thumb_gui_position(row, col)) {
			return (RGB){RGB_EXT};
		}
	}
	if (layer == SYS) {
		if (is_left_thumb_gui_position(row, col)) {
			return (RGB){RGB_MED};
		}
	}
	return (RGB){RGB_OFF};
}

static bool is_left_number_anchor_position(uint8_t row, uint8_t col) {
	return row == L_HOME && col == L_MIDDLE;
}

static bool is_left_symbol_anchor_position(uint8_t row, uint8_t col) {
	return row == L_HOME && col == L_RING;
}

static bool is_left_function_anchor_position(uint8_t row, uint8_t col) {
	return row == L_HOME && col == L_PINKY;
}

static bool is_left_control_anchor_position(uint8_t row, uint8_t col) {
	return row == L_HOME && col == L_INDEX;
}

static uint16_t keycode_at_position(uint8_t layer, uint8_t row, uint8_t col) {
	return keymap_key_to_keycode(layer, MAKE_KEYPOS(row, col));
}

static bool is_nontransparent_layer_key(uint16_t keycode) {
	return keycode > KC_TRNS;
}

static bool is_delete_key(uint16_t keycode) {
	return keycode == KC_DEL;
}

static bool is_colemak_toggle_key(uint16_t keycode) {
	return keycode == TG(CMK);
}

static bool rgb_should_light_command_key(uint8_t layer, uint8_t row, uint8_t col, uint16_t keycode) {
	// Most layer RGB follows the keymap itself; explicit rules below add chord anchors and thumb hints.
	if (is_nontransparent_layer_key(keycode)) {
		return true;
	}

	switch (layer) {
		case NAV:
			// Completed anchor+GUI chords keep the anchor and selected thumb lit.
			return is_left_number_anchor_position(row, col) || is_left_thumb_gui_position(row, col);
		case SNP:
			return is_left_control_anchor_position(row, col) || is_left_thumb_gui_position(row, col);
		case EXT:
			return is_left_symbol_anchor_position(row, col) || is_left_thumb_gui_position(row, col);
		case MED:
			return is_left_function_anchor_position(row, col) || is_left_thumb_gui_position(row, col);
		case SYS:
			return is_left_function_anchor_position(row, col) || is_left_thumb_alt_position(row, col);
		case ESP:
			return is_spanish_hold_position(row, col) || is_right_spanish_hold_position(row, col);
		default:
			return false;
	}
}

layer_state_t layer_state_set_user(layer_state_t const state) {
	rgb_matrix_mode_noeeprom(DEF_MODE);
	return state;
}

void keyboard_post_init_user(void) {
	rgb_matrix_mode_noeeprom(DEF_MODE);
}

static RGB rgb_for_position(uint8_t layer, uint8_t row, uint8_t col) {
	RGB rgb = (RGB){RGB_OFF};
	uint16_t keycode = keycode_at_position(layer, row, col);

	if (spanish_language_switch_mode &&
	    (is_active_spanish_language_source(row, col) || is_navigation_up_position(row, col) || is_navigation_down_position(row, col) ||
	     is_left_thumb_space_position(row, col))) {
		return (RGB){RGB_ESP};
	}

	if (layer > CMK && rgb_should_light_command_key(layer, row, col, keycode)) {
		rgb = rgb_for_layer(layer);
		if (layer != NUM && layer != SYM && is_delete_key(keycode)) {
			rgb = (RGB){RGB_CAPS};
		} else if (is_colemak_toggle_key(keycode)) {
			rgb = (RGB){RGB_CMK};
		}
	}

	RGB thumb_rgb = rgb_for_thumb_hint(layer, row, col);
	if (thumb_rgb.r || thumb_rgb.g || thumb_rgb.b) {
		rgb = thumb_rgb;
	}

	return rgb;
}

#ifdef __AVR__

static void rgb_set_key_positions(uint8_t layer) {
	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index_led = g_led_config.matrix_co[row][col];
			RGB rgb = rgb_for_position(layer, row, col);
			if (index_led != NO_LED && (rgb.r || rgb.g || rgb.b)) {
				rgb_matrix_set_color(index_led, rgb.r, rgb.g, rgb.b);
			}
		}
	}
}

bool rgb_matrix_indicators_user(void) {
	uint8_t layer = get_highest_layer(layer_state);
	if (host_keyboard_led_state().caps_lock) {
		rgb_matrix_set_color_all(RGB_CAPS);
	} else if (layer_state_is(CMK)) {
		rgb_matrix_set_color_all(RGB_CMK);
	} else {
		rgb_matrix_set_color_all(RGB_OFF);
	}
	rgb_set_key_positions(layer);
	return false;
}

#else

static void rgb_set_key_positions(uint8_t layer, uint8_t led_min, uint8_t led_max) {
	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index_led = g_led_config.matrix_co[row][col];
			RGB rgb = rgb_for_position(layer, row, col);
			if (index_led != NO_LED && led_min <= index_led && index_led <= led_max &&
			    (rgb.r || rgb.g || rgb.b)) {
				rgb_matrix_set_color(index_led, rgb.r, rgb.g, rgb.b);
			}
		}
	}
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	uint8_t layer = get_highest_layer(layer_state);
	if (host_keyboard_led_state().caps_lock) {
		for (uint8_t i = led_min; i <= led_max; ++i) {
			rgb_matrix_set_color(i, RGB_CAPS);
		}
	} else if (layer_state_is(CMK)) {
		for (uint8_t i = led_min; i <= led_max; ++i) {
			rgb_matrix_set_color(i, RGB_CMK);
		}
	} else {
		for (uint8_t i = led_min; i <= led_max; ++i) {
			rgb_matrix_set_color(i, RGB_OFF);
		}
	}
	rgb_set_key_positions(layer, led_min, led_max);
	return false;
}

#endif // __AVR__
