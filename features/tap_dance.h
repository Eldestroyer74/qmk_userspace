// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#ifndef USER_TAP_DANCE_H
#define USER_TAP_DANCE_H

// Keep this header preprocessor-only because layout.h is included from config.h.
#define TD_TAB_ESC_CLOSE 0
#define TD_LEFT_BRACKET 1
#define TD_RIGHT_BRACKET 2
#define TD_SLASH_PIPE 3
#define TD_PLUS_EQUAL 4
#define TD_NUM_TWO_COMMA_LT 5
#define TD_NUM_THREE_DOT_GT 6
#define TD_NAV_UP 7
#define TD_NAV_LEFT 8
#define TD_NAV_DOWN 9
#define TD_NAV_RIGHT 10

#define TAB_ESC_CLOSE TD(TD_TAB_ESC_CLOSE)
#define LEFT_BRACKET TD(TD_LEFT_BRACKET)
#define RIGHT_BRACKET TD(TD_RIGHT_BRACKET)
#define SLASH_PIPE TD(TD_SLASH_PIPE)
#define PLUS_EQUAL TD(TD_PLUS_EQUAL)
#define NUM_TWO_COMMA_LT TD(TD_NUM_TWO_COMMA_LT)
#define NUM_THREE_DOT_GT TD(TD_NUM_THREE_DOT_GT)
#define NAV_UP_DANCE TD(TD_NAV_UP)
#define NAV_LEFT_DANCE TD(TD_NAV_LEFT)
#define NAV_DOWN_DANCE TD(TD_NAV_DOWN)
#define NAV_RIGHT_DANCE TD(TD_NAV_RIGHT)

#endif
