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

The previous `C:\Program Files\QMK_MSYS` path caused AVR LTO linking to fail
because the toolchain mishandled the space in `Program Files`.

## Known QMK Compatibility Notes

- Old mouse keycodes such as `KC_MS_L`, `KC_WH_U`, and `KC_BTN1` should use the
  current `MS_LEFT`, `MS_WHLU`, and `MS_BTN1` style names.
- `IGNORE_MOD_TAP_INTERRUPT` has been removed from current QMK and should not be
  used as live guidance.
- Combo source that defines `key_combos` must be visible to QMK keymap
  introspection; use the current introspection build path instead of treating it
  as ordinary shared `SRC`.
- Legacy keymap wiring that names `filterpaper` should not be copied into the
  active ChieftainDots userspace unless the goal is explicitly to build
  Filterpaper's original userspace.

## Size Pressure

The successful Corne build with LTO enabled had about 460 bytes free. Any feature
that adds code must include a size-risk check. If a feature pushes the firmware
over size, optimize that feature or roll it back before starting another one.

## Rollback Rule

Every behavior change should be small enough to undo alone. If a feature fails
to compile, cannot fit, or feels worse after flashing, revert only that feature
and keep the rest of the project intact.
