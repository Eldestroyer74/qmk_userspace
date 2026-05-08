// ChieftainDots Corne combo reference.
//
// Combos match exact QMK keycodes. If a key is wrapped by HRM(), use the
// matching HM_* keycode rather than the plain tap key.
//
// KC_Q    KC_W    KC_E    KC_R    KC_T        KC_Y    KC_U    KC_I    KC_O    KC_P
// HM_A    HM_S    HM_D    HM_F    KC_G        KC_H    HM_J    HM_K    HM_L    HM_SCLN
// KC_Z    KC_X    KC_C    KC_V    KC_B        KC_N    KC_M    KC_COMM KC_DOT  KC_SLSH
//                   KC_LGUI KC_LALT KC_SPC    KC_ENT  KC_RALT MO(FNC)

#ifdef SWAP_HANDS_ENABLE
	COMB(swap_l,  SH_TG,  KC_X, KC_C, KC_V)
	COMB(swap_r,  SH_TG,  TH_M, TH_COMM, TH_DOT)
#endif
// Stale Filterpaper layer toggles used old thumb keys that are not present in
// the current Corne layout. Rebuild deliberately if persistent toggles return.
// COMB(tog_fnc, TG(FNC), LT(SYM,KC_TAB),  KC_B)
// COMB(tog_num, TG(NUM), LT(NUM,KC_BSPC), KC_N)
COMB(vol_up,  KC_VOLU, KC_P,    HM_SCLN)
COMB(vol_dn,  KC_VOLD, HM_SCLN, KC_SLSH)

// macros
//SUBS(vi_quit, ":q!",   KC_Q,    KC_W)
//SUBS(vi_save, ":x",    KC_Z,    KC_X)
//SUBS(dir_up,  "../",   TH_DOT,  TH_SLSH)
SUBS(hub_mtng, "https://meetings.hubspot.com/ricardoescalon", HM_F, KC_G)

// navigation
COMB(nav_dn,  CS_DN,   KC_M,    KC_COMM)
//COMB(nav_up,  SA_UP,   TH_COMM, TH_DOT)
COMB(nav_up,  CS_UP,   KC_I, KC_O)
COMB(nav_lf, CS_LF, KC_H, HM_J)
COMB(nav_ri, CS_RI, HM_J, HM_K)

//CAPSLOCK 
//COMB(caps, KC_CAPS, KC_F, KC_G)

// noop thumbs
// COMB(thmb_l,  KC_NO,   LT(SYM,KC_TAB), LCA_T(KC_ENT))
// COMB(thmb_r,  KC_NO,   RSFT_T(KC_SPC), LT(NUM,KC_BSPC))
