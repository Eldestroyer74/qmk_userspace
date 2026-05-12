// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "features/tap_dance.h"

// macOS shortcuts
// @REJ: Don't need shortcuts, these are called in the eldestroyr74.c file.
#define Z_UND	C(KC_Z) 
#define Z_CUT	C(KC_X)
#define Z_CPY	C(KC_C)
#define Z_PST	C(KC_V)
#define Z_LOCK	G(KC_L)
#define Z_SHUT  A(KC_F4) //Alt + F4 shuts
/*#define Z_SLEEP	A(G(KC_PWR))
#define Z_SHUT	C(A(G(KC_PWR)))
#define Z_SSAVE	HYPR(KC_S)*/


// Tap hold macros
/* @REJ: Leave, back to layer 0 if you press the bottom right hand row from any layer?
 * Add TH macros for accents and egne 
 * These are multiple options to return to layer 0
 */
#define TH_M	LT(0,KC_M)
#define TH_COMM	LT(0,KC_COMM) //GUI Key
#define TH_DOT	LT(0,KC_DOT)
#define TH_SLSH	LT(0,KC_SLSH)
#define TH(k1,k2,k3,k4) LT(0,k1),LT(0,k2),LT(0,k3),LT(0,k4) // provides a shortcut to the base layer from either hand

// Home row mod-tap macros
/* @REJ: Change the order of this SyNCS SCNSy  */
#define HM_A	/*LSFT_T(KC_A) LGUI_T(KC_A)*/ LT(SYM, KC_A) // SYMBOL
#define HM_S	/*LALT_T(KC_S)*/ LT(NUM, KC_S) // NUMBERS
#define HM_D	LCTL_T(KC_D)
#define HM_F	/*LGUI_T(KC_F)*/ LSFT_T(KC_F) 
#define HM_J	/*LGUI_T(KC_J)*/ RSFT_T(KC_J)
#define HM_K	RCTL_T(KC_K)
#define HM_L	/*LALT_T(KC_L)*/ LT(NUM, KC_L) // NUMBERS
//#define HM_QUOT	/*LSFT_T(KC_QUOT) LGUI_T(KC_QUOT)*/ LT(SYM, KC_QUOT)// SYMBOL
#define HM_SCLN LT(SYM, KC_SCLN)
// @REJ: change the below to match the order that you like
//#define HRML(k1,k2,k3,k4) LSFT_T(k1),LALT_T(k2),LCTL_T(k3),LGUI_T(k4)
#define HRML(k1,k2,k3,k4) LT(SYM, k1),LT(NUM, k2),LCTL_T(k3),LSFT_T(k4)
//#define HRMR(k1,k2,k3,k4) LGUI_T(k1),LCTL_T(k2),LALT_T(k3),LSFT_T(k4)
#define HRMR(k1,k2,k3,k4) LSFT_T(k1),LCTL_T(k2),LT(NUM, k3),LT(SYM, k4)

// Navigation shortcuts
// @REJ: define my select word "Ctrl" + "Shift" + Left, or right, same with home and end
//#define SA_UP S(A(KC_UP))
#define CS_UP C(S(KC_UP))
//#define SA_DN S(A(KC_DOWN))
#define CS_DN C(S(KC_DOWN))
#define CS_LF C(S(KC_LEFT))
#define CS_RI C(S(KC_RIGHT))

// Layers
// @REJ: remove colemak, define BSE, NUM, use json file to see what else I defined
// add a layer for vide editing
#define BSE 0
#define CMK 1
#define NUM 2
#define SYM 3
#define NAV 4
#define SEL 5
#define EXT 6
#define MED 7
#define MOU 8
#define SYS 9

// Default 3x5_2 split layout
// @REJ: modify so that the smallest keyboard is a corne keyboard
#define _BASE \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │  TAB   │  Q     │  W     │  E     │  R     │  T     │   │  Y     │  U     │  I     │  O     │  P     │  BSPC  │ */\
	TAB_ESC_CLOSE, KC_Q, KC_W,  KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   KC_BSPC,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  CAPS  │  A     │  S     │  D     │  F     │  G     │   │  H     │  J     │  K     │  L     │  ; :   │  ' "   │ */\
	KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, QUOTE_TILDE,\
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ MEDIA  │  Z     │  X     │  C     │  V     │  B     │   │  N     │  M     │ , <    │ . >    │ / ?    │  CALC  │ */\
	KC_MSEL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  SLASH_PIPE, KC_CALC, \
/* ╰────────┴────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
				  				KC_LGUI, KC_LALT, KC_SPC,  		KC_ENT, KC_RALT, KC_APP
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */ 

#define _COLE \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │  TAB   │  Q     │  W     │  F     │  P     │  G     │   │  J     │  L     │  U     │  Y     │  ' "   │  BSPC  │ */\
	KC_TAB,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,   KC_Y,   QUOTE_TILDE, KC_BSPC, \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  CAPS  │  A     │  R     │  S     │  T     │  D     │   │  H     │  N     │  E     │  I     │  O     │  ; :   │ */\
	KC_CAPS,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,   KC_SCLN,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ MEDIA  │  Z     │  X     │  C     │  V     │  B     │   │  K     │  M     │ , <    │ . >    │ / ?    │  CALC  │ */\
	KC_MSEL,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  SLASH_PIPE, KC_CALC, \
/* ╰────────┴────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            _______, _______, _______,     _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */ 

// S/L-held numbered commands: the right hand owns digits and calculator operators.
#define _NUMB \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┼────────┬────────┬────────┬────────╮ */\
/* │        │        │        │        │        │        │   │   +    │   7    │   8    │   9    │   *    │ DELETE │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_PLUS, KC_7,    KC_8,    KC_9,    KC_ASTR, KC_DEL,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │   -    │   4    │   5    │   6    │   /    │   .    │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_MINS, KC_4,    KC_5,    KC_6,    KC_SLSH, KC_DOT,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │   (    │   1    │   2    │   3    │   0    │   )    │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_LPRN, KC_1,    KC_2,    KC_3,    KC_0,    KC_RPRN, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            MO(NAV), MO(SEL), MO(EXT),     _______, _______, _______
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

// S/L + left Alt thumb selection: word/large-unit selection follows I/J/K/L.
#define _SELE \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┼────────┬────────┬────────┬────────╮ */\
/* │        │        │        │        │        │        │   │        │        │ CS UP  │        │        │ DELETE │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, CS_UP,   XXXXXXX, XXXXXXX, KC_DEL,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │ CS LF  │ CS DN  │ CS RT  │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, CS_LF,   CS_DN,   CS_RI,   XXXXXXX, XXXXXXX, \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │        │   │        │        │        │        │        │        │ */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            XXXXXXX, _______, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */

// S/L + left Space thumb extremes: Home/Page movement follows the same shape.
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
	KC_EXLM,  KC_AT,  KC_HASH,  KC_DLR, KC_PERC,  KC_CIRC,    KC_AMPR, KC_ASTR, LEFT_BRACKET, RIGHT_BRACKET, KC_UNDS, KC_DEL, \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │   │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │ */\
	_______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │   │ trans  │ trans  │ trans  │ trans  │ trans  │ trans  │ */\
	_______, _______, _______, _______, _______, _______,     _______, _______, _______, _______, _______, _______, \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┬────────┼────────┴────────┴────────╯ */\
	                            MO(MED), MO(MOU), MO(SYS),    _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */
// A + left GUI thumb media: volume and track controls follow the I/J/K/L shape.
#define _MEDI \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_MPLY, XXXXXXX, KC_VOLU, XXXXXXX, XXXXXXX, KC_DEL,  \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_MUTE, KC_MPRV, KC_VOLD, KC_MNXT, XXXXXXX, XXXXXXX, \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
	                            _______, XXXXXXX, XXXXXXX,    XXXXXXX, XXXXXXX, XXXXXXX

// A + left Alt thumb mouse: pointer movement follows I/J/K/L; wheel follows +/-.
#define _MOUS \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     MS_WHLU, XXXXXXX, MS_UP,   XXXXXXX, XXXXXXX, KC_DEL,  \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     MS_WHLD, MS_LEFT, MS_DOWN, MS_RGHT, XXXXXXX, XXXXXXX, \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
	                            XXXXXXX, _______, XXXXXXX,    MS_BTN1, XXXXXXX, MS_BTN2

// A + left Space thumb function/system: function keys mirror the number-pad shape.
#define _SYST \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_F12,  KC_F7,   KC_F8,   KC_F9,   XXXXXXX, KC_DEL,  \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_F11,  KC_F4,   KC_F5,   KC_F6,   XXXXXXX, TG(CMK), \
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     KC_F10,  KC_F1,   KC_F2,   KC_F3,   XXXXXXX, XXXXXXX, \
	                            XXXXXXX, XXXXXXX, _______,    XXXXXXX, XXXXXXX, XXXXXXX

// 	                           _______, _______,     _______, MO(FNC)
/* │ BOOT   │  F1    │  F2    │  F3    │ F10    │   │        │ WH UP  │ WH DN  │        │COLEMAK │ */
//	QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F10,      _______, KC_WH_U, KC_WH_D, _______, TG(CMK),    
#define _FUNC \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │ ESC `  │        │  F7    │  F8    │  F9    │   F10  │   │   F11  │   F12  │ WH DN  │ WH UP  │        │ DELETE │ */\
	QK_GESC,  XXXXXXX, KC_F7,   KC_F8,   KC_F9,  KC_F10,      KC_F11,  KC_F12, MS_WHLD, MS_WHLU, XXXXXXX, KC_DEL,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │ Alt+F4 │  F4    │  F5    │  F6    │        │   │ Vol +  │ MS LFT │ MS DN  │ MS UP  │ MS RGT │        │ */\
	XXXXXXX,  Z_SHUT, KC_F4,   KC_F5,   KC_F6,   XXXXXXX,      KC_VOLU, MS_LEFT, MS_DOWN, MS_UP,   MS_RGHT, _______,   \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │ 		 │  F1    │  F2    │  F3    │        │   │ Vol -  │        │ BTN 1  │ BTN 2  │  	  │ COLEQ  │ */\
	XXXXXXX,  XXXXXXX, KC_F1,   KC_F2,   KC_F3,   XXXXXXX,     KC_VOLD, XXXXXXX, MS_BTN1, MS_BTN2, XXXXXXX, TG(CMK),   \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┴────────┴────────╯ */\
	                            _______, _______, _______,     _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */
/* │ SLEEP  │  F7    │  F8    │  F9    │ F12    │   │        │ BTN 2  │ BTN 1  │        │ SSAVE  │ */
//	                           _______, QK_RBT,      _______, _______
/*                            ╰────────┴────────╯   ╰────────┴────────╯ */
// Mod-tap wrapper
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



// Layout aliases for
#define LAYOUT_34key_w(...) LAYOUT_split_3x5_2(__VA_ARGS__)
#define LAYOUT_crkbd_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
#define LAYOUT_ortho_w(...) LAYOUT_ortho_4x12(__VA_ARGS__)


// Convert 3x5_2 to 42-key
/* REJ: Swap tab and gesc, put sym, del in the buttons for thumbs*/
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

// Convert 3x5_2 to 4x12
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
