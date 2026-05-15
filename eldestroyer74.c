// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "eldestroyer74.h"
#include "features/text_stubs.h"

#if (defined TAPPING_TERM_PER_KEY || defined PERMISSIVE_HOLD_PER_KEY)
static uint_fast16_t tap_timer = 0;
#	define IS_TYPING() (timer_elapsed(tap_timer) < TAPPING_TERM * 1.3)
#endif


#ifdef TAPPING_TERM_PER_KEY
// Increase tapping term in between short key presses to avoid false trigger.
uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
	return IS_HOME_ROW() && IS_TYPING() ? TAPPING_TERM * 1.3 : TAPPING_TERM;
}
#endif


#ifdef QUICK_TAP_TERM_PER_KEY
// Reduce quick tap term for thumb keys
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
	return IS_THUMB_ROW() ? QUICK_TAP_TERM * 0.5 : QUICK_TAP_TERM;
}
#endif


#ifdef FLOW_TAP_TERM
uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
	if (keycode == HM_F || keycode == HM_J) {
		return 0;
	}
	return is_flow_tap_key(keycode) && is_flow_tap_key(prev_keycode) ? FLOW_TAP_TERM : 0;
}
#endif


#ifdef PERMISSIVE_HOLD_PER_KEY
// Select Shift mod tap immediately when another key is pressed and released.
bool get_permissive_hold(uint16_t keycode, keyrecord_t *record) {
	return MODTAP_BIT(keycode) & MOD_MASK_SHIFT && !IS_TYPING() ? true : false;
}
#endif


#ifdef HOLD_ON_OTHER_KEY_PRESS_PER_KEY
static inline bool is_eager_hold_key(uint16_t keycode) {
	return keycode == HM_F || keycode == HM_J;
}

// Shift participates in typing, so it gets eager-hold behavior. Layers are
// command modes, so they stay deliberate and rely on tapping term, Flow Tap, and
// Chordal Hold instead.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return is_eager_hold_key(keycode) && !IS_TYPING();
}
#endif


bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
	switch (keycode) {
		case TXT_EMAIL:
			if (record->event.pressed) {
				SEND_STRING(TEXT_STUB_EMAIL);
			}
			return false;
		case TXT_PHONE:
			if (record->event.pressed) {
				SEND_STRING(TEXT_STUB_PHONE);
			}
			return false;
		case TXT_NAME:
			if (record->event.pressed) {
				SEND_STRING(TEXT_STUB_NAME);
			}
			return false;
		case TXT_MEET:
			if (record->event.pressed) {
				SEND_STRING(TEXT_STUB_MEETING);
			}
			return false;
		case TXT_WORK:
			if (record->event.pressed) {
				SEND_STRING(TEXT_STUB_WORK);
			}
			return false;
		case TXT_HOME:
			if (record->event.pressed) {
				SEND_STRING(TEXT_STUB_HOME);
			}
			return false;
	}

	if (record->event.pressed) {
#if (defined TAPPING_TERM_PER_KEY || defined PERMISSIVE_HOLD_PER_KEY)
		tap_timer = timer_read();
#endif
#if (defined OLED_ENABLE && !defined WPM_ENABLE)
		extern uint32_t oled_tap_timer;
		oled_tap_timer = timer_read32();
#endif
#ifdef AUTO_CORRECT
		extern bool process_autocorrect(uint16_t keycode, keyrecord_t* record);
		if (!process_autocorrect(keycode, record)) {
			return false;
		}
#endif
#ifdef CAPS_UNLOCK
		extern bool process_caps_unlock(uint16_t keycode, keyrecord_t *record);
		if (!process_caps_unlock(keycode, record)) {
			return false;
		}
#endif
	}
	return true;
}
