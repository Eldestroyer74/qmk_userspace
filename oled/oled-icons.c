// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

/* Graphical active layer and modifier status display.
   Modified from @soundmonster's keymap:
   (keyboards/crkbd/keymaps/soundmonster)

   Usage guide
   1 Place this file next to keymap.c or in userspace.
   2 Add the following lines into rules.mk:
        OLED_ENABLE = yes
        SRC += oled-icons.c
   3 Add the following lines into the main 'oled_task_user()'
     function to render this module:
        extern void render_mod_status(void);
        render_mod_status();
   4 Add the font file reference into `config.h`:
        #define OLED_FONT_H "oledfont.c"
   5 Add your layer numbers for NUM and SYM. Example:
        #define NUM 1
        #define SYM 2
*/

#include QMK_KEYBOARD_H
#include "../features/layers.h"


static void render_logo(uint8_t const layer) {
	static char const corne_logo[] PROGMEM = {
		0x80, 0x81, 0x82, 0x83, 0x84,
		0xa0, 0xa1, 0xa2, 0xa3, 0xa4,
		0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0};
	static char const katakana[] PROGMEM = {
		0x20, 0xd1, 0xd2, 0xd3, 0x20, 0};
	static char const nav_label[] PROGMEM = " nav ";
	static char const styles_label[] PROGMEM = "style";
	static char const media_label[] PROGMEM = "media";
	static char const snap_label[] PROGMEM = "snap ";

	oled_write_P(corne_logo, false);
	switch (layer) {
		case NAV: oled_write_P(nav_label, false); break;
		case EXT: oled_write_P(styles_label, false); break;
		case MED: oled_write_P(media_label, false); break;
		case SNP: oled_write_P(snap_label, false); break;
		default:  oled_write_P(layer <= CMK ? katakana : PSTR("corne"), false);
	}
}


static void render_layer_state(uint8_t const state) {
	static char const base_layer[] PROGMEM = {
		0x20, 0x9a, 0x9b, 0x9c, 0x20,
		0x20, 0xba, 0xbb, 0xbc, 0x20,
		0x20, 0xda, 0xdb, 0xdc, 0x20, 0};
	static char const anchor_layer[] PROGMEM = {
		0x20, 0x94, 0x95, 0x96, 0x20,
		0x20, 0xb4, 0xb5, 0xb6, 0x20,
		0x20, 0xd4, 0xd5, 0xd6, 0x20, 0};
	static char const chord_layer[] PROGMEM = {
		0x20, 0x97, 0x98, 0x99, 0x20,
		0x20, 0xb7, 0xb8, 0xb9, 0x20,
		0x20, 0xd7, 0xd8, 0xd9, 0x20, 0};
	switch(state) {
	case NAV:
	case EXT:
	case MED:
	case SNP: oled_write_P(chord_layer, false); break;
		case NUM:
		case SYM:
		case SYS: oled_write_P(anchor_layer, false); break;
		default:  oled_write_P(base_layer, false);
	}
}

static void render_connector(uint8_t const left, uint8_t const right, uint8_t const bottom) {
	static char const off_off_1[] PROGMEM = {0xc5, 0};
	static char const off_off_2[] PROGMEM = {0xc6, 0};
	static char const on_off_1[]  PROGMEM = {0xc7, 0};
	static char const on_off_2[]  PROGMEM = {0xc8, 0};
	static char const off_on_1[]  PROGMEM = {0xc9, 0};
	static char const off_on_2[]  PROGMEM = {0xca, 0};
	static char const on_on_1[]   PROGMEM = {0xcb, 0};
	static char const on_on_2[]   PROGMEM = {0xcc, 0};

	if (left && right) { oled_write_P(bottom ? on_on_2 : on_on_1, false); }
	else if (left)    { oled_write_P(bottom ? on_off_2 : on_off_1, false); }
	else if (right)   { oled_write_P(bottom ? off_on_2 : off_on_1, false); }
	else              { oled_write_P(bottom ? off_off_2 : off_off_1, false); }
}


static void render_home_pair(char const *left_top, char const *left_bottom, char const *right_top, char const *right_bottom, uint8_t const left_on, uint8_t const right_on) {
	oled_write_P(left_top, false);
	render_connector(left_on, right_on, false);
	oled_write_P(right_top, false);
	oled_write_P(left_bottom, false);
	render_connector(left_on, right_on, true);
	oled_write_P(right_bottom, false);
}


static void render_home_row_status(uint8_t const fn, uint8_t const sym, uint8_t const num, uint8_t const ctrl) {
	static char const fn_off_1[] PROGMEM = {0x85, 0x86, 0};
	static char const fn_off_2[] PROGMEM = {0xa5, 0xa6, 0};
	static char const fn_on_1[]  PROGMEM = {0x8d, 0x8e, 0};
	static char const fn_on_2[]  PROGMEM = {0xad, 0xae, 0};

	static char const sym_off_1[] PROGMEM = {0x87, 0x88, 0};
	static char const sym_off_2[] PROGMEM = {0xa7, 0xa8, 0};
	static char const sym_on_1[]  PROGMEM = {0x8f, 0x90, 0};
	static char const sym_on_2[]  PROGMEM = {0xaf, 0xb0, 0};

	static char const num_off_1[] PROGMEM = {0x89, 0x8a, 0};
	static char const num_off_2[] PROGMEM = {0xa9, 0xaa, 0};
	static char const num_on_1[]  PROGMEM = {0x91, 0x92, 0};
	static char const num_on_2[]  PROGMEM = {0xb1, 0xb2, 0};

	static char const ctrl_off_1[] PROGMEM = {0x8b, 0x8c, 0};
	static char const ctrl_off_2[] PROGMEM = {0xab, 0xac, 0};
	static char const ctrl_on_1[]  PROGMEM = {0xcd, 0xce, 0};
	static char const ctrl_on_2[]  PROGMEM = {0xcf, 0xd0, 0};

	render_home_pair(
		fn ? fn_on_1 : fn_off_1,
		fn ? fn_on_2 : fn_off_2,
		sym ? sym_on_1 : sym_off_1,
		sym ? sym_on_2 : sym_off_2,
		fn,
		sym
	);
	render_home_pair(
		num ? num_on_1 : num_off_1,
		num ? num_on_2 : num_off_2,
		ctrl ? ctrl_on_1 : ctrl_off_1,
		ctrl ? ctrl_on_2 : ctrl_off_2,
		num,
		ctrl
	);
}

// Primary modifier status display function
void render_mod_status(void) {
	uint8_t mods = get_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif
	uint8_t layer = get_highest_layer(layer_state | default_layer_state);
	uint8_t fn    = layer_state_is(SYS) || layer_state_is(MED);
	uint8_t sym   = layer_state_is(SYM) || layer_state_is(EXT);
	uint8_t num   = layer_state_is(NUM) || layer_state_is(NAV);
	uint8_t ctrl  = (mods & MOD_MASK_CTRL) || layer_state_is(SNP);

	render_logo(layer);
	oled_set_cursor(0,6);
	render_layer_state(layer);

	oled_set_cursor(0,11);
	render_home_row_status(fn, sym, num, ctrl);
}
