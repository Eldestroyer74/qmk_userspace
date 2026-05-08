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
  The old right-thumb Function position now sends the Application/context-menu
  key for typo fixes and right-click style menus.
- Colemak: alternate typing layer toggled from the Apps/System layer.
- Numbers/Commands: function keys on the left using old number-layer memory,
  numbers and calculator operators on the right, accessed from `S` or `L`.
- Symbols: punctuation, brackets, braces, and shifted symbols.
- Legacy Function: removed from the active Corne build recipe. Its useful jobs
  have moved into Numbers/Commands, Media, Mouse, Apps/System, and Base Menu.
- Navigation: arrow movement using the `I/J/K/L` spatial shape, accessed by
  holding `S` or `L` and the left GUI thumb.
- Selection: Ctrl+Shift arrow movement using the same `I/J/K/L` shape, accessed
  by holding `S` or `L` and the left Alt thumb.
- Extremes: Home/Page/End movement using the same `I/J/K/L` shape, accessed by
  holding `S` or `L` and the left Space thumb.
- Media: volume and track controls, accessed by holding `A` and the left GUI
  thumb.
- Mouse: pointer movement, scroll, and primary/secondary mouse buttons,
  accessed by holding `A` and the left Alt thumb.
- Apps/System: persistent application/system toggles such as Colemak, accessed
  by holding `A` and the left Space thumb.

## Cross-Layer Key Roles

Some physical positions should keep a broad family meaning even when the exact
keycode changes by layer. The Backspace position is one of these: Base should use
Backspace for typing correction, while non-base command layers should use Delete
where practical because those layers are used for editing and command work.

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

## Two Anchor Families Exploration

One candidate direction is to use home-row anchors plus left-thumb refinements.
The `S`/`L` numbered-command slice is implemented and being trialed; the
left-thumb refinement layers are not implemented yet.

The model:

- The anchor key selects a command family.
- A left thumb position refines that family into a related sub-layer.
- The right hand performs the command.

### `S` Family: Numbers And Editing

- Hold `S`: Numbers on the right hand.
- Hold `S` plus the left GUI thumb position: Arrows.
- Hold `S` plus the left Alt thumb position: selection shortcuts such as
  Ctrl+Shift+Arrows.
- Hold `S` plus the left Space thumb position: extremes such as Home, End, Page
  Up, and Page Down.

Directional sub-layers use a right-hand spatial shape:

```text
      I = Up
J = Left   K = Down   L = Right
```

Selection and extremes should reuse this same physical shape. This intentionally
deviates from a strict home-row-only rule because the spatial direction pattern
is more memorable and easier to carry across related editing layers.

First implemented refinement:

```text
Hold S or L, then hold left GUI thumb:

      I = Up
J = Left   K = Down   L = Right
```

Additional implemented refinements:

```text
Hold S or L, then hold left Alt thumb:

      I = Ctrl+Shift+Up
J = Ctrl+Shift+Left   K = Ctrl+Shift+Down   L = Ctrl+Shift+Right

Hold S or L, then hold left Space thumb:

      I = Page Up
J = Home   K = Page Down   L = End
```

Navigation and extremes keep `D = Ctrl` and `F = Shift` as plain held modifiers
for consistency. Selection leaves the left hand blank because the selected
commands already include Ctrl+Shift.

Known conflict: the left GUI thumb is also an operating-system modifier. Because
it currently refines `S`/`L` into Navigation, ordinary keyboard-only window
snapping such as GUI+Left and GUI+Right is no longer directly available from the
arrow shape. Future navigation revisions must either add explicit window-snap
commands, move Navigation off GUI, or provide another documented OS-window route.

First-slice number candidate:

```text
Y   U   I   O   P   BSPC
+   7   8   9   *   blank

H   J   K   L   ;   '
-   4   5   6   /   .

N   M   ,   .   /   RSFT
(   1   2   3   0   )
```

The first slice proved the right-hand number pad. It kept the left side blank
while the number pad itself was tested.

Active numbered-command refinement:

```text
TAB  Q    W   E   R   T       Y   U   I   O   P   BSPC
---  F12  F7  F8  F9  ---     +   7   8   9   *   DEL

CAPS A    S   D   F   G       H   J   K   L   ;   '
---  F11  F4  F5  F6  ---     -   4   5   6   /   .

LSFT Z    X   C   V   B       N   M   ,   .   /   RSFT
---  F10  F1  F2  F3  ---     (   1   2   3   0   )
```

This keeps the old number/function memory: `F7/F8/F9`, `F4/F5/F6`, and
`F1/F2/F3` sit where old `7/8/9`, `4/5/6`, and `1/2/3` lived. `F10`, `F11`, and
`F12` sit to the left of their corresponding function rows. Tab, Caps, and Shift
positions are blank on this layer because they do not belong to the numbered
command concept. The Backspace position becomes Delete under the cross-layer
editing-key rule.

Both `S` and `L` can access this layer. `S` supports left-hand anchor, right-hand
number entry. `L` remains useful as a right-hand access key for the function-key
side of the same numbered-command layer.

### `A` Family: Symbols And Tools

- Hold `A`: Symbols.
- Hold `A` plus the left GUI thumb position: Function keys and system actions.
- Hold `A` plus the left Alt thumb position: Mouse.
- Hold `A` plus the left Space thumb position: Media and volume.

Current A-family direction:

- Hold `A`: Symbols stay as-is for now.
- Hold `A` plus the left GUI thumb position: Media.
- Hold `A` plus the left Alt thumb position: Mouse.
- Hold `A` plus the left Space thumb position: Apps/System, including Colemak
  toggle.

Agreed media layout:

```text
Hold A, then hold left GUI thumb:

Y = Play/Pause
H = Mute
        I = Volume Up
J = Previous   K = Volume Down   L = Next
```

Visually:

```text
Y      U   I      O   P   BSPC
Play   --- Vol+   --- --- DEL

H      J      K      L     ;   '
Mute   Prev   Vol-   Next  --- ---

N   M   ,   .   /   RSFT
--- --- --- --- --- ---
```

This deliberately treats Play/Pause and Mute like the calculator/media operator
pair: `Y` carries the positive/additive action and `H` carries the
negative/suppressing action. Right Shift is unused on this layer.

Implemented mouse layout:

```text
Hold A, then hold left Alt thumb:

Y = Wheel Up             I = Mouse Up
H = Wheel Down   J = Mouse Left   K = Mouse Down   L = Mouse Right

Right thumb Enter position = Mouse Button 1
Right thumb old Function/Menu position = Mouse Button 2
```

Implemented apps/system layout:

```text
Hold A, then hold left Space thumb:

Any right home-row key = Colemak toggle
Backspace position = Delete
```

The System layer intentionally avoids duplicating clean Windows shortcuts and no
longer acts as a general app-launch layer. Calculator and Media Player launch
ideas are kept in the roadmap for a separate launcher design.

### Spatial Consistency

Command layers should reuse physical meaning where possible:

- Positions chosen for calculator plus/minus should correspond to mouse scroll
  up/down. Current rule: `Y = +` corresponds to `Y = Wheel Up`; `H = -`
  corresponds to `H = Wheel Down`.
- Multiplication and division should remain a vertical pair. Current rule:
  `P = *` sits above `; = /`.
- Positions chosen for Enter, confirm, or accept should correspond to primary
  mouse button behavior where that makes sense.
- Related actions such as arrows, selection arrows, and Home/End/Page movement
  should keep directional meaning across sub-layers.

Mouse wheel/operator consistency:

```text
Numbers/Commands      Mouse
Y = +                 Y = Wheel Up
H = -                 H = Wheel Down
P = *                 Multiplication stays above division
; = /                 Division stays below multiplication
```

### Implementation Direction

The likely readable implementation is ordinary QMK layer stacking: `S` or `A`
enters the base family layer, and the left thumb positions on that layer
temporarily enter related sub-layers. Avoid solving this with QMK Combos or
custom state machines unless plain layers prove inadequate.

This idea may require revising older principles. It prefers more narrowly named
layers over fewer overloaded layers. It also treats thumbs as layer refinements
inside an anchor family instead of only ordinary modifiers.

### Ideas Considered And Set Aside

- Left thumb Space as `LT(NUM, KC_SPC)`: common in ergonomic layouts, but risks
  Space timing issues.
- Left thumb GUI or Alt as a Number hold: rejected for now because GUI and Alt
  need to remain reliable held modifiers for OS/application shortcuts.
- Caps as a layer hub: promising because it preserves thumbs, but it touches
  Caps Lock, Caps RGB, and `CAPS_UNLOCK`, so it is a larger design change.
- QMK Combos for numbers/navigation/text: disabled for now after ergonomic trial
  feedback. Reachable combos were not necessarily comfortable combos.

## Anti-Spaghetti Rules

- One layer should have one memorable concept.
- One feature should own one behavior.
- Cross-feature dependencies must be written down before implementation.
- If a feature needs to change a principle, update the principle first or record
  the principle decision in the roadmap.
