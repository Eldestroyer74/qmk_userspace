// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#include QMK_KEYBOARD_H
#include "split_util.h"
#include "transactions.h"
#include "../features/tap_dance.h"

extern void render_bongocat(void);
extern void render_mod_status(void);
extern uint32_t oled_tap_timer;
extern uint8_t chieftaindots_cat_state;

typedef struct {
	uint8_t activity_generation;
	uint8_t cat_state;
} oled_split_state_t;

#define OLED_SYNC_HEARTBEAT_MS 1000

static uint32_t oled_activity_timer;
static uint32_t oled_sync_timer;
static oled_split_state_t oled_split_state;
static oled_split_state_t oled_split_state_sent;

static void wake_oled_for_activity(void) {
	oled_activity_timer = timer_read32();
	oled_tap_timer      = oled_activity_timer;
	if (!is_oled_on()) {
		oled_on();
	}
}

static void oled_activity_sync_handler(uint8_t in_buflen, const void *in_data, uint8_t out_buflen, void *out_data) {
	(void)out_buflen;
	(void)out_data;
	if (in_buflen != sizeof(oled_split_state)) {
		return;
	}
	oled_split_state_t const *incoming = in_data;
	if (incoming->activity_generation != oled_split_state.activity_generation) {
		wake_oled_for_activity();
	}
	oled_split_state             = *incoming;
	chieftaindots_cat_state      = incoming->cat_state;
}

void chieftaindots_unicorne_oled_post_init(void) {
	transaction_register_rpc(CD_OLED_ACTIVITY_SYNC, oled_activity_sync_handler);
	wake_oled_for_activity();
}

void chieftaindots_unicorne_oled_note_activity(void) {
	if (!is_keyboard_master()) {
		return;
	}
	++oled_split_state.activity_generation;
	wake_oled_for_activity();
}

void chieftaindots_unicorne_oled_housekeeping(void) {
	if (!is_keyboard_master() || !is_transport_connected()) {
		return;
	}
	oled_split_state.cat_state = chieftaindots_cat_state;
	bool state_changed = memcmp(&oled_split_state, &oled_split_state_sent, sizeof(oled_split_state)) != 0;
	if (!state_changed && timer_elapsed32(oled_sync_timer) < OLED_SYNC_HEARTBEAT_MS) {
		return;
	}
	if (transaction_rpc_send(CD_OLED_ACTIVITY_SYNC, sizeof(oled_split_state), &oled_split_state)) {
		oled_split_state_sent = oled_split_state;
		oled_sync_timer       = timer_read32();
	}
}

bool chieftaindots_bongocat_anchor_active(void) {
	return get_highest_layer(layer_state) > CMK || (get_mods() & MOD_MASK_CTRL);
}

oled_rotation_t oled_init_user(oled_rotation_t const rotation) {
	(void)rotation;
	if (is_keyboard_master()) {
		return is_keyboard_left() ? OLED_ROTATION_0 : OLED_ROTATION_180;
	}
	return OLED_ROTATION_270;
}

bool oled_task_user(void) {
	bool visual_state_active = chieftaindots_bongocat_anchor_active() || chieftaindots_cat_state == CAT_BIG_PRESS;
	if (!visual_state_active && timer_elapsed32(oled_activity_timer) > OLED_TIMEOUT) {
		if (is_oled_on()) {
			oled_off();
		}
		return false;
	}
	if (!is_oled_on()) {
		oled_on();
	}
	if (is_keyboard_master()) {
		render_bongocat();
	} else {
		render_mod_status();
	}
	return false;
}
