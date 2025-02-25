/* Copyright (C) 2021 @filterpaper
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "eldestroyer74.h"


void keyboard_post_init_user(void) {
	rgblight_sethsv_noeeprom(HSV_OFF);
}

void matrix_scan_user(void) {
	if (host_keyboard_led_state().caps_lock) rgblight_sethsv_noeeprom(HSV_PURPLE);
}

layer_state_t layer_state_set_user(layer_state_t state) {
	switch(get_highest_layer(layer_state|default_layer_state)) {
	case FNC:
		rgblight_sethsv_noeeprom(HSV_AZURE);
		break;
	case NAV:
		rgblight_sethsv_noeeprom(HSV_YELLOW);
		break;
	case NUM:
		rgblight_sethsv_noeeprom(HSV_CYAN);
		break;
	case CMK:
		rgblight_sethsv_noeeprom(HSV_PINK);
		break;
	default:
		rgblight_sethsv_noeeprom(HSV_OFF);
	}
	return state;
}
