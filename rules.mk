# Disable unused features
#LEADER_ENABLE = no
#COMMAND_ENABLE = no
#TERMINAL_ENABLE = no
#KEY_LOCK_ENABLE = no
#SLEEP_LED_ENABLE = no
#VELOCIKEY_ENABLE = no
MAGIC_ENABLE = no
CONSOLE_ENABLE = no
UNICODE_ENABLE = no
SWAP_HANDS_ENABLE = no
SPACE_CADET_ENABLE = no
GRAVE_ESC_ENABLE = no

# Common features
COMBO_ENABLE = no
TAP_DANCE_ENABLE = yes
EXTRAKEY_ENABLE = yes
MOUSEKEY_ENABLE = no
BOOTMAGIC_ENABLE = yes
OLED_ENABLE = yes

VPATH += $(USER_PATH)/oled $(USER_PATH)/rgb $(USER_PATH)/features
OPT_DEFS += -DCAPS_UNLOCK -DINIT_EE_HANDS_$(shell echo ${SPLIT}|tr a-z A-Z)
SRC += eldestroyer74.c caps_unlock.c spanish_compose.c
# Combo definitions are disabled while ChieftainDots redesigns chords around
# comfortable finger positions instead of inherited Filterpaper placements.
# QMK keymap introspection can include only one file. Use an aggregator for
# Tap Dance now, and future combos or other introspected tables later.
INTROSPECTION_KEYMAP_C = introspection.c

ifeq ($(strip $(MCU)), atmega32u4)
	LTO_ENABLE = yes
	TOP_SYMBOLS = yes
	BOOTLOADER = atmel-dfu
endif

# RGB boards
ifeq ($(strip $(KEYBOARD)), crkbd/rev1)
	RGB_MATRIX_ENABLE = yes
	RGB_MATRIX_CUSTOM_USER = yes
	SRC += rgb-matrix.c
endif

ifeq ($(strip $(KEYBOARD)), boardsource/unicorne)
	ifeq ($(strip $(CHIEFTAINDOTS_UNICORNE_VISUALS)), yes)
		RGB_MATRIX_CUSTOM_USER = yes
		SRC += rgb-matrix.c
	endif
endif

# OLED
ifeq ($(strip $(OLED_ENABLE)), yes)
	ifeq ($(strip $(KEYBOARD)), crkbd/rev1)
		OPT_DEFS += -DCHIEFTAINDOTS_CUSTOM_OLED
		ifeq ($(strip $(OLED)), LUNA FELIX)
			OPT_DEFS += -D${OLED}
			SRC += oled-icons.c oled-luna.c
		else
			SRC += oled-icons.c oled-bongocat.c oled-corne.c
		endif
	endif
	ifeq ($(strip $(KEYBOARD)), boardsource/unicorne)
		ifeq ($(strip $(CHIEFTAINDOTS_UNICORNE_VISUALS)), yes)
			OPT_DEFS += -DCHIEFTAINDOTS_CUSTOM_OLED -DCHIEFTAINDOTS_UNICORNE_OLED
			SRC += oled-icons.c oled-bongocat.c oled-unicorne.c
		endif
	endif
endif
