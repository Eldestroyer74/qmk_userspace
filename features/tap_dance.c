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
	uint16_t snap;
} nav_dance_t;

// One pattern powers the small punctuation ladders:
// tap = common character, hold = related alternate, double-tap = rarer pair.
static tap_hold_double_t left_bracket_dance = {KC_LPRN, KC_LBRC, KC_LCBR};
static tap_hold_double_t right_bracket_dance = {KC_RPRN, KC_RBRC, KC_RCBR};
static tap_hold_double_t slash_pipe_dance = {KC_SLSH, KC_BSLS, KC_PIPE};
static tap_hold_double_t plus_equal_dance = {KC_PLUS, KC_EQL, KC_PLUS};
static tap_hold_double_t num_two_comma_lt_dance = {KC_2, KC_COMM, KC_LT};
static tap_hold_double_t num_three_dot_gt_dance = {KC_3, KC_DOT, KC_GT};
static nav_dance_t nav_up_dance = {KC_UP, KC_PGUP, G(KC_UP)};
static nav_dance_t nav_left_dance = {KC_LEFT, KC_HOME, G(KC_LEFT)};
static nav_dance_t nav_down_dance = {KC_DOWN, KC_PGDN, G(KC_DOWN)};
static nav_dance_t nav_right_dance = {KC_RGHT, KC_END, G(KC_RGHT)};

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
		action = "snap";
#endif
		tap_code16(dance->snap);
	} else {
		tap_code16(dance->tap);
		tap_code16(dance->tap);
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
};
