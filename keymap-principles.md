# Principles for ChieftainDots

ChieftainDots is the Corne keyboard project identity. These principles describe
the intended typing experience, and they should be challenged by the current
implementation before they are treated as rules.

## Current Implementation Challenge

- Finger memory is partly preserved: QWERTY, Shift, Enter, and familiar outer
  keys still exist, but home-row layer taps now compete with ordinary typing.
- Thumb consistency is partly implemented: left thumbs are GUI, Alt, Space and
  right thumbs are Enter, Alt, Function. The earlier Menu goal is not currently
  implemented.
- Move/select behavior is implemented mainly in the Numbers/Nav layer, including
  arrows, Home, End, Page Up, Page Down, and Ctrl+Shift navigation shortcuts.
- The layer model mostly follows "one layer, one concept": Base, Colemak,
  Numbers/Nav, Symbols, and Function. Numbers and navigation currently share a
  layer, which is accepted for now because it keeps movement close to numbers.
- Layers are mostly held rather than toggled, except Colemak, which is toggled
  from the Function layer.
- The left-hand-enables-right-hand idea is only partially true. Both hands can
  invoke number and symbol access through home-row layer taps.
- The "do not double up" principle is under review. A dedicated Symbols layer
  exists even though shifted number symbols also exist conceptually.
- Home-row modifiers/layers are implemented, but the exact CAGS/SyNCS ordering
  is still experimental and may be causing accidental layer activation.

## Revised Principles

### Preserve Useful Finger Memory

Keep familiar positions where they reduce friction: QWERTY on Base, Shift near
the outside lower row, Ctrl in a familiar location, and Enter on the right hand.
Changes are allowed when they make the layer model easier to remember, but they
must be called out in the roadmap.

### Make Thumbs Memorable

Thumb keys should follow a memorable bottom-row model. The intended direction is:

- Left: GUI, Alt, Space.
- Right: Space or Enter, Function, Menu or another deliberate command.

Any change to thumb keys must explain whether it prioritizes keycap familiarity,
typing comfort, or feature access.

### Support Movement And Selection

Navigation should make it easy to move and select by character, word, line, and
page. Arrows, Home, End, Page Up, Page Down, and Ctrl/Shift variants should stay
easy to reach and easy to remember.

### Prefer Memorable Layer Concepts

It is better to have more layers with clear concepts than fewer layers that hide
unrelated actions together. A layer should be judged by whether its purpose can
be named quickly.

### Keep Frequent Layer Access On Strong Positions

Frequently used layers should be reachable from thumbs or home-row holds. If
home-row holds cause accidental activation, tune timing or change placement
before adding more behavior on top.

### Keep Command Layers Literal

Command layers should remain predictable. Do not apply broad typing-layer
wrappers, such as `HRM(...)`, to Numbers/Nav, Symbols, Function, or future
editing layers unless a specific feature is deliberately designed and tested for
that behavior.

If a command layer needs selection or modifier behavior, prefer explicit named
commands, explicit modifier positions, or a dedicated editing layer. The layer
table should still explain what the key does without requiring the user to
remember nested tap-hold rules.

### Make Editing Keys Layer-Aware

Physical key positions can keep a related meaning across layers while changing
the exact command for the layer's job. On Base, the Backspace position should
remain Backspace for typing correction. On non-base command layers, that same
position should become Delete where practical, because command layers are more
often used for editing, navigation, and deliberate correction.

### One Layer, One Concept

Each layer should have a primary job:

- Base: ordinary typing.
- Colemak: alternate typing layout.
- Numbers/Nav: calculator-style numbers and movement.
- Symbols: punctuation and paired symbols.
- Function: function keys, mouse, media, and system actions.

If a feature does not fit a layer's concept, either change the feature, create a
new concept, or update this principle deliberately.

### Prefer Held Layers Over Toggles

Momentary layers are preferred because they reduce the need to check keyboard
state. Toggles are allowed only when the mode is intentionally persistent, such
as Colemak.

### Avoid Accidental Duplication

Do not add the same behavior in multiple places unless the duplicate serves a
clear ergonomic purpose. Duplicates must be documented as intentional.

### Use RGB As An Indicator

RGB should communicate keyboard state rather than compete with typing. Use it for
active layers, modifiers, Caps Lock, persistent toggles, and warnings. Avoid
continuous decorative animation as the default.

### Challenge Principles During Feature Work

Every feature must state which principle it supports or challenges. If a feature
conflicts with a principle, decide before implementation whether to revise the
feature or revise the principle.

## Decision Log

- Filterpaper remains acknowledged as ancestry, but ChieftainDots current
  guidance should describe this userspace rather than inherited defaults.
- A dedicated Symbols layer is accepted for now, even though shifted symbols also
  exist conceptually, because memorability may matter more than strict
  non-duplication.
- Colemak is allowed as a toggle because it is a persistent typing mode rather
  than a transient command layer.
- Applying `HRM(...)` to Numbers/Nav, Symbols, and Function was tested and rolled
  back. The wrapper works mechanically, but it transforms command-layer keys into
  nested tap-hold behaviors and makes the layers hard to understand.
