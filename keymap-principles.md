# Principles for ChieftainDots

ChieftainDots is the Corne keyboard project identity. These principles describe
the intended typing experience, and they should be challenged by the current
implementation before they are treated as rules.

## Current Implementation Challenge

- Finger memory is partly preserved: QWERTY, Shift, Enter, and familiar outer
  keys still exist, but home-row layer taps now compete with ordinary typing.
- Thumb consistency is partly implemented: left thumbs are GUI, Alt, Space and
  right thumbs are Enter, Alt, Menu. The old Function thumb has been retired in
  the active Corne build.
- Move/select behavior is implemented mainly in the Numbers/Nav layer, including
  arrows, Home, End, Page Up, Page Down, and Ctrl+Shift navigation shortcuts.
- The layer model mostly follows "one layer, one concept": Base, Colemak,
  Numbers/Nav, Symbols, and Function. Numbers and navigation currently share a
  layer, which is accepted for now because it keeps movement close to numbers.
- Layers are mostly held rather than toggled, except Colemak, which is a
  persistent typing mode and is being moved out of the retired Function layer.
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

Directional command layers should prioritize spatial consistency over strict
home-row purity. The intended right-hand directional shape is `I/J/K/L`: `I` is
Up, `J` is Left, `K` is Down, and `L` is Right. Selection and extremes should
reuse that same shape where practical, even though `I` is above the home row.

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
- Media/Mouse/System: tool and persistent-mode commands that replaced the old
  mixed Function layer.

If a feature does not fit a layer's concept, either change the feature, create a
new concept, or update this principle deliberately.

### Prefer Held Layers Over Toggles

Momentary layers are preferred because they reduce the need to check keyboard
state. Toggles are allowed only when the mode is intentionally persistent, such
as Colemak.

### Avoid Accidental Duplication

Do not add the same behavior in multiple places unless the duplicate serves a
clear ergonomic purpose. Duplicates must be documented as intentional.

### Prefer One Source Of Truth

When QMK or the local architecture already knows something, derive from that
source instead of copying the same knowledge into another table or helper. This
keeps behavior aligned, reduces maintenance, and can save firmware bytes.

Examples:

- RGB command-layer lighting should follow the active keymap where possible,
  with explicit exceptions only for visual concepts that are not literal keys,
  such as chord anchors, thumb previews, or warning colors.
- Documentation diagrams should be updated when a layer changes, but code should
  not maintain a second independent map of the same layer unless the visual rule
  deliberately differs from the key behavior.
- Size optimizations should first look for duplicated concepts, stale helpers,
  and custom code that QMK already provides before cutting useful behavior.

### Byte Is King For Unused Code

Do not spend firmware bytes on behavior ChieftainDots is not actively using.
Unused QMK features, stale helpers, inherited aliases, dormant source files, and
"maybe later" code should be disabled, removed, or left in git history instead
of compiled into the keyboard.

Byte cuts should be measured one at a time. A feature is safe to disable only
when the active Corne layout does not use it, the compile still succeeds, and
the expected physical behavior is unchanged. If the feature is behavior-visible,
such as OLED, RGB, Caps Unlock, app launchers, text snippets, or split-hand
recovery, treat it as a design decision rather than a cleanup.

### Do Not Double Up Clean Windows Shortcuts

Do not spend System-layer keys on Windows shortcuts that are already clean,
memorable, and directly available, such as Snipping Tool (`Win+Shift+S`), Show
Desktop (`Win+D`), Settings (`Win+I`), File Explorer (`Win+E`), or Lock
(`Win+L`).

The System layer should prefer app-launch keycodes, keyboard-mode toggles, and
commands that do not already have a good operating-system shortcut. Exceptions
are allowed only when the ergonomic benefit is explicit and recorded in the
roadmap.

### Use RGB As An Indicator

RGB should communicate keyboard state rather than compete with typing. Use it for
active layers, modifiers, Caps Lock, persistent toggles, and warnings. Avoid
continuous decorative animation as the default.

RGB should show the scope of the active state:

- Whole-keyboard states use the whole keyboard. Caps Lock is red because it
  changes ordinary typing globally. Colemak is purple because it is a persistent
  whole-keyboard typing mode.
- Plain modifier states should not be lit from global modifier state. That
  approach made Ctrl and Shift ambiguous and lit opposite-hand modifiers that
  were not physically pressed. Modifier RGB can return only if it tracks the
  actual physical modifier key being held and uses distinct Ctrl/Shift colors.
- Command layers light the usable command surface for that layer, not every
  inherited or technically non-transparent key.

RGB should also distinguish present state from future choice:

- Base and Colemak should stay visually quiet. Do not use idle home-row preview
  lights; they proved noisy and did not help because some modifier previews did
  not correspond to pressed-key feedback.
- When only an anchor such as Numbers or Symbols is held, the active command
  surface should light in that anchor's current-layer color.
- While only the anchor half of a chord is held, the thumb keys that can refine
  that anchor should light in the colors of their future layers, not in the color
  of the currently held anchor. For example, holding Numbers can show thumb hints
  for Navigation, Extremes, and Snap; holding Symbols can show hints for Media,
  Text, and System.
- Once a thumb refinement is pressed, the refined layer becomes the active state
  and its usable command surface should light in that layer's own color. The
  unused thumb hints should turn off; only the anchor key and the pressed thumb
  key should remain lit as the active chord path.
- If a command key toggles a persistent future state, it should use the color of
  that future state. For example, the Colemak toggle key on Function/System
  should use the same purple as persistent Colemak mode.
- Future-layer colors must be distinguishable in real use, not only in code.
  If two command concepts look too similar on the physical board, such as Snap
  and Function/System, change the color vocabulary before adding more cues.
- Do not rely on LEDs that are physically unavailable or consistently dark on
  the current board. If the leftmost columns or GUI thumb LED do not light,
  treat that as a physical display limitation rather than a behavior bug.
- On thumb-refined command layers, the Backspace position should light red when
  it has become Delete. Numbers and Symbols should not show that red Delete cue
  when Delete is not part of their current character-entry concept.

Active states override previews. The intended priority is whole-board modes,
active held layers, then off.

Red needs a clear visual grammar:

- Whole-board flat red means Caps Lock, because Caps changes ordinary typing
  globally. A pulsing red effect was considered, but the bytes are currently
  better spent on anchor and thumb-chord discoverability.
- A short whole-board red flash means "that key is blank on the active command
  layer." This is event feedback, not a persistent map, so blank keys stay quiet
  until they are actually pressed.

Colemak may use a distinctive whole-board pattern rather than a flat fill if the
pattern makes the persistent typing mode easier to notice and does not conflict
with Caps Lock or command-layer warning feedback.

When RGB does not match the layer concept, fix either the RGB mask or the layer
concept deliberately rather than adding a second visual grammar.

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
- Directional command layers deliberately use `I/J/K/L` rather than a pure
  home-row line because the spatial arrow shape is easier to remember and should
  carry across arrows, selection, and extremes.
- The old mixed Function layer has been removed from the active Corne build.
  Function keys now live with Numbers/Commands, mouse behavior lives on the
  Mouse layer, media behavior lives on the Media layer, and the old Function
  thumb position is now Base Menu.
- The Base corner Shift positions are now under review because the current
  workflow does not depend on them. Candidate requirement: use those corners for
  deliberate persistent/app-launch commands, such as Colemak on bottom-left and
  Calculator on bottom-right, if testing confirms the loss of corner Shifts is
  acceptable.
