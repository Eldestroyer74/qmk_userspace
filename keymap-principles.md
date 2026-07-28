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
- Home-row modifiers/layers are implemented, but the exact outside-to-inside
  order is still experimental. The current trial is Control, Symbols, Numbers,
  Function on `A/;`, `S/L`, `D/K`, and `F/J`.

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

When thumb timing becomes the main source of errors, prefer making the most
frequent thumb taps boring again. Space and Enter may be less suitable for
tap-hold modifiers than less-frequent thumb positions; a future trial may move
Shift to plain middle-thumb keys and move Alt to Space/Enter holds if that makes
ordinary typing calmer.

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

Prefer placing the most frequent, timing-sensitive, or error-costly home-row
anchors on the strongest and most coordinated fingers. When that conflicts with
mirrored mnemonic order, document which value wins for the trial and test the
weaker-side cost physically.

### Keep Command Layers Literal

Command layers should remain predictable. Do not apply broad typing-layer
wrappers, such as `HRM(...)`, to Numbers/Nav, Symbols, Function, or future
editing layers unless a specific feature is deliberately designed and tested for
that behavior.

If a command layer needs selection or modifier behavior, prefer explicit named
commands, explicit modifier positions, or a dedicated editing layer. The layer
table should still explain what the key does without requiring the user to
remember nested tap-hold rules.

### Distinguish Modifiers From Anchors

Traditional modifiers such as Ctrl, Shift, Alt, and GUI are keyboard ingredients,
not ChieftainDots command layers. They should keep their normal host behavior
and should not be visually or conceptually promoted into custom layers just
because they change what another key does.

ChieftainDots anchors exist to make up for missing physical keys on the Corne:
Symbols, Numbers, Function/System, Snap, Navigation, Media, Office, and similar
surfaces may use stronger OLED/RGB discovery feedback because they are custom
command modes. Plain modifiers may have feedback, but it should stay quieter and
physically grounded. If a future design treats a modifier like a layer, push back
first and decide whether the user-facing benefit is worth blurring this
distinction.

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
- Shared constants that must be visible to both preprocessor code and C feature
  modules should live in a small shared header, not in duplicated numeric
  literals. For example, layer ids belong in `features/layers.h` so `layout.h`,
  Tap Dance, RGB, and OLED can all refer to the same names.

### Make Diagrams Truthful Before Decorative

The guide is a reference for real behavior. Its key positions, tap and hold
legends, chord members, and repeated thumb behavior must be derived from the
current keymap data or checked against `layout.h`. Styling may establish visual
hierarchy, but it must never replace, hide, or contradict a functional payload.

In particular:

- A home-row anchor or sibling-family cue must not replace the action shown in
  the centre of a functional key. Show entry grammar with a border, corner mark,
  header cue, or other secondary annotation that leaves the payload intact.
- Use the established paper-guide entry grammar in both modes: highlight only
  the left-hand representative anchor inside the keyboard drawing, and state
  the complete mirrored access pair in the card header (`A or ;`, `S or L`,
  `D or K`, or `F or J`). Do not add a second highlighted anchor or sibling
  family cues merely to make the keyboard drawing symmetrical. This keeps the
  right-hand positions available for their real payloads while still recording
  every valid route into the surface.
- Icons may replace words only when their meaning is established and legible at
  the intended viewing size. Keep a short text or corner-letter cue where the
  icon alone could be ambiguous.
- Renderer-only reference material must be labelled as such. It must not look
  like firmware behavior when it describes a host convention or mnemonic.

### Distinguish Keyboard Maps From Reference Overlays

Layer cards depict what the firmware produces at physical key positions.
Reference overlays may depict useful conventions that are not QMK layers, but
they must be explicit about that different contract.

The desktop `CONTROL` card is an intentional exception to the normal modifier
principle: it is a first-class end-user command reference because common Ctrl
shortcuts are useful to the person using the guide. The user-facing diagram
must not expose or visually weaken the card because Control happens to be a
modifier rather than a QMK layer; that distinction is an internal source and
verification concern. Present CONTROL with the same coherent card grammar as
the other command concepts. Its descriptor is `common shortcuts`; its header
retains `A or ;` as the physical Control access pair and owns the entry
explanation. Inside the CONTROL keyboard, shortcut keys including `A` and `F`
use the same dark payload background. Each shortcut uses a CONTROL-specific
three-part anatomy: letter at top centre, slightly smaller action icon in the
middle, and one short action word beneath it. The active `A` is the exception:
it follows the standard large centred anchor-letter grammar and keeps Select All
only as a small corner icon. The card badge and `A` anchor share one
unmistakable CONTROL accent. An available child-chord key uses the shared opaque
affordance treatment, not the destination colour; the destination family colour
is reserved for the selected chord/current child card. The other payloads
retain the common dark navy surface. Do not highlight the mirrored `;` inside
the drawing or add a redundant subtitle beneath CONTROL.

On every command card, the four left home-row seats `A S D F` follow one visual
precedence: the pressed/current anchor uses its active semantic colour; a seat
with a real key payload uses the dark payload treatment and keeps its legend;
otherwise the empty seat uses the shared opaque affordance treatment. Never
make an unoccupied left home-row seat look like an ordinary unused translucent
key, and never replace a real payload merely to make the four backgrounds
match.

Desktop command-card headers use a single information split: the title on the
left identifies the concept, and the complete access instruction on the right
explains how to reach it. Do not repeat `hold`, `+ GUI`, or another access
fragment beneath the title. Base/Colemak may retain useful typing-mode
descriptors because those describe the surface rather than duplicating a chord.
Paper remains unchanged unless separately revised.

Side-column card heights should distribute the fixed column envelope according
to content density rather than giving the alpha card unused space while utility
cards crowd their bottom edges. Preserve symmetric left/right columns, stable
outer margins and gaps, and canonical keyboard scale. Prefer reallocating height
within the existing envelope over moving individual keyboards by arbitrary
offsets.

Opaque semantic keys use a soft-white foreground. This includes active anchor
letters, selected chord-member icons, and available chord-affordance icons. The
background—not a dim foreground—distinguishes active semantic colour from the
shared affordance surface. Reserve muted blue-grey foregrounds for translucent
inherited keys; unused translucent wells carry no icon.

Desktop Key Anatomy teaches only non-obvious notation: the existing corner
legend anatomy, an active anchor with white foreground, and an opaque chord
affordance with white icon. Do not add permanent samples for inherited and
unused wells; their icon-versus-no-icon distinction is self-explanatory and
remains documented in the material principles.

Key Anatomy examples should reuse real diagram vocabulary where practical. The
active-anchor sample uses the same bright blue `D` as the desktop Numbers card,
so the legend reinforces a real anchor rather than inventing an isolated generic
`A` state.

Internally, keep the chosen conventional shortcut vocabulary in one named
renderer-owned dataset rather than pretending it is firmware-defined behavior
or duplicating it across layout tables, paper data, firmware, or documentation
tables. Reuse the shared physical keyboard geometry and verify the real Control
access anchors against `layout.h`. The CONTROL shortcut arrangement is the
Base/QWERTY reference; its letter cues name the shortcuts and it must not claim
to be a separate literal Colemak physical map.

### Use Visual Material Semantically

Desktop materials should explain state, not merely add polish. Translucency
primarily communicates absence, with one deliberate exception for inherited
unchanged behavior. Any key that communicates a payload, entry point, selected
chord, future chord affordance, wayfinding role, or layer-specific change must
be opaque and visibly distinct from both its card surface and an unused etched
well.

- Transparent etched wells mean genuinely unused positions.
- Payload keys use an opaque cool slate/navy surface with sufficient fill-value
  and boundary contrast to remain distinct over both the darkest background and
  the brightest cyan/violet arcs. Opacity alone is not evidence of legibility.
- Active anchors and selected chord members use the strongest opaque semantic
  family treatment.
- Chord affordances remain opaque because they communicate an available route,
  but use a quieter family tint than selected chord members. Do not use
  translucency to express their lower priority.
- Repeated or inherited unchanged thumbs may use the same translucent well as
  unused positions, but must retain their muted icon and any essential hold
  legend. The icon distinguishes "still available as normal" from an empty
  position. This is the desktop equivalent of paper's quiet-grey repeated-thumb
  treatment and lets opaque chord affordances and changed thumbs carry the
  learning hierarchy.
- Opaque keys may use a consistent restrained outline and top-edge highlight to
  separate them from the card, but must not gain independent glow, blur, raised
  shadow, or gaming-button styling.
- A card samples one continuous blurred background pane. Individual keys do not
  create independent blur, glow, external shadow, or raised gaming-button
  effects.

### Preserve Desktop Information Hierarchy

The desktop guide is a glanceable wallpaper, not a uniformly weighted poster.
Numbers, Symbols, Function, and Media are the primary central reference group.
Base and Colemak are secondary. Control, Navigation, MS Styles, and Snap are
quiet utility cards. Key Anatomy remains clear enough to decode the notation.

Implement hierarchy by controlling panel, border, heading, accent, key, and
legend strength separately. Do not fade an entire card uniformly, and do not
allow bright background arcs to make a quiet side card compete with the central
four. All essential legends must remain readable at the intended monitor size;
phone crops are review aids, not proof of full-wallpaper legibility.

### Keep Mode Scope Deliberate

Paper and desktop share keymap truth and keyboard geometry, but may use
different presentation rules. Paper remains the stable print reference unless a
requirement explicitly changes it. Desktop may use responsive placement,
semantic icons, smoked glass, and stronger information hierarchy. A desktop
refinement must not silently regenerate or alter paper output.

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

Good practice can save bytes, but only indirectly. The useful pattern is:

1. Make ownership clearer.
2. Remove or centralize duplicated structure.
3. Re-run the build.
4. Keep the change only if behavior is preserved and the byte result is known.

Do not assume a readability cleanup saves firmware space. Preprocessor aliases,
comments, ignored files, archived files, and source that is not included in the
active build usually cost attention rather than flash. They may still be worth
cleaning, but they should not be recorded as byte wins unless a compile proves
it. The best byte-saving cleanups usually expose compiled dead behavior, such as
an unreachable layer table, an unused handler path, an enabled QMK feature flag,
or custom state that duplicates a simpler keymap or QMK mechanism.

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
- Printable guides should de-emphasize repeated thumb behavior. Thumb keys that
  behave the same as the normal thumb model should use the same quiet grey as
  blank keys; active chord thumbs should stay in the chord color, and
  layer-specific thumb changes should stay dark/high-contrast so the changed
  behavior is the thing the eye learns.
- Printable guide chord affordances should be visually quieter than active
  chord members. A parent-card thumb that previews a future chord may keep the
  pale future-layer cue background, but its icon should use the muted thumb-row
  grey rather than the destination layer's saturated text color.
- Diagram styling must not override behavior truth. Before muting, highlighting,
  or changing an icon, confirm the key's tap and hold behavior in `layout.h`;
  the printable guide can simplify visual weight, but it must not invent a
  second keymap.

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
