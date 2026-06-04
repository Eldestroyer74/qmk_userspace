// Copyright 2026 Ricardo Escalon
// SPDX-License-Identifier: GPL-2.0+

#pragma once

#define TXT_EMAIL QK_USER_0
#define TXT_PHONE QK_USER_1
#define TXT_NAME QK_USER_2
#define TXT_MEET QK_USER_3
#define TXT_WORK QK_USER_4
#define TXT_HOME QK_USER_5

#if __has_include("text_stubs_private.h")
#	include "text_stubs_private.h"
#endif

#ifndef TEXT_STUB_EMAIL
#	define TEXT_STUB_EMAIL "[email]"
#endif

#ifndef TEXT_STUB_PERSONAL_EMAIL
#	define TEXT_STUB_PERSONAL_EMAIL "[personal-email]"
#endif

#ifndef TEXT_STUB_PHONE
#	define TEXT_STUB_PHONE "[phone]"
#endif

#ifndef TEXT_STUB_NAME
#	define TEXT_STUB_NAME "[name]"
#endif

#ifndef TEXT_STUB_MEETING
#	define TEXT_STUB_MEETING "[meeting-link]"
#endif

#ifndef TEXT_STUB_WORK
#	define TEXT_STUB_WORK "[work-address]"
#endif

#ifndef TEXT_STUB_HOME
#	define TEXT_STUB_HOME "[home-address]"
#endif
