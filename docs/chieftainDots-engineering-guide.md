# ChieftainDots Engineering Guide

ChieftainDots is the Corne keyboard project. The active implementation lives in
the `eldestroyer74` QMK userspace, and the Corne build recipe is
`keymaps/corne.json`.

## Working Model

- Treat `qmk_firmware` as the upstream build dependency.
- Treat `users/eldestroyer74` as the personal source of truth.
- Treat Filterpaper as ancestry and reference material, not the active identity.
- Make one feature change at a time and compile before moving on.

## Research Before Editing

Before changing behavior, check the current QMK docs, the local QMK source, and
the existing userspace code. Prefer current QMK names and build hooks over older
examples found in inherited docs.

Any QMK callback, hook, config option, or keycode must be verified against at
least one current source before use:

- local QMK source or docs in this checkout
- official QMK docs
- an existing working pattern in this userspace

Do not add a hook just because a name sounds plausible. If the hook is not found
in QMK docs or source, route behavior through a documented callback such as
`process_record_user`, `post_process_record_user`, `keyboard_post_init_user`, or
the documented RGB Matrix indicator callbacks.

Use local inspection first:

```bash
rg "symbol_or_feature_name" users/eldestroyer74 quantum keyboards
```

Then compile the current baseline:

```bash
qmk compile users/eldestroyer74/keymaps/corne.json
```

## Firmware Checkpoints

QMK should keep writing its normal output file:

```text
C:\Users\RicardoEscalon\Documents\qmk_firmware\crkbd_rev1_eldestroyer74.hex
```

After an accepted source commit, copy that file into the local ignored
`firmware-history/` folder with a Julian-style date, short commit SHA, and short
behavior name:

```text
firmware-history/2026128_6920586_checkpoint-cleanup.hex
```

These firmware files are for quick QMK Toolbox rollback only. They are not the
source of truth and should not be committed.

## Coding Style

Follow the userspace `.editorconfig`: UTF-8, tabs, and four-column tab width.
Keep C changes small and module-owned:

- Put RGB color names and constants in `rgb/rgb-matrix.h`.
- Put RGB behavior in `rgb/rgb-matrix.c`.
- Put key-event routing in `eldestroyer74.c`.
- Put layer and key-placement concepts in `layout.h`.
- Put build feature flags in `rules.mk`.
- Put QMK configuration in `config.h`.

Prefer short helper functions when they clarify ownership, but avoid clever
abstractions that hide QMK behavior. Add comments only for rules a future reader
would not infer from the code, such as "Caps Lock is a whole-keyboard warning
state."

## QMK MSYS

The working Windows path should avoid spaces:

```text
C:\QMK_MSYS
```

Run QMK from MINGW64. From PowerShell, Codex can use:

```powershell
& 'C:\QMK_MSYS\shell_connector.cmd' -lc 'cd /c/Users/RicardoEscalon/Documents/qmk_firmware && qmk compile users/eldestroyer74/keymaps/corne.json'
```

For split Corne flashing, prefer the canonical JSON flash commands so QMK uses
the active ChieftainDots userspace and writes the correct `EE_HANDS` handedness
marker to each half:

```bash
qmk flash users/eldestroyer74/keymaps/corne.json -bl dfu-split-left
qmk flash users/eldestroyer74/keymaps/corne.json -bl dfu-split-right
```

Flash each half while USB is plugged directly into that half. Press the reset
button near the screen when QMK waits for the bootloader. This matters for RGB
and OLED behavior because each half must know whether it is left or right.

The previous `C:\Program Files\QMK_MSYS` path caused AVR LTO linking to fail
because the toolchain mishandled the space in `Program Files`.

## Known QMK Compatibility Notes

- `EE_HANDS` requires the halves to be flashed with the correct split target.
  If layer keys work but per-key RGB appears on only one side, or right-side
  command-layer indicators do not light, check handedness flashing before
  changing RGB masks.
- Flow Tap is the preferred first experiment for accidental home-row tap-hold
  activation during normal typing flow. QMK documents `FLOW_TAP_TERM 150` as a
  starting point; ChieftainDots should trial it before making broader
  home-row timing or layer-placement changes.
- Old mouse keycodes such as `KC_MS_L`, `KC_WH_U`, and `KC_BTN1` should use the
  current `MS_LEFT`, `MS_WHLU`, and `MS_BTN1` style names.
- `IGNORE_MOD_TAP_INTERRUPT` has been removed from current QMK and should not be
  used as live guidance.
- Combo source that defines `key_combos` must be visible to QMK keymap
  introspection; use the current introspection build path instead of treating it
  as ordinary shared `SRC`.
- Tap Dance source that defines `tap_dance_actions` must also be visible to QMK
  keymap introspection. Put the action table in a feature file such as
  `features/tap_dance.c`, include it from the introspection aggregator, wire that
  aggregator with `INTROSPECTION_KEYMAP_C`, and do not add the same file to
  `SRC`.
- QMK keymap introspection accepts one include file. Use
  `features/introspection.c` as the aggregator for Tap Dance, future combos, and
  any other introspected tables.
- `layout.h` is included through `config.h`, so it can be seen by AVR assembly
  sources during the build. Keep declarations in `layout.h` preprocessor-safe;
  avoid C-only declarations such as `enum` there.
- Legacy keymap wiring that names `filterpaper` should not be copied into the
  active ChieftainDots userspace unless the goal is explicitly to build
  Filterpaper's original userspace.

## Size Pressure

The current Corne build after the A-family command layers has about 2126 bytes
free. Any feature that adds code must include a size-risk check. If a feature
pushes the firmware over size, optimize that feature or roll it back before
starting another one.

ChieftainDots now uses more than eight layers, so `config.h` must use
`LAYER_STATE_16BIT`. If QMK reports that the number of keymap layers exceeds
`LAYER_STATE_(8|16|32)BIT`, check the layer count before changing behavior.

## OLED Lessons

ChieftainDots currently keeps Filterpaper's OLED architecture: Bongocat renders
on the master/primary side, and `oled-icons.c` renders the compact status panel
on the secondary side. Do not casually change this to physical-left/physical-
right behavior; the cat wake/sleep timer and split activity sync need deliberate
testing if the cat moves away from the master side.

Generated 30x16 outline/filled icon artwork for all concepts is too expensive
for the current firmware budget. A generated `oledfont_icons.c` trial used glyph
codes beyond `0xFF`, conflicted with existing font positions, and represented
roughly 1KB or more of new font data. Future OLED art should be tiny: start with
only the four home-row concepts (Symbols, Numbers, Control, Shift), reuse or
replace a small number of existing glyphs, and measure size before flashing.

Do not stack Filterpaper's existing Ctrl/Shift modifier glyph fragments as if
they were standalone full-height tiles. They were designed for the original
horizontal modifier panel and look partially drawn when used vertically by
themselves. Either preserve the original panel, create purpose-built tiny
home-row glyphs, or use text labels as a temporary readable fallback.

If considering a full chord-discovery OLED concept, first measure how many bytes
are recovered by removing Bongocat. Treat that as a separate product decision:
the tradeoff is visual personality versus functional discoverability.

## Rollback Rule

Every behavior change should be small enough to undo alone. If a feature fails
to compile, cannot fit, or feels worse after flashing, revert only that feature
and keep the rest of the project intact.
