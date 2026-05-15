// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "features/tap_dance.h"
#include "features/text_stubs.h"

// Home row mod-tap macros
#define HM_A	/*LSFT_T(KC_A) LGUI_T(KC_A)*/ LT(SYM, KC_A) // SYMBOL
#define HM_S	/*LALT_T(KC_S)*/ LT(NUM, KC_S) // NUMBERS
#define HM_D	LCTL_T(KC_D)
#define HM_F	/*LGUI_T(KC_F)*/ LSFT_T(KC_F) 
#define HM_J	/*LGUI_T(KC_J)*/ RSFT_T(KC_J)
#define HM_K	RCTL_T(KC_K)
#define HM_L	/*LALT_T(KC_L)*/ LT(NUM, KC_L) // NUMBERS
//#define HM_QUOT	/*LSFT_T(KC_QUOT) LGUI_T(KC_QUOT)*/ LT(SYM, KC_QUOT)// SYMBOL
#define HM_SCLN LT(SYM, KC_SCLN)
//#define HRML(k1,k2,k3,k4) LSFT_T(k1),LALT_T(k2),LCTL_T(k3),LGUI_T(k4)
#define HRML(k1,k2,k3,k4) LT(SYM, k1),LT(NUM, k2),LCTL_T(k3),LSFT_T(k4)
//#define HRMR(k1,k2,k3,k4) LGUI_T(k1),LCTL_T(k2),LALT_T(k3),LSFT_T(k4)
#define HRMR(k1,k2,k3,k4) LSFT_T(k1),LCTL_T(k2),LT(NUM, k3),LT(SYM, k4)

// Window snap shortcuts. Plain Ctrl/Shift live on NAV and EXT so selection
// remains composable instead of needing a dedicated selection layer.
#define CS_UP C(S(KC_UP))
#define CS_DN C(S(KC_DOWN))
#define CS_LF C(S(KC_LEFT))
#define CS_RI C(S(KC_RIGHT))
#define G_UP G(KC_UP)
#define G_DN G(KC_DOWN)
#define G_LF G(KC_LEFT)
#define G_RI G(KC_RIGHT)

// Layer ids used by corne.json.
#define BSE 0
#define CMK 1
#define NUM 2
#define SYM 3
#define NAV 4
#define EXT 5
#define SNP 6
#define MED 7
#define TXT 8
#define SYS 9

// Base and Colemak alpha layers. corne.json applies HRM(...) to these layers.
#define _BASE \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │  TAB   │  Q     │  W     │  E     │  R     │  T     │   │  Y     │  U     │  I     │  O     │  P     │  BSPC  │ */\
	TAB_ESC_CLOSE, KC_Q, KC_W,  KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   KC_BSPC,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  CAPS  │  A     │  S     │  D     │  F     │  G     │   │  H     │  J     │  K     │  L     │  ; :   │  ' "   │ */\
	KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,   \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ MEDIA  │  Z     │  X     │  C     │  V     │  B     │   │  N     │  M     │ , <    │ . >    │ / ?    │  CALC  │ */\
	KC_MSEL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  SLASH_PIPE, KC_CALC, \
/* ╰────────┴────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
				  				KC_LGUI, KC_LALT, KC_SPC,  		KC_ENT, KC_RALT, KC_APP
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */ 

#define _COLE \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │  TAB   │  Q     │  W     │  F     │  P     │  G     │   │  J     │  L     │  U     │  Y     │  ' "   │  BSPC  │ */\
	KC_TAB,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,   KC_Y,   KC_QUOT, KC_BSPC, \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  CAPS  │  A     │  R     │  S     │  T     │  D     │   │  H     │  N     │  E     │  I     │  O     │  ; :   │ */\
	KC_CAPS,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,   KC_SCLN,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ MEDIA  │  Z     │  X     │  C     │  V     │  B     │   │  K     │  M     │ , <    │ . >    │ / ?    │  CALC  │ */\
	KC_MSEL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  SLASH_PIPE, KC_CALC, \
/* ╰────────┴────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            _______, _______, _______,     _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */ 

// S/L-held command layers: Numbers combines top-row memory with a right-hand pad.
#define _NUMB \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┼────────┬────────┬────────┬────────╮ */\
/* │        │        │        │        │        │        │   │   +    │   7    │   8    │   9    │   *    │ DELETE │ */\
	KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │   -    │   4    │   5    │   6    │   /    │   .    │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, MO(SYM), XXXXXXX,     XXXXXXX, KC_4,    KC_5,    KC_6,    KC_EQL,  XXXXXXX, \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │   (    │   1    │   2    │   3    │   0    │   )    │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_1,    NUM_TWO_COMMA_LT, NUM_THREE_DOT_GT, KC_SLSH, XXXXXXX, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            MO(NAV), MO(EXT), MO(SNP),     _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */

// S/L + left GUI thumb navigation: I/J/K/L form the right-hand arrow shape.
#define _NAV \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┼────────┬────────┬────────┬────────╮ */\
/* │        │        │        │        │        │        │   │        │        │   UP   │        │        │ DELETE │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, KC_DEL,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │ LEFT   │ DOWN   │ RIGHT  │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL, KC_LSFT, XXXXXXX,     XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │        │        │        │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            _______, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */

// S/L + left Space thumb snap: GUI+arrows follow the same I/J/K/L shape.
#define _SNP \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┼────────┬────────┬────────┬────────╮ */\
/* │        │        │        │        │        │        │   │        │        │ G UP   │        │        │ DELETE │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, G_UP,    XXXXXXX, XXXXXXX, KC_DEL,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │ G LEFT │ G DOWN │ G RGHT │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, G_LF,    G_DN,    G_RI,    XXXXXXX, XXXXXXX, \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │        │        │        │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            XXXXXXX, XXXXXXX, _______,     XXXXXXX, XXXXXXX, XXXXXXX
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */

// S/L + left Alt thumb extremes: Home/Page movement follows the same shape.
#define _EXTR \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┼────────┬────────┬────────┬────────╮ */\
/* │        │        │        │        │        │        │   │        │        │ PG UP  │        │        │ DELETE │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_PGUP, XXXXXXX, XXXXXXX, KC_DEL,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │ HOME   │ PG DN  │ END    │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL, KC_LSFT, XXXXXXX,     XXXXXXX, KC_HOME, KC_PGDN, KC_END,  XXXXXXX, XXXXXXX, \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │        │        │        │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            XXXXXXX, XXXXXXX, _______,     XXXXXXX, XXXXXXX, XXXXXXX
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */

#define _SYMB \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │   !    │  @     │   #    │   $    │   %    │   ^    │   │   &    │   *    │ ( [ {  │ ) ] }  │   _    │ DELETE │ */\
	KC_TILD,  KC_EXLM, KC_AT,  KC_HASH, KC_DLR,   KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, LEFT_BRACKET, RIGHT_BRACKET, KC_UNDS, \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │   │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS, XXXXXXX, \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │   │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_EXLM, KC_AT,   KC_HASH, XXXXXXX, XXXXXXX, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┬────────┼────────┴────────┴────────╯ */\
	                            MO(MED), MO(TXT), MO(SYS),    _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */
// A/; + left-thumb command layers.
// Media uses the same right-hand I/J/K/L navigation shape for volume and tracks.
#define _MEDI \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_MPLY, XXXXXXX, KC_VOLU, XXXXXXX, XXXXXXX, KC_DEL,  \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_MUTE, KC_MPRV, KC_VOLD, KC_MNXT, XXXXXXX, XXXXXXX, \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
	                            _______, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX

// Text stubs keep personal snippets in the same right-hand I/J/K/L shape.
#define _TEXT \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     TXT_HOME, XXXXXXX, TXT_PHONE, XXXXXXX, XXXXXXX, KC_DEL,  \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     TXT_WORK, TXT_MEET, TXT_EMAIL, TXT_NAME, XXXXXXX, XXXXXXX, \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
	                            XXXXXXX, _______, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX

// System/function keys mirror the number-pad shape.
#define _SYST \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_F12,  KC_F7,   KC_F8,   KC_F9,   XXXXXXX, KC_DEL,  \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_F11,  KC_F4,   KC_F5,   KC_F6,   XXXXXXX, TG(CMK), \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_F10,  KC_F1,   KC_F2,   KC_F3,   XXXXXXX, XXXXXXX, \
	                            XXXXXXX, XXXXXXX, _______,    XXXXXXX, XXXXXXX, XXXXXXX

// Home-row wrapper. Only Base/Colemak pass through HRM(...) in corne.json;
// command layers stay plain so held keys compose predictably with thumbs.
#define HRM(k) HRM_TAPHOLD(k)
#define HRM_TAPHOLD( \
	  l01, l02, l03, l04, l05, l06,   r01, r02, r03, r04, r05, r06,  \
	  l07, l08, l09, l10, l11, l12,   r07, r08, r09, r010, r111, r12,\
	  l13, l14, l15, l16, l17, l18,   r13, r14, r15, r16, r17, r18,  \
	                 l19, l20, l21,   r19, r20, r21 \
) \
	  l01,      l02, l03, l04, l05,  l06,   r01, 	  r02, r03, r04, r05,    r06, \
	  l07, HRML(l08, l09, l10, l11), l12,   r07, HRMR(r08, r09, r010, r111), r12, \
	  l13,      l14, l15, l16, l17,  l18,   r13, 	  r14, r15, r16, r17,    r18, \
	                 l19, l20, l21,   r19, r20, r21 



// Layout aliases used by the userspace keymap JSON files.
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define LAYOUT_crkbd_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_ortho_w(...) LAYOUT_ortho_4x12(__VA_ARGS__)


// Convert 3x5_2 to 42-key for legacy userspace keymaps.
#define C_42(k) CONV_42(k)
#define CONV_42( \
	     l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, \
	     l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, \
	     l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, \
	                    l16, l17,   r16, r17                 \
) \
QK_GESC,  l01, l02, l03, l04, l05,   r01, r02, r03, r04, r05, KC_BSPC, \
KC_TAB,  l06, l07, l08, l09, l10,   r06, r07, r08, r09, r10, KC_ENT,  \
KC_LSFT, l11, l12, l13, l14, l15,   r11, r12, r13, r14, r15, KC_SCLN, \
	     LT(SYM, KC_DEL), l16, l17,   r16, r17, LT(SYM, KC_DEL)
//	     RSA_T(KC_ESC), l16, l17,   r16, r17, RAG_T(KC_DEL)

// Convert 3x5_2 to 4x12 for legacy userspace keymaps.
#define C_O(k) CONV_4x12(k)
#define CONV_4x12( \
	                      l01, l02, l03, l04, l05, r01, r02, r03, r04, r05, \
	                      l06, l07, l08, l09, l10, r06, r07, r08, r09, r10, \
	                      l11, l12, l13, l14, l15, r11, r12, r13, r14, r15, \
	                                     l16, l17, r16, r17                 \
) \
                 KC_TAB,  l01, l02, l03, l04, l05, r01, r02, r03, r04, r05, KC_BSPC, \
                 QK_GESC, l06, l07, l08, l09, l10, r06, r07, r08, r09, r10, KC_SCLN, \
                 KC_LSFT, l11, l12, l13, l14, l15, r11, r12, r13, r14, r15, KC_ENT,  \
KC_DEL, KC_VOLD, KC_VOLU, RSA_T(KC_ESC), l16, l17, r16, r17, KC_LEFT, KC_DOWN, KC_UP, KC_RGHT
