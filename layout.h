// Copyright 2021 @filterpaper
// SPDX-License-Identifier: GPL-2.0+

#include "features/tap_dance.h"
#include "features/spanish_compose.h"
#include "features/user_keycodes.h"
#include "features/layers.h"

// Home row mod-tap macros
#define HM_A	/*LSFT_T(KC_A) LGUI_T(KC_A)*/ LT(SYM, KC_A) // SYMBOL
#define HM_S	/*LALT_T(KC_S)*/ LT(NUM, KC_S) // NUMBERS
#define HM_D	LCTL_T(KC_D)
#define HM_F	/*LGUI_T(KC_F)*/ LCTL_T(KC_F)
#define HM_J	/*LGUI_T(KC_J)*/ RCTL_T(KC_J)
#define HM_K	RCTL_T(KC_K)
#define HM_L	/*LALT_T(KC_L)*/ LT(NUM, KC_L) // NUMBERS
//#define HM_QUOT	/*LSFT_T(KC_QUOT) LGUI_T(KC_QUOT)*/ LT(SYM, KC_QUOT)// SYMBOL
#define HM_SCLN LT(SYM, KC_SCLN)
//#define HRML(k1,k2,k3,k4) LSFT_T(k1),LALT_T(k2),LCTL_T(k3),LGUI_T(k4)
#define HRML(k1,k2,k3,k4) LT(SYS, k1), LT(SYM, k2), LT(NUM, k3), LCTL_T(k4)
//#define HRMR(k1,k2,k3,k4) LGUI_T(k1),LCTL_T(k2),LALT_T(k3),LSFT_T(k4)
#define HRMR(k1,k2,k3,k4) RCTL_T(k1), LT(NUM, k2), LT(SYM, k3), LT(SYS, k4)

// Command layers repeat the Base right thumbs instead of using transparent
// fallthrough so behavior and RGB stay aligned through the ordinary
// keymap-driven path. Keep this repetition here instead of adding RGB
// special cases for inherited thumb keys.
#define THUMB_SPACE_SHIFT LSFT_T(KC_SPC)
#define THUMB_ENTER_SHIFT RSFT_T(KC_ENT)
#define NUM_SPACE_SYMBOLS LT(SYM, KC_SPC)
#define NUM_ENTER_SYMBOLS LT(SYM, KC_ENT)
#define RIGHT_THUMBS THUMB_ENTER_SHIFT, KC_RALT, KC_APP
#define NUM_RIGHT_THUMBS NUM_ENTER_SYMBOLS, KC_RALT, KC_APP

// Base and Colemak alpha layers. corne.json applies HRM(...) to these layers.
#define _BASE \
/* Top:    TAB    Q      W      E      R      T        Y      U      I      O      P      BSPC */\
	TAB_ESC_CLOSE, KC_Q, KC_W,  KC_E,    KC_R,    KC_T,         KC_Y,    KC_U,    KC_I,    KC_O,    KC_P,   KC_BSPC,    \
/* Home:   CAPS   A      S      D      F      G        H      J      K      L      ;      '    */\
	CAPS_LONG_DANCE, KC_A, KC_S, KC_D,   KC_F,    KC_G,        KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,   \
/* Bottom: ES     Z      X      C      V      B        N      M      ,      .      /      ES   */\
	SPANISH_LEFT_DANCE, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,        KC_N,    KC_M,    KC_COMM, KC_DOT,  SLASH_PIPE, SPANISH_RIGHT_DANCE, \
/* Thumbs:                         GUI    ALT    SPC/SFT  ENT/SFT RALT   MENU */\
	BASE_GUI_SNAP, DELAYED_LALT, THUMB_SPACE_SHIFT, THUMB_ENTER_SHIFT, KC_RALT, KC_APP

#define _COLE \
/* Top:    TAB    Q      W      F      P      G        J      L      U      Y      '      BSPC */\
	KC_TAB,   KC_Q,    KC_W,    KC_F,    KC_P,    KC_G,        KC_J,    KC_L,    KC_U,   KC_Y,   KC_QUOT, KC_BSPC, \
/* Home:   CAPS   A      R      S      T      D        H      N      E      I      O      ;    */\
	CAPS_LONG_DANCE, KC_A, KC_R, KC_S,   KC_T,    KC_D,        KC_H,    KC_N,    KC_E,    KC_I,    KC_O,   KC_SCLN,    \
/* Bottom: ES     Z      X      C      V      B        K      M      ,      .      /      ES   */\
	SPANISH_LEFT_DANCE, KC_Z,   KC_X,    KC_C,    KC_V,    KC_B,        KC_K,    KC_M,    KC_COMM, KC_DOT,  SLASH_PIPE, SPANISH_RIGHT_DANCE, \
/* Thumbs:                         trans  trans  trans    trans  trans  trans */\
	                            _______, _______, _______,     _______, _______, _______

// D/K-held command layer: Numbers combines top-row memory with a right-hand pad.
#define _NUMB \
/* Top:    `      1      2      3      4      5        6      7      8      9      0      -    */\
	KC_GRV,  KC_1,    KC_2,    KC_3,    KC_4,    KC_5,        KC_6,    KC_7,    KC_8,    KC_9,    KC_0,    KC_MINS, \
/* Home:   ---    ---    ---    ---    ---    ---      ---    4      5      6      =      ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_4,    KC_5,    KC_6,    KC_EQL,  XXXXXXX, \
/* Bottom: ---    ---    ---    ---    ---    ---      ---    1      2/,/<  3/./>  //\//| ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_1,    NUM_TWO_COMMA_LT, NUM_THREE_DOT_GT, SLASH_PIPE, XXXXXXX, \
/* Thumbs:                         NAV    ALT    SPC/SYM  ENT/SYM RALT   MENU */\
	                            NUM_GUI_NAV_SNAP, DELAYED_LALT, NUM_SPACE_SYMBOLS, NUM_RIGHT_THUMBS

// D/K + left GUI thumb navigation: tap arrows, hold extremes,
// double-tap selection movement, double-tap-hold extreme selection.
#define _NAV \
/* Top:    ---    ---    ---    ---    ---    ---      ---    ---    UP     ---    ---    DEL  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, NAV_UP_DANCE, XXXXXXX, XXXXXXX, KC_DEL,   \
/* Home:   ---    ---    ---    ---    CTRL   ---      ---    LEFT   DOWN   RIGHT  ---    ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, KC_LCTL, XXXXXXX,     XXXXXXX, NAV_LEFT_DANCE, NAV_DOWN_DANCE, NAV_RIGHT_DANCE, XXXXXXX, XXXXXXX, \
/* Bottom: ---    ---    ---    ---    ---    ---      ---    ---    ---    ---    ---    ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* Thumbs:                         trans  ALT    SHIFT    ENTER  RALT   MENU */\
	                            _______, DELAYED_LALT, THUMB_SPACE_SHIFT, RIGHT_THUMBS

// Snap is entered by double-tap-holding an exposed GUI key. The gesture keeps
// GUI held, so this layer sends plain arrows for repeated Windows traversal.
#define _SNP \
/* Top:    ---    ---    ---    ---    ---    ---      ---    ---    UP     ---    ---    DEL  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_UP,   XXXXXXX, XXXXXXX, KC_DEL,   \
/* Home:   ---    ---    ---    ---    ---    ---      ---    LEFT   DOWN   RIGHT  ---    ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_LEFT, KC_DOWN, KC_RGHT, XXXXXXX, XXXXXXX, \
/* Bottom: ---    ---    ---    ---    ---    ---      ---    ---    ---    ---    ---    ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* Thumbs:                         trans  ---    trans    ENTER  RALT   MENU */\
	                            _______, XXXXXXX, _______,     RIGHT_THUMBS

// Symbols + left GUI thumb MS Styles: PowerPoint paragraph/list level movement.
#define _EXTR \
/* Top:    ---    ---    ---    ---    ---    ---      ---    ---    AS-UP  ---    ---    DEL  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, MS_STYLE_UP_DANCE, XXXXXXX, XXXXXXX, KC_DEL,   \
/* Home:   ---    ---    ---    ---    ---    ---      ---    AS-LF  AS-DN  AS-RI  ---    ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, MS_STYLE_LEFT_DANCE, MS_STYLE_DOWN_DANCE, MS_STYLE_RIGHT_DANCE, XXXXXXX, XXXXXXX, \
/* Bottom: ---    ---    ---    ---    ---    ---      ---    ---    ---    ---    ---    ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* Thumbs:                         trans  ALT    SHIFT    ENTER  RALT   MENU */\
	                            _______, DELAYED_LALT, THUMB_SPACE_SHIFT, RIGHT_THUMBS

#define _SYMB \
/* Top:    ~      !      @      #      $      %        ^      &      *      (/[/{  )/]/}  _    */\
	KC_TILD,  KC_EXLM, KC_AT,  KC_HASH, KC_DLR,   KC_PERC,    KC_CIRC, KC_AMPR, KC_ASTR, LEFT_BRACKET, RIGHT_BRACKET, KC_UNDS, \
/* Home:   ---    ---    ---    ---    ---    ---      ---    $      %      ^      +      ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_DLR,  KC_PERC, KC_CIRC, KC_PLUS, XXXXXXX, \
/* Bottom: ---    ---    ---    ---    ---    ---      ---    !      @      #      //\//| ---  */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_EXLM, KC_AT,   KC_HASH, SLASH_PIPE, XXXXXXX, \
/* Thumbs:                         MS     ALT    SHIFT    ENTER  RALT   MENU */\
	                            SYM_GUI_EXT_SNAP, DELAYED_LALT, THUMB_SPACE_SHIFT, RIGHT_THUMBS
// Function + left GUI thumb media: compact arrow-shaped meeting/audio controls.
#define _MEDI \
/* Top:    ---    ---    ---    ---    ---    ---      ---    ---    VOL+   ---    ---    DEL */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, KC_VOLU, XXXXXXX, XXXXXXX, KC_DEL,  \
/* Home:   ---    ---    ---    ---    ---    ---      ---    MUTE   VOL-   PLAY   ---    --- */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_MUTE, KC_VOLD, KC_MPLY, XXXXXXX, XXXXXXX, \
/* Bottom: ---    ---    ---    ---    ---    ---      ---    ---    ---    ---    ---    --- */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, \
/* Thumbs:                         trans  ALT    SHIFT    ENTER  RALT   MENU */\
	                            _______, DELAYED_LALT, THUMB_SPACE_SHIFT, RIGHT_THUMBS

// Function keys mirror the Numbers layer's top row and right-hand pad.
#define _SYST \
/* Top:    TAB    F1     F2     F3     F4     F5       F6     F7     F8     F9     F10    DEL */\
	TAB_ESC_CLOSE, KC_F1, KC_F2, KC_F3, KC_F4,   KC_F5,       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  KC_DEL,  \
/* Home:   ---    ---    ---    ---    ---    ---      F11    F4     F5     F6     ---    COLE */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_F4,   KC_F5,   KC_F6,   KC_F11,  TG(CMK), \
/* Bottom: ---    ---    ---    ---    ---    ---      ---    F1     F2     F3     F12    --- */\
	XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, KC_F1,   KC_F2,   KC_F3,   KC_F12,  XXXXXXX, \
/* Thumbs:                         MED    ALT    SHIFT    ENTER  RALT   MENU */\
	                            SYS_GUI_MED_SNAP, DELAYED_LALT, THUMB_SPACE_SHIFT, RIGHT_THUMBS

// Spanish layer: hold either outside bottom key, then press the matching letter.
#define _SPAN \
/* Top:    ---    ¡      ---    É      ---    ---      ---    Ú      Í      Ó      ---    ---  */\
	XXXXXXX, ES_IEXL, XXXXXXX, ES_E,    XXXXXXX, XXXXXXX,     XXXXXXX, ES_U,    ES_I,    ES_O,    XXXXXXX, XXXXXXX, \
/* Home:   ---    Á      ---    ---    ---    ---      ---    ---    ---    ---    Ü      ---  */\
	ES_LDAQ, ES_A,    XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, ES_UDIA, ES_RDAQ, \
/* Bottom: trans  ---    ---    ---    ---    ---      Ñ      ---    ---    ---    ¿      trans */\
	_______, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX, XXXXXXX,     ES_N,    XXXXXXX, XXXXXXX, XXXXXXX, ES_IQUE, _______, \
/* Thumbs:                         ---    ---    ---      ENTER  RALT   MENU */\
	                            XXXXXXX, XXXXXXX, XXXXXXX,    RIGHT_THUMBS

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



// Current ChieftainDots uses corne.json and LAYOUT_crkbd_w.
// Future keyboard ports should add a fresh wrapper from the current layer model
// instead of carrying forward stale inherited recipes.
#define LAYOUT_crkbd_w(...) LAYOUT_split_3x6_3(__VA_ARGS__)
