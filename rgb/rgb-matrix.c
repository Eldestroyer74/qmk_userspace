// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "rgb-matrix.h"
#include "lib/lib8tion/lib8tion.h"

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
		case SYS:
			return (RGB){RGB_SYS};
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

static bool is_delete_position(uint8_t row, uint8_t col) {
	return row == R_TOP && col == R_OUTER;
}

static bool is_direction_position(uint8_t row, uint8_t col) {
	return (row == R_TOP && col == R_MIDDLE) ||
	       (row == R_HOME && (col == R_INDEX || col == R_MIDDLE || col == R_RING));
}

static bool is_left_df_modifier_position(uint8_t row, uint8_t col) {
	return row == L_HOME && (col == L_MIDDLE || col == L_INDEX);
}

static bool is_system_function_position(uint8_t row, uint8_t col) {
	return is_delete_position(row, col) ||
	       (row == R_TOP && (col == R_INNER || col == R_INDEX || col == R_MIDDLE || col == R_RING)) ||
	       (row == R_HOME &&
	        (col == R_INNER || col == R_INDEX || col == R_MIDDLE || col == R_RING || col == R_OUTER)) ||
	       (row == R_BOTTOM && (col == R_INNER || col == R_INDEX || col == R_MIDDLE || col == R_RING));
}

static bool is_number_top_row_position(uint8_t row, uint8_t col) {
	return (row == L_TOP && L_PINKY <= col) || row == R_TOP;
}

static bool is_right_number_pad_position(uint8_t row, uint8_t col) {
	return (row == R_HOME && R_PINKY <= col && col <= R_INDEX) ||
	       (row == R_BOTTOM && R_OUTER <= col && col <= R_INDEX);
}

static bool is_number_symbol_shift_position(uint8_t row, uint8_t col) {
	return row == L_HOME && col == L_INDEX;
}

static bool rgb_should_light_number_key(uint8_t row, uint8_t col) {
	return is_number_top_row_position(row, col) ||
	       is_right_number_pad_position(row, col) ||
	       is_number_symbol_shift_position(row, col);
}

static bool rgb_should_light_symbol_key(uint8_t row, uint8_t col) {
	return is_number_top_row_position(row, col) ||
	       is_right_number_pad_position(row, col) ||
	       (row == L_THUMB && L_MIDDLE <= col);
}

static bool rgb_should_light_command_key(uint8_t layer, uint8_t row, uint8_t col) {
	switch (layer) {
		case NUM:
			return rgb_should_light_number_key(row, col);
		case SYM:
			return rgb_should_light_symbol_key(row, col);
		case NAV:
			return is_direction_position(row, col) || is_left_df_modifier_position(row, col) ||
			       is_delete_position(row, col);
		case SNP:
			return is_direction_position(row, col) || is_delete_position(row, col);
		case EXT:
			return is_direction_position(row, col) || is_left_df_modifier_position(row, col) ||
			       is_delete_position(row, col);
		case MED:
			return is_delete_position(row, col) ||
			       (row == R_TOP && (col == R_INNER || col == R_MIDDLE)) ||
			       (row == R_HOME &&
			        (col == R_INNER || col == R_INDEX || col == R_MIDDLE || col == R_RING));
		case TXT:
			return is_delete_position(row, col) ||
			       (row == R_TOP && (col == R_INNER || col == R_MIDDLE)) ||
			       (row == R_HOME &&
			        (col == R_INNER || col == R_INDEX || col == R_MIDDLE || col == R_RING));
		case SYS:
			return is_system_function_position(row, col);
		default:
			return false;
	}
}

layer_state_t layer_state_set_user(layer_state_t const state) {
	rgb_matrix_mode_noeeprom(DEF_MODE);
	return state;
}

#ifdef __AVR__

static void rgb_set_command_keys(uint8_t layer, RGB rgb) {
	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index_led = g_led_config.matrix_co[row][col];
			if (index_led != NO_LED && rgb_should_light_command_key(layer, row, col)) {
				rgb_matrix_set_color(index_led, rgb.r, rgb.g, rgb.b);
			}
		}
	}
}


bool rgb_matrix_indicators_user(void) {
	if (host_keyboard_led_state().caps_lock) {
		uint8_t pulse = scale8(abs8(sin8(scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8)) - 128) * 2,
		                       rgb_matrix_config.hsv.v);
		rgb_matrix_set_color_all(pulse, 0, 0);
	} else if (layer_state_is(CMK)) {
		rgb_matrix_set_color_all(RGB_CMK);
	} else {
		rgb_matrix_set_color_all(RGB_OFF);
	}
	// Modifier keys
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = 0; i < RGB_MATRIX_LED_COUNT; ++i) {
			if (g_led_config.flags[i] & MOD_FLAG) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Command layers light only the keys that do work on that layer.
	if (get_highest_layer(layer_state) > CMK) {
		uint8_t layer = get_highest_layer(layer_state);
		rgb_set_command_keys(layer, rgb_for_layer(layer));
	}
	return false;
}

#else

static void rgb_set_command_keys(uint8_t layer, RGB rgb, uint8_t led_min, uint8_t led_max) {
	for (uint8_t row = 0; row < MATRIX_ROWS; ++row) {
		for (uint8_t col = 0; col < MATRIX_COLS; ++col) {
			uint8_t index_led = g_led_config.matrix_co[row][col];
			if (index_led != NO_LED && led_min <= index_led && index_led <= led_max &&
			    rgb_should_light_command_key(layer, row, col)) {
				rgb_matrix_set_color(index_led, rgb.r, rgb.g, rgb.b);
			}
		}
	}
}


bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
	if (host_keyboard_led_state().caps_lock) {
		uint8_t pulse = scale8(abs8(sin8(scale16by8(g_rgb_timer, rgb_matrix_config.speed / 8)) - 128) * 2,
		                       rgb_matrix_config.hsv.v);
		for (uint8_t i = led_min; i <= led_max; ++i) {
			rgb_matrix_set_color(i, pulse, 0, 0);
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
	// Modifier keys
	if (get_mods() & MOD_MASK_CSAG) {
		for (uint8_t i = led_min; i <= led_max; ++i) {
			if (g_led_config.flags[i] & MOD_FLAG) {
				rgb_matrix_set_color(i, RGB_MODS);
			}
		}
	}
	// Command layers light only the keys that do work on that layer.
	if (get_highest_layer(layer_state) > CMK) {
		uint8_t layer = get_highest_layer(layer_state);
		rgb_set_command_keys(layer, rgb_for_layer(layer), led_min, led_max);
	}
	return false;
}

#endif // __AVR__
