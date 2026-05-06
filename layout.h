// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

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
// @REJ: remove colemak, define BSE, NUM, FNC, use json file to see what else I defined
// add a layer for vide editing
#define BSE 0
#define CMK 1
#define NUM 2
#define SYM 3
#define FNC 4

// Default 3x5_2 split layout
// @REJ: modify so that the smallest keyboard is a corne keyboard
#define _BASE \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │  TAB   │  Q     │  W     │  E     │  R     │  T     │   │  Y     │  U     │  I     │  O     │  P     │  BSPC  │ */\
	KC_TAB,   KC_Q,    KC_W,    KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   KC_BSPC,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  CAPS  │  A     │  S     │  D     │  F     │  G     │   │  H     │  J     │  K     │  L     │  ; :   │  ' "   │ */\
	KC_CAPS,  KC_A,    KC_S,    KC_D,    KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,   \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  SFT   │  Z     │  X     │  C     │  V     │  B     │   │  N     │  M     │ , <    │ . >    │ / ?    │  SFT   │ */\
	KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,    \
/* ╰────────┴────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
				  				KC_LGUI, KC_LALT, KC_SPC,  		KC_ENT, KC_RALT, MO(FNC)
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */ 

#define _COLE \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │  TAB   │  Q     │  W     │  F     │  P     │  G     │   │  J     │  L     │  U     │  Y     │  ' "   │  BSPC  │ */\
	KC_TAB,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,   KC_Y,   KC_QUOT,   KC_BSPC,   \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  CAPS  │  A     │  R     │  S     │  T     │  D     │   │  H     │  N     │  E     │  I     │  O     │  ; :   │ */\
	KC_CAPS,  KC_A,    KC_R,    KC_S,    KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,   KC_SCLN,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │  SFT   │  Z     │  X     │  C     │  V     │  B     │   │  K     │  M     │ , <    │ . >    │ / ?    │  SFT   │ */\
	KC_LSFT,  KC_Z,    KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  KC_SLSH, KC_RSFT,    \
/* ╰────────┴────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            _______, _______, _______,     _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */ 

// @REJ: put numbers on the right, and a shift modifier in the home row of the left, make your thumb buttons consistent
#define _NUMB \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┼────────┬────────┬────────┬────────╮ */\
/* │   /    │  CALC  │  7     │  8     │  9     │        │   │        │ HOME   │ PG DN  │ PG UP  │ END    │ DELETE │ */\
	KC_SLSH,  KC_CALC,  KC_7,    KC_8,    KC_9,   XXXXXXX,     XXXXXXX, KC_HOME, KC_PGDN, KC_PGUP, KC_END,  KC_DEL,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │   *    │   +    │  4     │  5     │  6     │        │   │        │ LEFT   │ DOWN   │ UP     │  RIGHT │        │ */\
	KC_ASTR, KC_PLUS,  KC_4,    KC_5,    KC_6,    XXXXXXX,     XXXXXXX, KC_LEFT, KC_DOWN, KC_UP,   KC_RGHT, _______,   \
/* ├────────┼────────┬────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │    -   │    0   │  1     │  2     │  3     │        │   │        │CTSF LF │CTSF DN │CTSF UP │CTSF RI │        │ */\
	KC_MINS,  KC_0,  	KC_1,    KC_2,    KC_3,   XXXXXXX,     XXXXXXX, CS_LF,	CS_DN,   CS_UP,  CS_RI,    _______,  \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┴────────┴────────╯ */\
	                            _______, _______, _______,     _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */

#define _SYMB \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │    ~   │    `   │        │        │        │        │   │        │        │   [    │  ]     │   \    │ DELETE │ */\
	KC_TILD,  KC_GRV, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_LBRC, KC_RBRC, KC_BSLS, KC_DEL,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │   !    │  @     │   #    │   $    │   %    │   ^    │   │  &     │  *     │   (    │   )    │   _    │   =    │ */\
	KC_EXLM,  KC_AT,  KC_HASH,  KC_DLR, KC_PERC,  KC_CIRC,    KC_AMPR, KC_ASTR, KC_LPRN,  KC_RPRN, KC_UNDS, KC_EQL,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │        │        │        │        │   	 |   │        │        │   {    │   }    │    |   │ 	   |*/\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_LCBR, KC_RCBR, KC_PIPE, _______,   \
/* ╰────────┴────────┴────────┴────────┼────────┼────────┤   ├────────┼────────┬────────┼────────┴────────┴────────╯ */\
	                            _______, _______, _______,     _______, _______, _______
/*                            ╰────────┴────────┴────────╯   ╰────────┴────────┴────────╯ */
// 	                           _______, _______,     _______, MO(FNC)
/* │ BOOT   │  F1    │  F2    │  F3    │ F10    │   │        │ WH UP  │ WH DN  │        │COLEMAK │ */
//	QK_BOOT, KC_F1,   KC_F2,   KC_F3,   KC_F10,      _______, KC_WH_U, KC_WH_D, _______, TG(CMK),    
#define _FUNC \
/* ╭────────┬────────┬────────┬────────┬────────┬────────╮   ╭────────┬────────┬────────┬────────┬────────┬────────╮ */\
/* │ ESC `  │        │  F7    │  F8    │  F9    │   F10  │   │   F11  │   F12  │ WH DN  │ WH UP  │        │ DELETE │ */\
	QK_GESC,  XXXXXXX, KC_F7,   KC_F8,   KC_F9,  KC_F10,      KC_F11,  KC_F12, KC_WH_D, KC_WH_U, XXXXXXX, KC_DEL,    \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │ Alt+F4 │  F4    │  F5    │  F6    │        │   │ Vol +  │ MS LFT │ MS DN  │ MS UP  │ MS RGT │        │ */\
	XXXXXXX,  Z_SHUT, KC_F4,   KC_F5,   KC_F6,   XXXXXXX,      KC_VOLU, KC_MS_L, KC_MS_D, KC_MS_U, KC_MS_R, _______,   \
/* ├────────┼────────┼────────┼────────┼────────┼────────┤   ├────────┼────────┼────────┼────────┼────────┼────────┤ */\
/* │        │ 		 │  F1    │  F2    │  F3    │        │   │ Vol -  │        │ BTN 1  │ BTN 2  │  	  │ COLEQ  │ */\
	XXXXXXX,  XXXXXXX, KC_F1,   KC_F2,   KC_F3,   XXXXXXX,     KC_VOLD, XXXXXXX, KC_BTN1, KC_BTN2, XXXXXXX, TG(CMK),   \
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
