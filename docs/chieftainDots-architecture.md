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
- Numbers/Commands: numbers and calculator operators on the right, accessed
  from `S` or `L`.
- Symbols: top-row punctuation and shifted symbols, with lower rows transparent
  so Base/Colemak letters still pass through while symbol access is held.
- Legacy Function: removed from the active Corne build recipe. Its useful jobs
  have moved into Numbers/Commands, Media, Text Snippets, Apps/System, and Base
  Menu.
- Navigation: arrow movement using the `I/J/K/L` spatial shape, accessed by
  holding `S` or `L` and the left GUI thumb.
- Extremes: Home/Page/End movement using the same `I/J/K/L` shape, accessed by
  holding `S` or `L` and the left Alt thumb.
- Snap: Windows GUI+Arrow window snapping using the same `I/J/K/L` shape,
  accessed by holding `S` or `L` and the left Space thumb.
- Media: volume and track controls, accessed by holding `A` and the left GUI
  thumb.
- Text Snippets: safe-to-type personal snippets, accessed by holding `A` and
  the left Alt thumb. Private string values must live in ignored local files.
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
- Feature modules own reusable behavior such as Tap Dance punctuation ladders,
  text snippets, combos, Caps Unlock, and future macros.
- OLED and RGB modules should report state or provide feedback; they should not
  become the owner of core key behavior.
- `process_record_user` should stay coordinated. New behavior should be routed
  deliberately so one feature does not silently block another.

Current source ownership:

| File or folder | Owns | Should not own |
| --- | --- | --- |
| `keymaps/corne.json` | Corne build recipe and layer order. | Key behavior details or feature logic. |
| `layout.h` | Layer tables, home-row wrappers, aliases, and cross-layer placement concepts. | RGB/OLED drawing rules or private string values. |
| `eldestroyer74.c` | Userspace hook coordination, timing callbacks, Caps Unlock call, OLED tap timer, and text snippet dispatch. | Large feature-specific state machines when a feature module would be clearer. |
| `features/` | Reusable behavior: Tap Dance, text snippets, Caps Unlock, disabled combos, and future macros. | Physical layer ownership beyond named keycodes exposed to `layout.h`. |
| `rgb/` | RGB state vocabulary, layer/chord indicators, physical LED mapping helpers. | Key behavior or alternate layer definitions. |
| `oled/` | Screen rendering and animation. | Core key behavior or RGB policy. |
| `docs/` | Principles, workflow, architecture, roadmap, and printable learning aids. | Independent source-of-truth tables that silently drift from the keymap. |

If a change needs to update more than one owner, name the source of truth first.
For example, layer behavior starts in `layout.h`; RGB may derive from the
keymap, and the printable guide should describe the result.

## RGB Scope Rule

RGB belongs to feedback, not behavior. It should report the active scope of the
keyboard state without becoming another place where key behavior is defined.

- Caps Lock is whole-board red because it changes ordinary typing globally.
- Colemak is whole-board purple because it is a persistent whole-board typing
  mode.
- Momentary command layers light their usable command surface. The mask should
  follow the active keymap where possible, with explicit visual exceptions for
  chord anchors, future thumb hints, and the Delete cue.
- Pressed global modifier RGB is currently disabled. It was removed because it
  lit ambiguous opposite-hand positions and did not clearly identify the
  physical key being held.
- Reactive per-key RGB is currently disabled by omitting
  `RGB_MATRIX_KEYPRESSES`. Restoring it costs roughly 372 bytes before any
  custom effect code and should be treated as a new feature.

Because ChieftainDots runs on a split Corne, RGB state also depends on split
state sync. `SPLIT_LAYER_STATE_ENABLE` is required for momentary layer RGB on
the non-master half, `SPLIT_LED_STATE_ENABLE` is required for Caps Lock RGB on
the non-master half, and `SPLIT_MODS_ENABLE` preserves cross-half modifier
awareness for OLED/status/future behavior. Keep those sync options explicit in
`config.h` unless a future feature deliberately accepts the tradeoff.

The current command-layer color vocabulary is deliberately small: Numbers is
blue, Symbols is cyan, Navigation is cyan, Extremes is yellow, Snap is white,
Media is dark teal, Text Snippets is dark pink, Function/System is fluorescent
green, Colemak is whole-board purple, and Caps is whole-board red. On
thumb-refined command layers, the Backspace position lights red when it is
available as Delete.

Corne's right-hand matrix columns are reversed from the visual key order. For
example, visual `Y U I O P BSPC` maps to matrix columns `5 4 3 2 1 0`.
RGB masks should use named visual-position helpers such as right-hand index,
middle, ring, and outer positions rather than raw column numbers. This keeps
the indicator code aligned with the layout a user sees.

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
The `S`/`L` numbered-command slice and its left-thumb refinements are active
firmware concepts.

The model:

- The anchor key selects a command family.
- A left thumb position refines that family into a related sub-layer.
- The right hand performs the command.

### `S` Family: Numbers And Editing

- Hold `S`: Numbers on the right hand.
- Hold `S` plus the left GUI thumb position: Arrows.
- Hold `S` plus the left Alt thumb position: extremes such as Home, End, Page
  Up, and Page Down.
- Hold `S` plus the left Space thumb position: snap shortcuts such as
  GUI+Left and GUI+Right.

Directional sub-layers use a right-hand spatial shape:

```text
      I = Up
J = Left   K = Down   L = Right
```

Extremes and snap should reuse this same physical shape. This intentionally
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

      I = Page Up
J = Home   K = Page Down   L = End

Hold S or L, then hold left Space thumb:

      I = GUI+Up
J = GUI+Left   K = GUI+Down   L = GUI+Right
```

Navigation and extremes keep `D = Ctrl` and `F = Shift` as plain held modifiers
for consistency. Selection is now composed from those plain modifiers plus
Navigation or Extremes, instead of being owned by a dedicated layer.

The Snap layer exists because the left GUI thumb is already used to enter
Navigation. It provides explicit GUI+Arrow commands without adding double-tap
timing to ordinary arrow keys.

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

Historical numbered-command refinement:

```text
TAB  Q    W   E   R   T       Y   U   I   O   P   BSPC
---  ---  --- --- --- ---     +   7   8   9   *   DEL

CAPS A    S   D   F   G       H   J   K   L   ;   '
---  ---  --- --- --- ---     -   4   5   6   /   .

LSFT Z    X   C   V   B       N   M   ,   .   /   RSFT
---  ---  --- --- --- ---     (   1   2   3   0   )
```

That older shape proved the right-hand number-pad idea, but function keys later
moved to the A-family Function/System layer and the number layer was redesigned
around traditional top-row memory.

Active Numbers layout:

```text
Hold S or L

`   1   2   3   4   5       6   7   8   9   0   -

--- --- --- --- SYM ---     --- 4   5   6   =   ---

--- --- --- --- --- ---     --- 1   2   3   /   ---
                            2 hold = ,   2 double tap = <
                            3 hold = .   3 double tap = >
```

Both `S` and `L` can access this layer. `S` supports left-hand anchor, right-hand
number entry. `L` remains useful as a right-hand access key for the same
numbered-command layer. The left-hand `F` position enters Symbols while Numbers
is held, so the layer can mimic shifted number-row symbols without adding a
second concept to the base number layer.

### `A` Family: Symbols And Tools

- Hold `A`: Symbols.
- Hold `A` plus the left GUI thumb position: Media and volume.
- Hold `A` plus the left Alt thumb position: Text Snippets.
- Hold `A` plus the left Space thumb position: Function keys and system actions.

Current A-family direction:

- Hold `A`: Symbols on the top row.
- Hold `A` plus the left GUI thumb position: Media.
- Hold `A` plus the left Alt thumb position: Text Snippets.
- Hold `A` plus the left Space thumb position: Apps/System, including Colemak
  toggle.

Implemented Symbols layout:

```text
Hold A or ;

TAB  Q  W  E  R  T      Y  U  I      O      P  BSPC
!    @  #  $  %  ^      &  *  (/[/{  )/]/}  _  DEL

Home row and bottom row: transparent to the active typing layer
Left thumbs: Media, Text Snippets, System refinements
```

The bracket keys use one physical concept: tap for round brackets, hold for
square brackets, and double tap for curly brackets.

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

Implemented Text Snippets layout:

```text
Hold A, then hold left Alt thumb:

Y = Home address           I = Phone
H = Work address   J = Meeting link   K = Email   L = Name

The committed source must not contain private address values. Local private
values live in `features/text_stubs_private.h`, which is ignored by git.
```

Implemented Function/System layout:

```text
Hold A, then hold left Space thumb:

Y   U   I   O   P   BSPC
F12 F7  F8  F9  --- DEL

H   J   K   L   ;   '
F11 F4  F5  F6  --- Colemak toggle

N   M   ,   .   /   RSFT
F10 F1  F2  F3  --- ---
```

The System layer intentionally avoids duplicating clean Windows shortcuts and no
longer acts as a general app-launch layer. Function/System uses a momentary
command-layer RGB color distinct from persistent whole-board Colemak RGB.
Calculator and Media Player launch ideas are handled by Base lower-corner keys.

### Spatial Consistency

Command layers should reuse physical meaning where possible:

- Paired vertical concepts should stay aligned where useful. Current Text
  Snippets rule: `Y = Home address` and `H = Work address`.
- Multiplication and division should remain a vertical pair. Current rule:
  `P = *` sits above `; = /`.
- Related actions such as arrows, selection arrows, and Home/End/Page movement
  should keep directional meaning across sub-layers.

Text snippet placement:

```text
Y = Home address           I = Phone
H = Work address   J = Meeting link   K = Email   L = Name
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
