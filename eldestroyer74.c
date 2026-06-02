// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "eldestroyer74.h"
#include "features/spanish_compose.h"
#include "features/text_stubs.h"

#ifdef CONSOLE_ENABLE
#	include "print.h"
#endif

extern bool process_spanish_compose(uint16_t keycode, keyrecord_t *record);

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

// Keep hold-on-other-key-press disabled for typing keys. Shift mod-taps rely on
// permissive hold, Flow Tap, and Chordal Hold so ordinary rolls like "fo" do not
// become accidental shifted letters.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
	return false;
}
#endif

#ifdef CONSOLE_ENABLE
static uint16_t shift_trial_keycode = KC_NO;
static uint16_t shift_trial_timer = 0;
static bool shift_trial_pending;
static uint16_t shift_trial_next = KC_NO;
static uint16_t shift_trial_dt;
static uint8_t shift_trial_mods;
static uint8_t shift_trial_tap;
static char shift_trial_word[13];
static uint8_t shift_trial_word_len;
static bool shift_trial_word_safe = true;
#endif

enum auto_caps_trial_state {
	AUTO_CAPS_IDLE,
	AUTO_CAPS_WORD,
	AUTO_CAPS_WAIT_SPACE,
	AUTO_CAPS_WAIT_ALPHA,
	AUTO_CAPS_LOG_WORD,
};

static enum auto_caps_trial_state auto_caps_trial_state = AUTO_CAPS_IDLE;
static uint8_t auto_caps_trial_punct;
static uint16_t auto_caps_trial_next = KC_NO;
static uint8_t auto_caps_trial_mods;
static bool auto_caps_trial_applied;
static char auto_caps_trial_word[13];
static uint8_t auto_caps_trial_word_len;
static bool auto_caps_trial_word_safe = true;
static char auto_caps_context_word[9];
static uint8_t auto_caps_context_word_len;
static bool auto_caps_context_word_safe = true;

static uint16_t base_tap_keycode(uint16_t keycode) {
	if (IS_QK_LAYER_TAP(keycode)) {
		return QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
	}
	if (IS_QK_MOD_TAP(keycode)) {
		return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
	}
	return keycode;
}

static bool is_shift_trial_alpha(uint16_t keycode) {
	return KC_A <= keycode && keycode <= KC_Z;
}

static bool is_spanish_trial_alpha(uint16_t keycode) {
	return keycode == ES_A || keycode == ES_E || keycode == ES_I || keycode == ES_O || keycode == ES_U || keycode == ES_N ||
	       keycode == ES_UDIA;
}

static bool is_auto_caps_trial_alpha(uint16_t keycode) {
	return is_shift_trial_alpha(keycode) || is_spanish_trial_alpha(keycode);
}

#ifdef CONSOLE_ENABLE
static bool is_shift_trial_candidate(uint16_t keycode) {
	return is_shift_trial_alpha(keycode) || keycode == KC_SLSH;
}
#endif

static char shift_trial_char(uint16_t keycode) {
	return 'a' + (keycode - KC_A);
}

static char auto_caps_trial_char(uint16_t keycode) {
	if (is_shift_trial_alpha(keycode)) {
		return shift_trial_char(keycode);
	}
	switch (keycode) {
		case ES_A:
			return 'a';
		case ES_E:
			return 'e';
		case ES_I:
			return 'i';
		case ES_O:
			return 'o';
		case ES_U:
		case ES_UDIA:
			return 'u';
		case ES_N:
			return 'n';
	}
	return '\0';
}

static bool is_spanish_opening_punctuation(uint16_t keycode) {
	return keycode == ES_IQUE || keycode == ES_IEXL;
}

static bool is_shift_trial_boundary(uint16_t keycode) {
	return keycode == KC_SPC || keycode == KC_ENT || keycode == KC_DOT || keycode == KC_COMM ||
	       keycode == KC_SCLN || keycode == KC_QUOT || keycode == KC_SLSH || keycode == KC_MINS;
}

static uint8_t shift_trial_punctuation(uint16_t keycode, uint8_t mods) {
	if (IS_QK_MODS(keycode)) {
		mods |= QK_MODS_GET_MODS(keycode);
		keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
	}
	if (keycode == KC_DOT && !(mods & MOD_MASK_SHIFT)) {
		return '.';
	}
	if (keycode == KC_SLSH && (mods & MOD_MASK_SHIFT)) {
		return '?';
	}
	if (keycode == SLASH_PIPE && (mods & MOD_MASK_SHIFT)) {
		return '?';
	}
	if (keycode == KC_QUES) {
		return '?';
	}
	if (keycode == KC_1 && ((mods & MOD_MASK_SHIFT) || layer_state_is(SYM))) {
		return '!';
	}
	if (keycode == KC_EXLM) {
		return '!';
	}
	if (keycode == ES_IQUE) {
		return 191;
	}
	if (keycode == ES_IEXL) {
		return 161;
	}
	return 0;
}

#ifdef CONSOLE_ENABLE
static void shift_trial_reset_word(void) {
	shift_trial_word[0] = '\0';
	shift_trial_word_len = 0;
	shift_trial_word_safe = true;
}

static void shift_trial_append_word(uint16_t keycode) {
	if (!shift_trial_word_safe) {
		return;
	}
	if (shift_trial_word_len >= sizeof(shift_trial_word) - 1) {
		shift_trial_word_safe = false;
		shift_trial_word[0] = '\0';
		return;
	}
	shift_trial_word[shift_trial_word_len++] = shift_trial_char(keycode);
	shift_trial_word[shift_trial_word_len] = '\0';
}

static void shift_trial_flush(void) {
	if (shift_trial_pending) {
		uprintf("shift_trial prev=%u next=%u dt=%u mods=%u tap=%u word=%s\n",
		        shift_trial_keycode,
		        shift_trial_next,
		        shift_trial_dt,
		        shift_trial_mods,
		        shift_trial_tap,
		        (!shift_trial_mods && shift_trial_word_safe && shift_trial_word_len) ? shift_trial_word : "omitted");
	}
	shift_trial_pending = false;
	shift_trial_keycode = KC_NO;
	shift_trial_next = KC_NO;
	shift_trial_reset_word();
}
#endif

static void auto_caps_trial_reset_word(void) {
	auto_caps_trial_word[0] = '\0';
	auto_caps_trial_word_len = 0;
	auto_caps_trial_word_safe = true;
}

static void auto_caps_context_reset_word(void) {
	auto_caps_context_word[0] = '\0';
	auto_caps_context_word_len = 0;
	auto_caps_context_word_safe = true;
}

static void auto_caps_trial_append_word(uint16_t keycode) {
	if (!auto_caps_trial_word_safe) {
		return;
	}
	if (auto_caps_trial_word_len >= sizeof(auto_caps_trial_word) - 1) {
		auto_caps_trial_word_safe = false;
		auto_caps_trial_word[0] = '\0';
		return;
	}
	char c = auto_caps_trial_char(keycode);
	if (!c) {
		auto_caps_trial_word_safe = false;
		auto_caps_trial_word[0] = '\0';
		return;
	}
	auto_caps_trial_word[auto_caps_trial_word_len++] = c;
	auto_caps_trial_word[auto_caps_trial_word_len] = '\0';
}

static void auto_caps_context_append_word(uint16_t keycode) {
	if (!auto_caps_context_word_safe) {
		return;
	}
	if (auto_caps_context_word_len >= sizeof(auto_caps_context_word) - 1) {
		auto_caps_context_word_safe = false;
		auto_caps_context_word[0] = '\0';
		return;
	}
	char c = auto_caps_trial_char(keycode);
	if (!c) {
		auto_caps_context_word_safe = false;
		auto_caps_context_word[0] = '\0';
		return;
	}
	auto_caps_context_word[auto_caps_context_word_len++] = c;
	auto_caps_context_word[auto_caps_context_word_len] = '\0';
}

static bool auto_caps_context_is_exception(void) {
	return auto_caps_context_word_safe &&
	       ((auto_caps_context_word_len == 2 && auto_caps_context_word[0] == 'v' && auto_caps_context_word[1] == 's') ||
	        (auto_caps_context_word_len == 3 && auto_caps_context_word[0] == 'e' && auto_caps_context_word[1] == 't' &&
	         auto_caps_context_word[2] == 'c'));
}

static void auto_caps_trial_flush(void) {
	if (auto_caps_trial_state == AUTO_CAPS_LOG_WORD) {
#ifdef CONSOLE_ENABLE
		uprintf("autocaps_trial punct=%u next=%u mods=%u auto=%u word=%s\n",
		        auto_caps_trial_punct,
		        auto_caps_trial_next,
		        auto_caps_trial_mods,
		        auto_caps_trial_applied,
		        (auto_caps_trial_word_safe && auto_caps_trial_word_len) ? auto_caps_trial_word : "omitted");
#endif
	}
	auto_caps_trial_state = AUTO_CAPS_IDLE;
	auto_caps_trial_punct = 0;
	auto_caps_trial_next = KC_NO;
	auto_caps_trial_applied = false;
	auto_caps_trial_reset_word();
	auto_caps_context_reset_word();
}

#ifdef CONSOLE_ENABLE
static void log_shift_trial(uint16_t keycode, keyrecord_t *record) {
	if (!record->event.pressed) {
		return;
	}

	uint16_t tap_key = base_tap_keycode(keycode);
	if (keycode == HM_F || keycode == HM_J) {
		if (shift_trial_pending) {
			shift_trial_flush();
		}
		shift_trial_keycode = tap_key;
		shift_trial_timer = timer_read();
		shift_trial_append_word(tap_key);
		return;
	}

	if (!shift_trial_pending && shift_trial_keycode != KC_NO && is_shift_trial_candidate(tap_key)) {
		shift_trial_pending = true;
		shift_trial_next = tap_key;
		shift_trial_dt = timer_elapsed(shift_trial_timer);
		shift_trial_mods = get_mods();
		shift_trial_tap = record->tap.count;
	}

	if (is_shift_trial_alpha(tap_key)) {
		shift_trial_append_word(tap_key);
	} else if (is_shift_trial_boundary(tap_key)) {
		shift_trial_flush();
	} else {
		shift_trial_word_safe = false;
		shift_trial_word[0] = '\0';
	}

	if (!shift_trial_pending) {
		shift_trial_keycode = KC_NO;
	}
}
#endif

static void process_auto_caps_trial(uint16_t keycode, keyrecord_t *record) {
	if (!record->event.pressed) {
		return;
	}

	uint16_t tap_key = base_tap_keycode(keycode);
	uint8_t mods = get_mods();
	uint8_t punct = shift_trial_punctuation(tap_key, mods);

	if (IS_QK_LAYER_TAP(keycode) && !punct) {
		return;
	}

	if (tap_key == KC_DOT || tap_key == KC_1 || tap_key == KC_SLSH || tap_key == SLASH_PIPE || tap_key == KC_EXLM || tap_key == KC_QUES ||
	    tap_key == ES_IQUE || tap_key == ES_IEXL || IS_QK_MODS(tap_key)) {
#ifdef CONSOLE_ENABLE
		uprintf("punct_probe key=%u tap=%u mods=%u layers=%u punct=%u\n", keycode, tap_key, mods, get_highest_layer(layer_state), punct);
#endif
	}

	if (layer_state_is(ESP) && !is_spanish_opening_punctuation(tap_key) && !is_spanish_trial_alpha(tap_key)) {
		auto_caps_trial_flush();
		return;
	}

	if (auto_caps_trial_state == AUTO_CAPS_LOG_WORD) {
		if (is_auto_caps_trial_alpha(tap_key)) {
			auto_caps_trial_append_word(tap_key);
			return;
		}
		if (is_shift_trial_boundary(tap_key)) {
			auto_caps_trial_flush();
		} else {
			auto_caps_trial_word_safe = false;
			auto_caps_trial_word[0] = '\0';
		}
	}

	if (auto_caps_trial_state == AUTO_CAPS_WAIT_SPACE) {
		if (tap_key == KC_SPC) {
			auto_caps_trial_state = AUTO_CAPS_WAIT_ALPHA;
			return;
		} else if (tap_key != KC_ENT) {
			auto_caps_trial_flush();
		}
	}

	if (auto_caps_trial_state == AUTO_CAPS_WAIT_ALPHA) {
		if (tap_key == KC_SPC) {
			return;
		}
		if (tap_key != KC_SPC && !is_auto_caps_trial_alpha(tap_key)) {
			auto_caps_trial_flush();
		}
	}

	if (is_auto_caps_trial_alpha(tap_key)) {
		if (auto_caps_trial_state == AUTO_CAPS_WAIT_ALPHA) {
			if (mods & ~MOD_MASK_SHIFT) {
				auto_caps_trial_flush();
				return;
			}
			auto_caps_trial_state = AUTO_CAPS_LOG_WORD;
			auto_caps_trial_next = tap_key;
			auto_caps_trial_mods = mods;
			auto_caps_trial_applied = !(mods & MOD_MASK_SHIFT) && !host_keyboard_led_state().caps_lock;
			if (auto_caps_trial_applied) {
				add_weak_mods(MOD_BIT(KC_LSFT));
			}
			auto_caps_trial_append_word(tap_key);
			return;
		}
		if (auto_caps_trial_state == AUTO_CAPS_IDLE || auto_caps_trial_state == AUTO_CAPS_WORD) {
			auto_caps_trial_state = AUTO_CAPS_WORD;
			auto_caps_context_append_word(tap_key);
		}
		return;
	}

	if (punct) {
		if (is_spanish_opening_punctuation(tap_key)) {
			auto_caps_trial_flush();
			auto_caps_trial_state = AUTO_CAPS_WAIT_ALPHA;
			auto_caps_trial_punct = punct;
		} else if (auto_caps_trial_state == AUTO_CAPS_WORD && !auto_caps_context_is_exception()) {
			auto_caps_trial_state = AUTO_CAPS_WAIT_SPACE;
			auto_caps_trial_punct = punct;
		} else {
			auto_caps_trial_flush();
		}
		return;
	}

	if (tap_key != KC_QUOT) {
		auto_caps_trial_flush();
	}
}

bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
	log_shift_trial(keycode, record);
#endif
	process_auto_caps_trial(keycode, record);

	if (!process_spanish_compose(keycode, record)) {
		return false;
	}

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
