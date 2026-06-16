// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "tap_dance.h"
#include "layers.h"

#ifdef CONSOLE_ENABLE
#	include "print.h"
#endif

static bool tab_esc_close_held;
bool chieftaindots_long_caps_mode;
extern void send_windows_alt_code(const char *code);

typedef struct {
	uint16_t tap;
	uint16_t hold;
	uint16_t double_tap;
} tap_hold_double_t;

typedef struct {
	uint16_t tap;
	uint16_t hold;
	uint16_t double_tap;
	uint16_t double_hold;
	bool held;
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
// Navigation is movement-only: tap arrows, hold arrows, double-tap semantic
// jumps, and double-tap-hold extremes.
static nav_dance_t nav_up_dance = {KC_UP, KC_UP, C(KC_UP), KC_PGUP};
static nav_dance_t nav_left_dance = {KC_LEFT, KC_LEFT, C(KC_LEFT), KC_HOME};
static nav_dance_t nav_down_dance = {KC_DOWN, KC_DOWN, C(KC_DOWN), KC_PGDN};
static nav_dance_t nav_right_dance = {KC_RGHT, KC_RGHT, C(KC_RGHT), KC_END};
static gui_snap_dance_t base_gui_snap_dance = {0, true};
static gui_snap_dance_t num_gui_nav_snap_dance = {NAV, false};
static gui_snap_dance_t sym_gui_ext_snap_dance = {EXT, false};
static gui_snap_dance_t sys_gui_med_snap_dance = {MED, false};
static spanish_dance_t spanish_left_dance = {SPANISH_SOURCE_LEFT};
static spanish_dance_t spanish_right_dance = {SPANISH_SOURCE_RIGHT};
static bool gui_snap_layer_held;
static bool gui_snap_gui_held;
static uint8_t gui_snap_active_layer;
uint8_t chieftaindots_snap_mode;
bool spanish_language_switch_mode;
uint8_t spanish_language_switch_source;
static bool spanish_layer_held;

enum {
	SNAP_LEFT_CTRL_ROW = 1,
	SNAP_LEFT_CTRL_COL = 4,
	SNAP_RIGHT_CTRL_ROW = 5,
	SNAP_RIGHT_CTRL_COL = 4,
};

static bool snap_ctrl_source_held(void) {
	return matrix_is_on(SNAP_LEFT_CTRL_ROW, SNAP_LEFT_CTRL_COL) ||
	       matrix_is_on(SNAP_RIGHT_CTRL_ROW, SNAP_RIGHT_CTRL_COL);
}

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
	dance->held = false;
#ifdef CONSOLE_ENABLE
	const char *action = "repeat";
#endif

	if (state->count == 1 && state->pressed) {
#ifdef CONSOLE_ENABLE
		action = "hold";
#endif
		register_code16(dance->hold);
		dance->held = true;
	} else if (state->count == 1) {
#ifdef CONSOLE_ENABLE
		action = "tap";
#endif
		tap_code16(dance->tap);
	} else if (state->pressed) {
#ifdef CONSOLE_ENABLE
		action = "extreme";
#endif
		tap_code16(dance->double_hold);
	} else {
#ifdef CONSOLE_ENABLE
		action = "jump";
#endif
		tap_code16(dance->double_tap);
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

static void nav_dance_reset(tap_dance_state_t *state, void *user_data) {
	nav_dance_t *dance = (nav_dance_t *)user_data;

	if (dance->held) {
		unregister_code16(dance->hold);
		dance->held = false;
	}
}

static void gui_snap_finished(tap_dance_state_t *state, void *user_data) {
	gui_snap_dance_t *dance = (gui_snap_dance_t *)user_data;
	gui_snap_layer_held = false;
	gui_snap_gui_held = false;
	gui_snap_active_layer = 0;
	chieftaindots_snap_mode = SNAP_MODE_NONE;
	uint8_t mods = get_mods();
	uint8_t weak_mods = get_weak_mods();
	bool ctrl_down = ((mods | weak_mods) & MOD_MASK_CTRL) || snap_ctrl_source_held();

	if (state->count > 1 && state->pressed) {
		layer_on(SNP);
		gui_snap_layer_held = true;
		gui_snap_active_layer = SNP;
		chieftaindots_snap_mode = SNAP_MODE_DESKTOP;
	} else if (state->count == 1 && state->pressed) {
		if (ctrl_down) {
			layer_on(SNP);
			gui_snap_layer_held = true;
			gui_snap_active_layer = SNP;
			chieftaindots_snap_mode = SNAP_MODE_WINDOW;
		} else if (dance->real_gui) {
			register_code(KC_LGUI);
			gui_snap_gui_held = true;
		} else {
			layer_on(dance->layer);
			gui_snap_layer_held = true;
			gui_snap_active_layer = dance->layer;
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
		layer_off(gui_snap_active_layer ? gui_snap_active_layer : (state->count > 1 ? SNP : dance->layer));
		gui_snap_layer_held = false;
	}
	chieftaindots_snap_mode = SNAP_MODE_NONE;
	gui_snap_active_layer = 0;
}

static void spanish_dance_finished(tap_dance_state_t *state, void *user_data) {
	spanish_dance_t *dance = (spanish_dance_t *)user_data;
	spanish_layer_held = false;
	spanish_language_switch_mode = false;
	spanish_language_switch_source = SPANISH_SOURCE_NONE;

	if (state->count > 1 && state->pressed) {
		register_code(KC_LGUI);
		if (dance->source == SPANISH_SOURCE_LEFT) {
			tap_code16(S(KC_SPC));
		} else {
			tap_code(KC_SPC);
		}
		spanish_language_switch_mode = true;
		spanish_language_switch_source = dance->source;
	} else if (state->count > 1) {
		tap_code16(G(KC_SPC));
	} else if (state->pressed) {
		layer_on(ESP);
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
		layer_off(ESP);
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

static bool has_shift_modifier(void) {
	uint8_t mods = get_mods() | get_weak_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif
	return mods & MOD_MASK_SHIFT;
}

static void caps_long_finished(tap_dance_state_t *state, void *user_data) {
	if (state->count > 1) {
		chieftaindots_long_caps_mode = !host_keyboard_led_state().caps_lock;
		tap_code(KC_CAPS);
		return;
	}

	if (has_shift_modifier()) {
		send_windows_alt_code("0147");
	} else {
		chieftaindots_long_caps_mode = false;
		tap_code(KC_CAPS);
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
		.fn = {NULL, nav_dance_finished, nav_dance_reset, NULL},
		.user_data = &nav_up_dance,
	},
	[TD_NAV_LEFT] = {
		.fn = {NULL, nav_dance_finished, nav_dance_reset, NULL},
		.user_data = &nav_left_dance,
	},
	[TD_NAV_DOWN] = {
		.fn = {NULL, nav_dance_finished, nav_dance_reset, NULL},
		.user_data = &nav_down_dance,
	},
	[TD_NAV_RIGHT] = {
		.fn = {NULL, nav_dance_finished, nav_dance_reset, NULL},
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
	[TD_CAPS_LONG] = ACTION_TAP_DANCE_FN(caps_long_finished),
};
