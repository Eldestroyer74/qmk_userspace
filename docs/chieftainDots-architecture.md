# ChieftainDots Architecture

## Current Summary

ChieftainDots is currently a Corne-first keyboard project built from
`keymaps/corne.json`. The current compiled trial uses two typing layers, three
home-row anchors, one GUI child per anchor, RGB as state feedback, and OLED as a
display subsystem rather than an owner of key behavior.

- Base and Colemak are the only layers wrapped with `HRM(...)`.
- `D`/`K` own Numbers; `Numbers + GUI` owns Navigation.
- `S`/`L` own Symbols; `Symbols + GUI` owns Office.
- `A`/`;` own Function/System; `Function + GUI` owns Media.
- Snap is a Ctrl+GUI chord so it uses the same visible chord grammar as the
  other command layers.
- Text snippets are mnemonic double-tap-hold gestures on Base/Colemak letters,
  not a daily layer chord.
- Spanish stays on the lower corners, with double-tap language switching.
- Future keyboard ports should add fresh recipes and wrappers around this model.
- Historical Filterpaper recipes are reference material only.

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

Only `keymaps/corne.json` is considered the active ChieftainDots build target.
The older Filterpaper-inherited `keymaps/cradio.json` and
`keymaps/technik.json` recipes were removed because they referenced the removed
`_FUNC` layer and no longer described current ChieftainDots. Future keyboard
recipes should be created from the current layer model when a real port starts.

## Current Layer Concepts

- Base: QWERTY typing layer with familiar outer modifiers and thumb keys.
  The old right-thumb Function position now sends the Application/context-menu
  key for typo fixes and right-click style menus.
- Colemak: alternate typing layer toggled from the Apps/System layer.
- Numbers: number-row memory plus a right-hand number pad, accessed from `D` or
  `K`. Holding either thumb while in Numbers reaches Symbols.
- Symbols: top-row punctuation and shifted symbols, accessed from `S` or `L`.
- Function/System: function keys in a Numbers-like shape, accessed from `A` or
  `;`.
- Navigation: movement using the `I/J/K/L` spatial shape, accessed by holding
  Numbers and the left GUI thumb. Target behavior: tap sends arrows, hold holds
  arrows, double tap sends semantic jumps, and double-tap-hold sends movement
  extremes.
- Office: Office-content editing using the `I/J/K/L` spatial shape, accessed by
  holding Symbols and the left GUI thumb. Target behavior: tap sends arrows,
  hold sends `Alt+Shift+Arrow` for PowerPoint/Word structure, double tap sends
  selection movement, and double-tap-hold sends extreme selection.
- Media: Teams-first meeting/media controls, accessed from Function/System plus
  the left GUI thumb. Meeting mute is the main expected use, with global volume
  and playback controls as secondary behavior.
- Snap: Windows snap traversal, accessed from the Ctrl+GUI chord so RGB and
  OLED can explain it consistently with the other command layers. Important
  displaced Windows virtual-desktop shortcuts live deliberately inside the
  layer rather than relying on raw pass-through.
- Text Snippets: safe-to-type personal snippets on mnemonic Base/Colemak
  double-tap-hold letters. Private string values must live in ignored local
  files.
- Spanish: accented letters, punctuation, angle quotes, and input-language
  switching from the lower-corner Spanish keys.

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
| future `keymaps/*.json` recipes | Keyboard-specific recipe, wrapper layout, and deliberate layer list for a tested port. | Inherited stale layers or behavior copied without compile/testing. |
| `features/layers.h` | Shared numeric layer IDs used by layout, feature modules, RGB, and OLED when they need to reference layers in C/preprocessor code. | Key placement, layer contents, or behavior. |
| `layout.h` | Layer tables, home-row wrappers, aliases, and cross-layer placement concepts. | RGB/OLED drawing rules or private string values. |
| `eldestroyer74.c` | Userspace hook coordination, timing callbacks, Caps Unlock call, OLED tap timer, and text snippet dispatch. | Large feature-specific state machines when a feature module would be clearer. |
| `features/` | Reusable behavior: Tap Dance, text snippets, Caps Unlock, disabled combos, and future macros. | Physical layer ownership beyond named keycodes exposed to `layout.h`. |
| `rgb/` | RGB state vocabulary, layer/chord indicators, physical LED mapping helpers. | Key behavior or alternate layer definitions. |
| `oled/` | Screen rendering and animation. | Core key behavior or RGB policy. |
| `docs/` | Principles, workflow, architecture, roadmap, and printable learning aids. | Independent source-of-truth tables that silently drift from the keymap. |

If a change needs to update more than one owner, name the source of truth first.
For example, layer behavior starts in `layout.h`; RGB may derive from the
keymap, and the printable guide should describe the result.

## OLED Status Grammar

OLED belongs to feedback, not behavior. The current direction keeps the
Filterpaper-style status structure but renames it around ChieftainDots concepts:

- The status panel has a top logo/text slot, a fixed middle layer/state tile,
  and a bottom 2x2 home-row status stack.
- The middle tile is named by role, not by historical art source:
  `base_layer` for resting typing, `anchor_layer` for a held home-row anchor,
  and `chord_layer` for a completed chord.
- The tile stays in a fixed position. The image changes; the same Base tile
  should not slide around the screen to imply depth.
- The bottom 2x2 stack reuses the old modifier-panel grammar to show the four
  home-row concepts: Function/System, Symbols, Numbers, and Control.
- The small text slot where `corne` appears is the cheap state-name slot. Every
  chorded layer should use the same label grammar once it exists in the active
  trial: `nav`, `style` for Office, `snap`, and `media` if Media is restored.
- Avoid full-screen raw label renderers for chord names. The first trial worked
  but was slow and cost bytes; the small text-slot approach keeps Bongocat from
  being overwritten and leaves more firmware headroom.

The design brief at `docs/chieftainDots-oled-screen-brief.html` is the
collaboration surface for screen changes. Update the brief before changing more
firmware art, then compile-measure the firmware slice.

## Porting To Other Keyboards

ChieftainDots can support other keyboards later, but a new recipe should start
from the current Corne concepts rather than an old inherited file. Treat a port
as a deliberate feature with its own compile target, firmware-size check, and
physical test.

Porting checklist:

- Confirm the QMK keyboard name and supported layout macro.
- Count the physical keys and thumbs.
- Decide which ChieftainDots roles must remain identical.
- Decide which roles move because the keyboard has fewer or more keys.
- Add a fresh wrapper in `layout.h` only when the recipe needs one.
- Compile and test the new target before calling it supported.

Example for a slightly smaller keyboard:

```json
{
  "keyboard": "example/smaller_board",
  "keymap": "eldestroyer74",
  "layout": "LAYOUT_chieftaindots_small",
  "layers": [
    [ "SMALL(HRM(_BASE))" ],
    [ "SMALL(HRM(_COLE))" ],
    [ "SMALL(_NUMB)" ],
    [ "SMALL(_SYMB)" ],
    [ "SMALL(_SYST)" ],
    [ "SMALL(_NAV)" ],
    [ "SMALL(_EXTR)" ],
    [ "SMALL(_MEDI)" ],
    [ "SMALL(_SNP)" ],
    [ "SMALL(_SPAN)" ]
  ]
}
```

In that model, `SMALL(...)` would be a new wrapper that drops or relocates
lower-priority physical positions while preserving the layer concepts.

Example for a slightly larger keyboard:

```json
{
  "keyboard": "example/larger_board",
  "keymap": "eldestroyer74",
  "layout": "LAYOUT_chieftaindots_large",
  "layers": [
    [ "LARGE(HRM(_BASE))" ],
    [ "LARGE(HRM(_COLE))" ],
    [ "LARGE(_NUMB)" ],
    [ "LARGE(_SYMB)" ],
    [ "LARGE(_SYST)" ],
    [ "LARGE(_NAV)" ],
    [ "LARGE(_EXTR)" ],
    [ "LARGE(_MEDI)" ],
    [ "LARGE(_SNP)" ],
    [ "LARGE(_SPAN)" ]
  ]
}
```

In that model, `LARGE(...)` would add convenience keys around the existing
concepts rather than inventing a different keyboard philosophy.

A larger board such as the Mountain Ergo is a good candidate for this approach.
The useful inherited idea is not the old Filterpaper helper itself, but the
pattern: keep the ChieftainDots core stable, then use a wrapper to place that
core inside a larger physical layout. Extra keys should be assigned deliberately
as convenience keys, duplicates, or board-specific experiments.

The removed Filterpaper-era conversion helpers are preserved in git for
reference at tag `filterpaper-layout-wrappers-20260515`. To inspect the old
helper code without restoring it:

```bash
git show filterpaper-layout-wrappers-20260515:layout.h
```

To restore one old helper for study, copy it from that tagged version into a
new branch and rewrite it around the current ChieftainDots layers before
compiling. Do not reintroduce the old helper unchanged; it encoded stale
Filterpaper placement decisions.

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
blue, Symbols is amber, Navigation is cyan, Extremes is yellow, Snap is white,
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

### Three Anchors And Chords

The current trial has evolved from older two-family thumb-refinement language
to three visible anchors plus one chord per anchor, with Snap as the separate
Ctrl+GUI command chord:

- Hold `D` or `K`: Numbers. GUI chord: Navigation.
- Hold `S` or `L`: Symbols. GUI chord: Office.
- Hold `A` or `;`: Function/System. GUI chord: Media.
- Hold Control plus GUI: Snap.

When this model changes, update `layout.h`, `keymaps/corne.json`, RGB, OLED
status, the printable guide, and this section as one design slice.

### Command Chord Hierarchy Requirement

The command layers should use one common gesture vocabulary wherever it makes
sense:

- Tap: the direct, safest daily action.
- Tap-and-hold: the related sustained, larger, or repeated action.
- Double tap: the secondary but still reversible action.
- Double-tap-and-hold: leave empty until a real need is proven, especially for
  destructive or broad host-state changes.

Current requirements by layer:

```text
Navigation, entered from Numbers + GUI

Tap = arrows
      Up
Left  Down  Right

Hold = held arrows
      hold Up
hold Left  hold Down  hold Right

Double tap = semantic jumps
      Ctrl+Up
Ctrl+Left  Ctrl+Down  Ctrl+Right

Double-tap-hold = movement extremes
      Page Up
Home  Page Down  End
```

```text
Office, entered from Symbols + GUI

Tap = arrow navigation inside Office content
      Up
Left  Down  Right

Hold = PowerPoint / Word structure
      Alt+Shift+Up
Alt+Shift+Left  Alt+Shift+Down  Alt+Shift+Right

Double tap = select word / line
      Shift+Up
Ctrl+Shift+Left  Shift+Down  Ctrl+Shift+Right

Double-tap-hold = select extremes
      Shift+Page Up
Shift+Home  Shift+Page Down  Shift+End
```

```text
Snap, Ctrl+GUI chord

Tap = snap current window
      Win+Alt+Up
Win+Left  Win+Alt+Down  Win+Right

Hold = current-window state or monitor movement
      Win+Up
Win+Shift+Left  Win+Down  Win+Shift+Right

Double tap = desktop/workspace manipulation
      Win+Tab
Win+Ctrl+Left  Win+D  Win+Ctrl+Right

Double-tap-hold = intentionally empty for the first trial
```

```text
Media, Function/System + GUI chord, Teams-first

Tap = daily meeting/media controls
      Volume Up
Teams mic mute  Volume Down  Play/Pause

Hold = sustained/system controls
      hold/repeat Volume Up
system speaker mute  hold/repeat Volume Down  no-op or Play/Pause

Double tap = track movement
      ---
Previous Track  ---  Next Track

Double-tap-hold = intentionally empty
```

### Current Implementation Notes

- Navigation is implemented as `_NAV` with `NAV_*_DANCE` in
  `features/tap_dance.c`. It is movement-first: tap arrows, hold held arrows,
  double-tap semantic jumps, and double-tap-hold movement extremes.
- Office is implemented on the older `_EXTR` layer id for compatibility with
  the existing layer stack, but user-facing docs and OLED labels call it
  `style`. It uses `OFFICE_*_DANCE`: tap plain arrows, hold
  `Alt+Shift+Arrow`, double-tap selection movement, and double-tap-hold extreme
  selection.
- Snap is implemented as `_SNP`, entered by holding the Control anchor and the
  exposed GUI thumb chord. It uses `SNAP_*_DANCE` so the same right-hand arrow
  shape can send snap, monitor, and desktop commands.
- Media is resurrected as `_MEDI`, entered by Function/System plus GUI. It is a
  deliberately small Teams-first layer: volume up/down, Teams mic mute,
  system mute, play/pause, and previous/next track.

Settled decisions for the current chord slice:

- Navigation Up/Down semantic jumps use `Ctrl+Up/Down`.
- Navigation Up/Down movement extremes use `Page Up/Page Down`.
- Office Up/Down extreme selection uses `Shift+Page Up/Page Down`.
- Snap lives on the Ctrl+GUI chord.
- Media returns in this slice to keep four anchors with one visible chord each.
- Snap includes `Win+Tab`, `Win+Ctrl+Left`, `Win+D`, and `Win+Ctrl+Right`;
  `Win+Ctrl+F4` is intentionally omitted from the first trial.
- Navigation, Office, Snap, and Media are implemented together so the physical
  model, RGB, and OLED status grammar can be tested as one coherent trial.

### Movement And Snap

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

The Snap layer is entered with Ctrl+GUI because the left GUI thumb already
enters the anchor child layers. Ctrl+GUI makes Snap a visible chord instead of
a hidden GUI double-tap-and-hold mode. Displaced Windows virtual desktop
shortcuts are added back deliberately inside Snap: Ctrl+GUI+Left/Right for
switching virtual desktops and Ctrl+GUI+D for showing the desktop. Ctrl+GUI+F4
is intentionally omitted from the first trial because closing a virtual desktop
is destructive enough to deserve its own later decision.

Implemented Snap hierarchy:

```text
Tap = snap current window
      Win+Alt+Up
Win+Left   Win+Alt+Down   Win+Right

Hold = current-window state or monitor movement
      Win+Up
Win+Shift+Left   Win+Down   Win+Shift+Right

Double tap = desktop/workspace manipulation
      Win+Tab
Win+Ctrl+Left   Win+D   Win+Ctrl+Right
```

Leave double-tap-hold empty in the first Snap-as-chord trial. `Win+Alt+Up/Down`
depends on Windows Snap Layout support, so top/bottom snap needs a real host
test before this requirement is marked kept.

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

### Symbols, Function, And Media

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

Historical Text Snippets layer layout:

```text
Hold A, then hold left Alt thumb:

Y = Home address           I = Phone
H = Work address   J = Meeting link   K = Email   L = Name

The committed source must not contain private address values. Local private
values live in `features/text_stubs_private.h`, which is ignored by git.
```

Current mnemonic text snippets:

```text
Double tap and hold the second tap on Base/Colemak:

G = Personal email
H = Home address
W = Work address
P = Phone
M = Meeting link
E = Email
N = Name
```

Private snippet values are intentionally split from the committed source:

- Committed source defines the public contract in `features/text_stubs.h`.
- `features/text_stubs_private.example.h` is the safe template.
- `features/text_stubs_private.h` is ignored by git and holds the real values.
- If the ignored file is missing, firmware falls back to placeholders such as
  `[personal-email]`, `[home-address]`, and `[meeting-link]`.

To recreate the ignored file, copy the example file and replace only the string
contents with local values:

```c
#pragma once

#define TEXT_STUB_EMAIL "work@example.com"
#define TEXT_STUB_PERSONAL_EMAIL "personal@example.com"
#define TEXT_STUB_PHONE "0000 000 000"
#define TEXT_STUB_NAME "Your Name"
#define TEXT_STUB_MEETING "https://example.com/meeting"
#define TEXT_STUB_WORK "Work address"
#define TEXT_STUB_HOME "Home address"
```

Never put real addresses, phone numbers, meeting links, personal email
addresses, passwords, recovery keys, or private URLs in committed files. Before
staging text-snippet changes, verify `features/text_stubs_private.h` is still
ignored and inspect the staged diff for real values.

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

Implemented Media chord:

```text
Hold Function/System, then chord GUI:

Tap = Teams-first daily controls
      I = Volume Up
J = Teams mic mute   K = Volume Down   L = Play/Pause

Hold = sustained/system controls
      I = hold/repeat Volume Up
J = system speaker mute   K = hold/repeat Volume Down   L = no-op or Play/Pause

Double tap = track movement
      ---
Previous Track   ---   Next Track
```

Keep Media intentionally small. Do not restore a broad music-player layer unless
real use proves it is needed. Teams microphone mute should send Teams'
`Ctrl+Shift+M`; system speaker mute should stay separate as `KC_MUTE`.

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
