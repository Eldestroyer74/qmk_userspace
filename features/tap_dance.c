// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "tap_dance.h"

#ifdef CONSOLE_ENABLE
#	include "print.h"
#endif

static bool tab_esc_close_held;

typedef struct {
	uint16_t tap;
	uint16_t hold;
	uint16_t double_tap;
} tap_hold_double_t;

typedef struct {
	uint16_t tap;
	uint16_t hold;
	uint16_t selection;
	uint16_t extreme_selection;
} nav_dance_t;

typedef struct {
	uint8_t layer;
	bool real_gui;
} gui_snap_dance_t;

typedef struct {
	uint8_t source;
} spanish_dance_t;

// One pattern powers the small punctuation ladders:
// tap = common character, hold = related alternate, double-tap = rarer pair.
static tap_hold_double_t left_bracket_dance = {KC_LPRN, KC_LBRC, KC_LCBR};
static tap_hold_double_t right_bracket_dance = {KC_RPRN, KC_RBRC, KC_RCBR};
static tap_hold_double_t slash_pipe_dance = {KC_SLSH, KC_BSLS, KC_PIPE};
static tap_hold_double_t plus_equal_dance = {KC_PLUS, KC_EQL, KC_PLUS};
static tap_hold_double_t num_two_comma_lt_dance = {KC_2, KC_COMM, KC_LT};
static tap_hold_double_t num_three_dot_gt_dance = {KC_3, KC_DOT, KC_GT};
enum {
	GUI_SNAP_NAV_LAYER = 4,
	GUI_SNAP_EXT_LAYER = 5,
	GUI_SNAP_SNP_LAYER = 6,
	GUI_SNAP_MED_LAYER = 7,
	SPANISH_LAYER = 10,
};
enum {
	SPANISH_SOURCE_NONE,
	SPANISH_SOURCE_LEFT,
	SPANISH_SOURCE_RIGHT,
};
// Navigation hierarchy is deliberately data-only for easy rollback:
// tap = arrow, hold = extreme, double-tap = selection, double-tap-hold = extreme selection.
static nav_dance_t nav_up_dance = {KC_UP, KC_PGUP, S(KC_UP), S(KC_PGUP)};
static nav_dance_t nav_left_dance = {KC_LEFT, KC_HOME, C(S(KC_LEFT)), S(KC_HOME)};
static nav_dance_t nav_down_dance = {KC_DOWN, KC_PGDN, S(KC_DOWN), S(KC_PGDN)};
static nav_dance_t nav_right_dance = {KC_RGHT, KC_END, C(S(KC_RGHT)), S(KC_END)};
static gui_snap_dance_t base_gui_snap_dance = {0, true};
static gui_snap_dance_t num_gui_nav_snap_dance = {GUI_SNAP_NAV_LAYER, false};
static gui_snap_dance_t sym_gui_ext_snap_dance = {GUI_SNAP_EXT_LAYER, false};
static gui_snap_dance_t sys_gui_med_snap_dance = {GUI_SNAP_MED_LAYER, false};
static spanish_dance_t spanish_left_dance = {SPANISH_SOURCE_LEFT};
static spanish_dance_t spanish_right_dance = {SPANISH_SOURCE_RIGHT};
static bool gui_snap_layer_held;
static bool gui_snap_gui_held;
bool spanish_language_switch_mode;
uint8_t spanish_language_switch_source;
static bool spanish_layer_held;

static void tap_hold_double_finished(tap_dance_state_t *state, void *user_data) {
	tap_hold_double_t *dance = (tap_hold_double_t *)user_data;

	if (state->count == 1 && state->pressed) {
		tap_code16(dance->hold);
	} else if (state->count == 1) {
		tap_code16(dance->tap);
	} else {
		tap_code16(dance->double_tap);
	}
}

static void nav_dance_finished(tap_dance_state_t *state, void *user_data) {
	nav_dance_t *dance = (nav_dance_t *)user_data;
#ifdef CONSOLE_ENABLE
	const char *action = "repeat";
#endif

	if (state->count == 1 && state->pressed) {
#ifdef CONSOLE_ENABLE
		action = "hold";
#endif
		tap_code16(dance->hold);
	} else if (state->count == 1) {
#ifdef CONSOLE_ENABLE
		action = "tap";
#endif
		tap_code16(dance->tap);
	} else if (state->pressed) {
#ifdef CONSOLE_ENABLE
		action = "extsel";
#endif
		tap_code16(dance->extreme_selection);
	} else {
#ifdef CONSOLE_ENABLE
		action = "select";
#endif
		tap_code16(dance->selection);
	}

#ifdef CONSOLE_ENABLE
	uprintf("nav k=%u a=%s c=%u p=%u m=%u l=%u\n",
	        dance->tap,
	        action,
	        state->count,
	        state->pressed,
	        get_mods(),
	        get_highest_layer(layer_state));
#endif
}

static void gui_snap_finished(tap_dance_state_t *state, void *user_data) {
	gui_snap_dance_t *dance = (gui_snap_dance_t *)user_data;
	gui_snap_layer_held = false;
	gui_snap_gui_held = false;

	if (state->count > 1 && state->pressed) {
		register_code(KC_LGUI);
		layer_on(GUI_SNAP_SNP_LAYER);
		gui_snap_gui_held = true;
		gui_snap_layer_held = true;
	} else if (state->count == 1 && state->pressed) {
		if (dance->real_gui) {
			register_code(KC_LGUI);
			gui_snap_gui_held = true;
		} else {
			layer_on(dance->layer);
			gui_snap_layer_held = true;
		}
	} else if (dance->real_gui) {
		tap_code(KC_LGUI);
	}
}

static void gui_snap_reset(tap_dance_state_t *state, void *user_data) {
	gui_snap_dance_t *dance = (gui_snap_dance_t *)user_data;

	if (gui_snap_gui_held) {
		unregister_code(KC_LGUI);
		gui_snap_gui_held = false;
	}
	if (gui_snap_layer_held) {
		layer_off(state->count > 1 ? GUI_SNAP_SNP_LAYER : dance->layer);
		gui_snap_layer_held = false;
	}
}

static void spanish_dance_finished(tap_dance_state_t *state, void *user_data) {
	spanish_dance_t *dance = (spanish_dance_t *)user_data;
	spanish_layer_held = false;
	spanish_language_switch_mode = false;
	spanish_language_switch_source = SPANISH_SOURCE_NONE;

	if (state->count > 1 && state->pressed) {
		register_code(KC_LGUI);
		spanish_language_switch_mode = true;
		spanish_language_switch_source = dance->source;
	} else if (state->count > 1) {
		tap_code16(G(KC_SPC));
	} else if (state->pressed) {
		layer_on(SPANISH_LAYER);
		spanish_layer_held = true;
	}
}

static void spanish_dance_reset(tap_dance_state_t *state, void *user_data) {
	if (spanish_language_switch_mode) {
		unregister_code(KC_LGUI);
		spanish_language_switch_mode = false;
		spanish_language_switch_source = SPANISH_SOURCE_NONE;
	}
	if (spanish_layer_held) {
		layer_off(SPANISH_LAYER);
		spanish_layer_held = false;
	}
}

static void tab_esc_close_finished(tap_dance_state_t *state, void *user_data) {
	tab_esc_close_held = false;
	uint8_t mods = get_mods();

	// Tab is tap, Esc is hold, and Alt+F4 is double-tap on the same key.
	if (state->count == 1 && state->pressed) {
		register_code16(KC_ESC);
		tab_esc_close_held = true;
	} else if (state->count == 1) {
		tap_code16(KC_TAB);
	} else if (mods & (MOD_MASK_ALT | MOD_MASK_GUI)) {
		tap_code16(KC_TAB);
	} else {
		tap_code16(A(KC_F4));
	}
}

static void tab_esc_close_reset(tap_dance_state_t *state, void *user_data) {
	if (tab_esc_close_held) {
		unregister_code16(KC_ESC);
		tab_esc_close_held = false;
	}
}

tap_dance_action_t tap_dance_actions[] = {
	[TD_TAB_ESC_CLOSE] = ACTION_TAP_DANCE_FN_ADVANCED(NULL, tab_esc_close_finished, tab_esc_close_reset),
	[TD_LEFT_BRACKET] = {
		.fn = {NULL, tap_hold_double_finished, NULL, NULL},
		.user_data = &left_bracket_dance,
	},
	[TD_RIGHT_BRACKET] = {
		.fn = {NULL, tap_hold_double_finished, NULL, NULL},
		.user_data = &right_bracket_dance,
	},
	[TD_SLASH_PIPE] = {
		.fn = {NULL, tap_hold_double_finished, NULL, NULL},
		.user_data = &slash_pipe_dance,
	},
	[TD_PLUS_EQUAL] = {
		.fn = {NULL, tap_hold_double_finished, NULL, NULL},
		.user_data = &plus_equal_dance,
	},
	[TD_NUM_TWO_COMMA_LT] = {
		.fn = {NULL, tap_hold_double_finished, NULL, NULL},
		.user_data = &num_two_comma_lt_dance,
	},
	[TD_NUM_THREE_DOT_GT] = {
		.fn = {NULL, tap_hold_double_finished, NULL, NULL},
		.user_data = &num_three_dot_gt_dance,
	},
	[TD_NAV_UP] = {
		.fn = {NULL, nav_dance_finished, NULL, NULL},
		.user_data = &nav_up_dance,
	},
	[TD_NAV_LEFT] = {
		.fn = {NULL, nav_dance_finished, NULL, NULL},
		.user_data = &nav_left_dance,
	},
	[TD_NAV_DOWN] = {
		.fn = {NULL, nav_dance_finished, NULL, NULL},
		.user_data = &nav_down_dance,
	},
	[TD_NAV_RIGHT] = {
		.fn = {NULL, nav_dance_finished, NULL, NULL},
		.user_data = &nav_right_dance,
	},
	[TD_BASE_GUI_SNAP] = {
		.fn = {NULL, gui_snap_finished, gui_snap_reset, NULL},
		.user_data = &base_gui_snap_dance,
	},
	[TD_NUM_GUI_NAV_SNAP] = {
		.fn = {NULL, gui_snap_finished, gui_snap_reset, NULL},
		.user_data = &num_gui_nav_snap_dance,
	},
	[TD_SYM_GUI_EXT_SNAP] = {
		.fn = {NULL, gui_snap_finished, gui_snap_reset, NULL},
		.user_data = &sym_gui_ext_snap_dance,
	},
	[TD_SYS_GUI_MED_SNAP] = {
		.fn = {NULL, gui_snap_finished, gui_snap_reset, NULL},
		.user_data = &sys_gui_med_snap_dance,
	},
	[TD_SPANISH_LEFT] = {
		.fn = {NULL, spanish_dance_finished, spanish_dance_reset, NULL},
		.user_data = &spanish_left_dance,
	},
	[TD_SPANISH_RIGHT] = {
		.fn = {NULL, spanish_dance_finished, spanish_dance_reset, NULL},
		.user_data = &spanish_right_dance,
	},
};
