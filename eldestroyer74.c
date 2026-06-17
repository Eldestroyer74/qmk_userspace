// Copyright 2022 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "eldestroyer74.h"
#include "features/spanish_compose.h"
#include "features/tap_dance.h"
#include "features/text_stubs.h"
#include "features/user_keycodes.h"

#ifdef CONSOLE_ENABLE
#	include "print.h"
#	define SPACE_PROBE_ENABLE 0
#endif

extern bool process_spanish_compose(uint16_t keycode, keyrecord_t *record);
extern void send_windows_alt_code(const char *code);
extern uint8_t chieftaindots_snap_mode;
uint8_t chieftaindots_cat_state;

#define AUTO_CAPS_ENABLE 0

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
// Keep hold-on-other-key-press disabled for typing keys; thumb Shift remains
// deliberate instead of firing on ordinary rolls.
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
#if SPACE_PROBE_ENABLE
static bool space_probe_down;
static bool space_probe_long_reported;
static uint16_t space_probe_timer;
static uint16_t space_probe_count;
static uint16_t space_probe_quick_count;
static uint8_t space_probe_keys_since_space;
static bool space_probe_gap_reported;
static bool session_probe_reported;

static unsigned long log_ms(void) {
	return (unsigned long)timer_read32();
}

static void log_session_probe(void) {
	if (session_probe_reported) {
		return;
	}
	session_probe_reported = true;
	uprintf("bt ms=%lu ma=%u lh=%u l=%u m=%u\n", log_ms(), is_keyboard_master(), is_keyboard_left(), get_highest_layer(layer_state), get_mods());
}
#endif
#endif

#if AUTO_CAPS_ENABLE
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
#endif
static bool delayed_alt_pending;
static bool delayed_alt_registered;
static bool delayed_alt_blocked;
static bool delayed_alt_space_blocked;

static uint16_t base_tap_keycode(uint16_t keycode) {
	if (IS_QK_LAYER_TAP(keycode)) {
		return QK_LAYER_TAP_GET_TAP_KEYCODE(keycode);
	}
	if (IS_QK_MOD_TAP(keycode)) {
		return QK_MOD_TAP_GET_TAP_KEYCODE(keycode);
	}
	return keycode;
}

#if defined(CONSOLE_ENABLE) || AUTO_CAPS_ENABLE
static bool is_shift_trial_alpha(uint16_t keycode) {
	return KC_A <= keycode && keycode <= KC_Z;
}
#endif

static bool is_shift_keycode(uint16_t keycode) {
	uint16_t tap_key = base_tap_keycode(keycode);

	return keycode == KC_LSFT || keycode == KC_RSFT || keycode == THUMB_SPACE_SHIFT ||
	       keycode == THUMB_ENTER_SHIFT || tap_key == KC_LSFT || tap_key == KC_RSFT;
}

static bool has_shift_modifier(void) {
	uint8_t mods = get_mods() | get_weak_mods();
#ifndef NO_ACTION_ONESHOT
	mods |= get_oneshot_mods();
#endif
	return mods & MOD_MASK_SHIFT;
}

static bool is_space_keycode(uint16_t keycode) {
	return base_tap_keycode(keycode) == KC_SPC;
}

static void register_delayed_alt(void) {
	if (!delayed_alt_registered) {
		register_code(KC_LALT);
		delayed_alt_registered = true;
	}
}

static void unregister_delayed_alt(void) {
	if (delayed_alt_registered) {
		unregister_code(KC_LALT);
	}
	delayed_alt_registered = false;
	delayed_alt_pending = false;
	delayed_alt_blocked = false;
	delayed_alt_space_blocked = false;
}

static bool process_delayed_alt(uint16_t keycode, keyrecord_t *record) {
	if (keycode == DELAYED_LALT) {
		if (record->event.pressed) {
			delayed_alt_pending = true;
			delayed_alt_registered = false;
			delayed_alt_blocked = false;
			delayed_alt_space_blocked = false;
		} else {
			if (delayed_alt_pending && !delayed_alt_registered && !delayed_alt_blocked) {
				tap_code(KC_LALT);
			}
			unregister_delayed_alt();
		}
		return false;
	}

	if (delayed_alt_pending && !delayed_alt_registered && record->event.pressed && is_space_keycode(keycode)) {
		delayed_alt_blocked = true;
		delayed_alt_space_blocked = true;
		return true;
	}

	if (delayed_alt_pending && !delayed_alt_registered && record->event.pressed && is_shift_keycode(keycode)) {
		delayed_alt_blocked = true;
		return true;
	}

	if (delayed_alt_pending && !delayed_alt_registered && record->event.pressed && !delayed_alt_space_blocked) {
		register_delayed_alt();
	}
	return true;
}

static bool process_directional_english_quotes(uint16_t keycode, keyrecord_t *record) {
	if (!record->event.pressed || !has_shift_modifier()) {
		return true;
	}

	if (keycode == KC_QUOT) {
		send_windows_alt_code("0148");
		return false;
	}
	return true;
}

#if AUTO_CAPS_ENABLE
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

#if defined(CONSOLE_ENABLE) || AUTO_CAPS_ENABLE
static char shift_trial_char(uint16_t keycode) {
	return 'a' + (keycode - KC_A);
}
#endif

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
#endif

#if AUTO_CAPS_ENABLE
static bool is_spanish_opening_punctuation(uint16_t keycode) {
	return keycode == ES_IQUE || keycode == ES_IEXL;
}
#endif

#if defined(CONSOLE_ENABLE) || AUTO_CAPS_ENABLE
static bool is_shift_trial_boundary(uint16_t keycode) {
	return keycode == KC_SPC || keycode == KC_ENT || keycode == KC_DOT || keycode == KC_COMM ||
	       keycode == KC_SCLN || keycode == KC_QUOT || keycode == KC_SLSH || keycode == KC_MINS;
}
#endif

#if AUTO_CAPS_ENABLE
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
#endif

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
		uprintf("st p=%u n=%u d=%u m=%u t=%u w=%s\n",
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

#if AUTO_CAPS_ENABLE
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
		uprintf("ac p=%u n=%u m=%u a=%u w=%s\n",
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
}

static void auto_caps_trial_clear_all(void) {
	auto_caps_trial_flush();
	auto_caps_context_reset_word();
}
#endif

#ifdef CONSOLE_ENABLE
static bool is_home_row_shift_trial_key(uint16_t keycode) {
	return false;
}

static void log_shift_trial(uint16_t keycode, keyrecord_t *record) {
	if (!record->event.pressed) {
		return;
	}

	uint16_t tap_key = base_tap_keycode(keycode);
	if (is_home_row_shift_trial_key(keycode)) {
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

static void log_thumb_shift_trial(uint16_t keycode, keyrecord_t *record) {
	if (record->event.pressed || (keycode != THUMB_SPACE_SHIFT && keycode != THUMB_ENTER_SHIFT)) {
		return;
	}

	uprintf("ts k=%u a=%s t=%u m=%u l=%u\n",
	        keycode,
	        record->tap.count ? "tap" : "hold",
	        record->tap.count,
	        get_mods(),
	        get_highest_layer(layer_state));
}

#if SPACE_PROBE_ENABLE
static void log_space_probe(uint16_t keycode, keyrecord_t *record) {
	log_session_probe();

	uint16_t tap_key = base_tap_keycode(keycode);
	if (tap_key != KC_SPC && keycode != THUMB_SPACE_SHIFT) {
		if (record->event.pressed && get_highest_layer(layer_state) <= CMK && tap_key != KC_NO) {
			if (space_probe_keys_since_space < UINT8_MAX) {
				space_probe_keys_since_space++;
			}
			if (!space_probe_gap_reported && space_probe_keys_since_space >= 16) {
				space_probe_gap_reported = true;
				uprintf("sg ms=%lu k=%u s=%u q=%u kc=%u m=%u l=%u ma=%u lh=%u\n",
				        log_ms(),
				        space_probe_keys_since_space,
				        space_probe_count,
				        space_probe_quick_count,
				        tap_key,
				        get_mods(),
				        get_highest_layer(layer_state),
				        is_keyboard_master(),
				        is_keyboard_left());
			}
		}
		return;
	}

	if (record->event.pressed) {
		space_probe_down = true;
		space_probe_long_reported = false;
		space_probe_timer = timer_read();
		uprintf("sp ms=%lu e=p kc=%u r=%u c=%u t=%u m=%u l=%u\n",
		        log_ms(),
		        keycode,
		        record->event.key.row,
		        record->event.key.col,
		        record->tap.count,
		        get_mods(),
		        get_highest_layer(layer_state));
		return;
	}

	uint16_t dt = timer_elapsed(space_probe_timer);
	space_probe_count++;
	if (dt < 20) {
		space_probe_quick_count++;
	}
	uprintf("sp ms=%lu e=r kc=%u r=%u c=%u a=%c t=%u d=%u m=%u l=%u\n",
	        log_ms(),
	        keycode,
	        record->event.key.row,
	        record->event.key.col,
	        record->tap.count ? 't' : 'h',
	        record->tap.count,
	        dt,
	        get_mods(),
	        get_highest_layer(layer_state));
	if ((space_probe_count % 25) == 0) {
		uprintf("ss ms=%lu s=%u q=%u d=%u a=%c m=%u l=%u ma=%u lh=%u\n",
		        log_ms(),
		        space_probe_count,
		        space_probe_quick_count,
		        dt,
		        record->tap.count ? 't' : 'h',
		        get_mods(),
		        get_highest_layer(layer_state),
		        is_keyboard_master(),
		        is_keyboard_left());
	}
	space_probe_keys_since_space = 0;
	space_probe_gap_reported = false;
	space_probe_down = false;
}
#endif
#endif

#if defined(CONSOLE_ENABLE) && SPACE_PROBE_ENABLE
void suspend_power_down_user(void) {
	uprintf("usb ms=%lu e=s l=%u m=%u\n", log_ms(), get_highest_layer(layer_state), get_mods());
}

void suspend_wakeup_init_user(void) {
	uprintf("usb ms=%lu e=w l=%u m=%u\n", log_ms(), get_highest_layer(layer_state), get_mods());
}
#endif

#if AUTO_CAPS_ENABLE
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
		uprintf("pp k=%u t=%u m=%u l=%u p=%u\n", keycode, tap_key, mods, get_highest_layer(layer_state), punct);
#endif
	}

	if (layer_state_is(ESP) && !is_spanish_opening_punctuation(tap_key) && !is_spanish_trial_alpha(tap_key)) {
		auto_caps_trial_clear_all();
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
			auto_caps_trial_clear_all();
		}
	}

	if (auto_caps_trial_state == AUTO_CAPS_WAIT_ALPHA) {
		if (tap_key == KC_SPC) {
			return;
		}
		if (tap_key != KC_SPC && !is_auto_caps_trial_alpha(tap_key)) {
			auto_caps_trial_clear_all();
		}
	}

	if (is_auto_caps_trial_alpha(tap_key)) {
		if (auto_caps_trial_state == AUTO_CAPS_WAIT_ALPHA) {
			if (mods & ~MOD_MASK_SHIFT) {
				auto_caps_trial_clear_all();
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
		} else if (auto_caps_context_is_exception()) {
			auto_caps_trial_clear_all();
		} else if (auto_caps_trial_state == AUTO_CAPS_WORD) {
			auto_caps_trial_state = AUTO_CAPS_WAIT_SPACE;
			auto_caps_trial_punct = punct;
		} else {
			auto_caps_trial_clear_all();
		}
		return;
	}

	if (tap_key != KC_QUOT) {
		auto_caps_trial_clear_all();
	}
}
#endif

#define TEXT_MNEMONICS_ENABLE 1
#define TEXT_MNEMONIC_TERM 250

#if TEXT_MNEMONICS_ENABLE
static uint16_t text_mnemonic_key = KC_NO;
static uint16_t text_mnemonic_timer;
static uint16_t text_mnemonic_pending_key = KC_NO;
static uint16_t text_mnemonic_pending_timer;
static const char *text_mnemonic_pending_stub;
static bool text_mnemonic_pending_sent;

static const char *text_stub_for_mnemonic(uint16_t keycode) {
	switch (keycode) {
		case KC_G:
			return TEXT_STUB_PERSONAL_EMAIL;
		case KC_H:
			return TEXT_STUB_HOME;
		case KC_W:
			return TEXT_STUB_WORK;
		case KC_P:
			return TEXT_STUB_PHONE;
		case KC_M:
			return TEXT_STUB_MEETING;
		case KC_E:
			return TEXT_STUB_EMAIL;
		case KC_N:
			return TEXT_STUB_NAME;
	}
	return NULL;
}

static bool process_text_mnemonic(uint16_t keycode, keyrecord_t *record) {
	uint16_t tap_key = base_tap_keycode(keycode);

	if (text_mnemonic_pending_key != KC_NO && tap_key != text_mnemonic_pending_key) {
		if (!text_mnemonic_pending_sent) {
			tap_code16(text_mnemonic_pending_key);
		}
		text_mnemonic_pending_key = KC_NO;
		text_mnemonic_key = KC_NO;
	}

	if (text_mnemonic_pending_key != KC_NO && tap_key == text_mnemonic_pending_key && !record->event.pressed) {
		if (!text_mnemonic_pending_sent) {
			tap_code16(text_mnemonic_pending_key);
		}
		text_mnemonic_pending_key = KC_NO;
		return false;
	}

	if (!record->event.pressed || get_highest_layer(layer_state) > CMK || (get_mods() | get_weak_mods())) {
		return true;
	}

	const char *stub = text_stub_for_mnemonic(tap_key);
	if (!stub) {
		text_mnemonic_key = KC_NO;
		return true;
	}

	if (text_mnemonic_key == tap_key && timer_elapsed(text_mnemonic_timer) <= TEXT_MNEMONIC_TERM) {
		text_mnemonic_key = KC_NO;
		text_mnemonic_pending_key = tap_key;
		text_mnemonic_pending_timer = timer_read();
		text_mnemonic_pending_stub = stub;
		text_mnemonic_pending_sent = false;
		return false;
	}

	text_mnemonic_key = tap_key;
	text_mnemonic_timer = timer_read();
	return true;
}
#endif

void matrix_scan_user(void) {
#if TEXT_MNEMONICS_ENABLE
	if (text_mnemonic_pending_key == KC_NO || text_mnemonic_pending_sent || timer_elapsed(text_mnemonic_pending_timer) < TAPPING_TERM) {
		goto space_probe_scan;
	}

	tap_code(KC_BSPC);
	send_string_with_delay(text_mnemonic_pending_stub, 0);
	text_mnemonic_pending_sent = true;

space_probe_scan:
#endif
#if defined(CONSOLE_ENABLE) && SPACE_PROBE_ENABLE
	if (space_probe_down && !space_probe_long_reported && timer_elapsed(space_probe_timer) > 1000) {
		space_probe_long_reported = true;
		uprintf("sp ms=%lu e=h kc=%u d=%u m=%u l=%u\n",
		        log_ms(),
		        THUMB_SPACE_SHIFT,
		        timer_elapsed(space_probe_timer),
		        get_mods(),
		        get_highest_layer(layer_state));
	}
#endif
}

static bool process_snap_mode(uint16_t keycode, keyrecord_t *record) {
	if (chieftaindots_snap_mode == SNAP_MODE_NONE) {
		return true;
	}

	switch (keycode) {
		case KC_UP:
		case KC_LEFT:
		case KC_DOWN:
		case KC_RGHT:
			if (record->event.pressed) {
				chieftaindots_cat_state = CAT_BIG_PRESS;
				if (chieftaindots_snap_mode == SNAP_MODE_WINDOW) {
					uint8_t mods = get_mods();
					uint8_t weak_mods = get_weak_mods();
					del_mods(MOD_MASK_CTRL);
					del_weak_mods(MOD_MASK_CTRL);
					tap_code16(G(keycode));
					set_mods(mods);
					set_weak_mods(weak_mods);
				}
			} else {
				chieftaindots_cat_state = CAT_IDLE;
			}
			return false;
	}

	return true;
}

static bool is_command_layer_action_key(uint16_t keycode) {
	if (IS_QK_MODS(keycode)) {
		keycode = QK_MODS_GET_BASIC_KEYCODE(keycode);
	}

	switch (keycode) {
		case KC_UP:
		case KC_LEFT:
		case KC_DOWN:
		case KC_RGHT:
		case KC_VOLU:
		case KC_VOLD:
		case KC_MPLY:
			return true;
		default:
			return false;
	}
}

static void update_cat_for_command_action(uint16_t keycode, keyrecord_t *record) {
	if (get_highest_layer(layer_state) <= CMK || !is_command_layer_action_key(keycode)) {
		return;
	}
	chieftaindots_cat_state = record->event.pressed ? CAT_BIG_PRESS : CAT_IDLE;
}

bool process_record_user(uint16_t const keycode, keyrecord_t *record) {
#ifdef CONSOLE_ENABLE
	log_shift_trial(keycode, record);
	log_thumb_shift_trial(keycode, record);
#if SPACE_PROBE_ENABLE
	log_space_probe(keycode, record);
#endif
#endif
#if AUTO_CAPS_ENABLE
	process_auto_caps_trial(keycode, record);
#endif

	if (!process_directional_english_quotes(keycode, record)) {
		return false;
	}

	if (!process_spanish_compose(keycode, record)) {
		return false;
	}

	if (!process_delayed_alt(keycode, record)) {
		return false;
	}

	if (!process_snap_mode(keycode, record)) {
		return false;
	}

	update_cat_for_command_action(keycode, record);

#if TEXT_MNEMONICS_ENABLE
	if (!process_text_mnemonic(keycode, record)) {
		return false;
	}
#endif

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
