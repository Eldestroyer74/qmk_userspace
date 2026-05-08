# ChieftainDots Architecture

## Name Model

- ChieftainDots is the Corne keyboard project identity.
- `eldestroyer74` is the QMK userspace and GitHub identity.
- `keymaps/corne.json` is the Corne build recipe.
- Filterpaper is the upstream inspiration and ancestry.

## Build Shape

`keymaps/corne.json` tells QMK which keyboard to build, which userspace keymap
name to use, which physical layout wrapper to call, and which layer macros to
expand. It is a recipe, not the full keymap.

The layer contents and layout concepts live mainly in `layout.h`. Shared
behavior lives in feature modules, OLED modules, RGB modules, and userspace hook
code.

## Current Layer Concepts

- Base: QWERTY typing layer with familiar outer modifiers and thumb keys.
- Colemak: alternate typing layer toggled from the function layer.
- Numbers/Nav: calculator-style numbers on the left and movement/navigation on
  the right.
- Symbols: punctuation, brackets, braces, and shifted symbols.
- Function: function keys, mouse keys, media keys, shutdown shortcut, and layer
  toggles.

## Ownership Boundaries

- JSON chooses the keyboard, keymap name, wrapper layout, and layer macro list.
- `layout.h` owns physical key placement, layer concepts, home-row wrappers, and
  high-level key aliases.
- Feature modules own reusable behavior such as combos, caps unlock, and future
  macros.
- OLED and RGB modules should report state or provide feedback; they should not
  become the owner of core key behavior.
- `process_record_user` should stay coordinated. New behavior should be routed
  deliberately so one feature does not silently block another.

## HRM Wrapper Decision

`HRM(...)` is a compile-time wrapper for typing layers. It rewrites selected
home-row positions into dual-role keys such as `LT(SYM, key)`, `LT(NUM, key)`,
Ctrl-tap, and Shift-tap.

Use `HRM(...)` only on layers whose tap keys are intended to behave like ordinary
typing keys, currently Base and Colemak. Do not wrap command layers such as
Numbers/Nav, Symbols, or Function with `HRM(...)`.

This was tested and rolled back. Wrapping non-base layers made those layers less
predictable because the wrapper transformed symbols, numbers, and navigation
commands into new layer-tap or mod-tap keys. For example, a Symbols-layer key
could become "tap symbol, hold Numbers", which created nested layer behavior
while the user was already holding Symbols.

If ChieftainDots needs modifiers or selection behavior on a command layer, prefer
explicit named commands such as `C(S(KC_LEFT))`, explicit modifier positions, or
a dedicated editing layer. Do not solve that by applying the typing-layer HRM
wrapper to every layer.

## Anti-Spaghetti Rules

- One layer should have one memorable concept.
- One feature should own one behavior.
- Cross-feature dependencies must be written down before implementation.
- If a feature needs to change a principle, update the principle first or record
  the principle decision in the roadmap.
