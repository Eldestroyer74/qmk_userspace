// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "tap_dance.h"

static bool tab_esc_close_held;

typedef struct {
	uint16_t tap;
	uint16_t hold;
	uint16_t double_tap;
} tap_hold_double_t;

// One pattern powers the small punctuation ladders:
// tap = common character, hold = related alternate, double-tap = rarer pair.
static tap_hold_double_t left_bracket_dance = {KC_LPRN, KC_LBRC, KC_LCBR};
static tap_hold_double_t right_bracket_dance = {KC_RPRN, KC_RBRC, KC_RCBR};
static tap_hold_double_t slash_pipe_dance = {KC_SLSH, KC_BSLS, KC_PIPE};
static tap_hold_double_t plus_equal_dance = {KC_PLUS, KC_EQL, KC_PLUS};
static tap_hold_double_t num_two_comma_lt_dance = {KC_2, KC_COMM, KC_LT};
static tap_hold_double_t num_three_dot_gt_dance = {KC_3, KC_DOT, KC_GT};

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

static void tab_esc_close_finished(tap_dance_state_t *state, void *user_data) {
	tab_esc_close_held = false;

	// Tab is tap, Esc is hold, and Alt+F4 is double-tap on the same key.
	if (state->count == 1 && state->pressed) {
		register_code16(KC_ESC);
		tab_esc_close_held = true;
	} else if (state->count == 1) {
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
};
