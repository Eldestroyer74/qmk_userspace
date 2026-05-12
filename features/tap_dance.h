// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#ifndef USER_TAP_DANCE_H
#define USER_TAP_DANCE_H

// Keep this header preprocessor-only because layout.h is included from config.h.
#define TD_TAB_ESC_CLOSE 0
#define TD_LEFT_BRACKET 1
#define TD_RIGHT_BRACKET 2
#define TD_SLASH_PIPE 3
#define TD_QUOTE_TILDE 4
#define TD_PLUS_EQUAL 5

#define TAB_ESC_CLOSE TD(TD_TAB_ESC_CLOSE)
#define LEFT_BRACKET TD(TD_LEFT_BRACKET)
#define RIGHT_BRACKET TD(TD_RIGHT_BRACKET)
#define SLASH_PIPE TD(TD_SLASH_PIPE)
#define QUOTE_TILDE TD(TD_QUOTE_TILDE)
#define PLUS_EQUAL TD(TD_PLUS_EQUAL)

#endif
