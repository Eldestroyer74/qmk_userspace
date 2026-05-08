# ChieftainDots Roadmap

This roadmap is a backlog, not an approval list. Items start as ideas and must
be discussed, researched, and approved before implementation.

## Lifecycle

- Idea: captured but not prioritized.
- Discussed: intent and ergonomics are understood.
- Researched: QMK compatibility and size risk are understood.
- Approved: ready to implement as the next single feature.
- Implemented: code exists locally.
- Compiled: firmware builds successfully.
- Flashed: firmware has been put on the keyboard.
- Kept: real use confirms the feature should stay.
- Rolled Back: the feature was removed after compile, flash, size, or usability
  concerns.

## Backlog

| Priority | Status | Feature | Principle Challenge |
| --- | --- | --- | --- |
| 1 | Compiled | Investigate hold-tap timing because accidental symbol/number layers occur while typing. First experiment: enable QMK Flow Tap at `FLOW_TAP_TERM 150` to reduce accidental S/L number-layer activation during normal typing flow. | Challenges home-row layer access and hold timing; Flow Tap has a real firmware-size cost. |
| 2 | Rolled Back | Audit and rebuild combos around the current ChieftainDots Corne layout. | Useful audit, but trial feedback showed the rebuilt inherited placements were not ergonomic enough to keep. |
| 3 | Idea | Design explicit editing/navigation behavior so selecting text does not depend on home-row modifier press order. | Challenges cross-layer modifiers, chord timing, and home-row behavior; avoid solving this by wrapping command layers with `HRM(...)`. |
| 4 | Idea | Investigate why Alt+Enter deletes or copies the last word. | Bug investigation before feature work; may relate to hidden clipboard holds or stale combos. |
| 5 | Idea | Reconsider CAGS/home-row strategy so frequent layers are easier to reach and GUI/Alt/Fn move to conventional positions. | Challenges home-row modifiers and thumb principles. |
| 6 | Idea | Explore a better way to access Function/mouse shortcuts from both hands because Function currently lives on one side. | Challenges one-handed access and layer reachability. |
| 7 | Idea | Bring back Menu and place it where one corner Shift currently is. | Challenges retain-finger-memory and corner modifier placement. |
| 8 | Compiled | Add or restore a contextual Menu key because typo fixes are harder without it. Requirement: replace the old right-thumb Function access key with the QMK context-menu/Application key, because Function keys are now accessible through the S/L numbered-command layer. | Supports editing workflow and reuses a thumb key whose old layer-access purpose is fading. |
| 9 | Idea | Reconsider the bottom-left and bottom-right Shift keys because they may be underused. | Challenges retain-finger-memory and corner modifier placement. |
| 10 | Idea | Review whether left-hand number entry is worth the new memory burden. | Challenges calculator-style number placement and memorability. |
| 11 | Idea | Add `/` to the calculator/number layer to avoid changing layers during calculator use. | Supports one layer, one concept. |
| 12 | Idea | Add full stop `.` to the left-hand side of the number layer. | Supports calculator completeness and memorability. |
| 13 | Idea | Add or restore modifiers on non-base layers. | Challenges the layer inheritance/modifier rule. |
| 14 | Idea | Investigate rollover behavior with symbols. | Challenges symbol-layer access and hold-tap timing. |
| 15 | Idea | Make non-base layers use Escape where Base uses Tab. | Supports predictable non-base escape behavior. |
| 16 | Idea | Add a combo for toggling Caps Lock. | Supports caps behavior but may interact with combos and typing flow. |
| 17 | Idea | Run a firmware size audit without behavior loss; compile before and after, keep current behavior, and record bytes saved. | Supports future feature capacity without weakening current ChieftainDots behavior. |
| 18 | Idea | Add macros for Spanish characters. | Needs a decision about text macros, layers, or combos. |
| 19 | Idea | Add DaVinci Resolve shortcuts. | May require a dedicated app/workflow layer. |
| 20 | Idea | Add a shortcut for "SUB for book a meeting". | Needs macro architecture review. |
| 21 | Idea | Add a shortcut for "sub password". | Needs privacy, naming, and macro-storage review. |
| 22 | Flashed | Make RGB indicator-first by disabling decorative default animation and using RGB only for layer states, modifiers, persistent toggles, or warnings. | Challenges visual identity in favor of lower distraction and clearer state feedback. |
| 23 | Idea | Design ergonomic text snippet shortcuts for HubSpot meeting link and email address. | Supports useful text shortcuts, but placement must not pull fingers awkwardly off home position; private or secret strings need separate security review before inclusion. |
| 24 | Idea | Refine Caps Lock RGB so red indicates Caps cleanly without relying on the rough whole-keyboard fallback. | Supports RGB-as-indicator, but requires careful Corne split LED mapping research before implementation. |
| 25 | Idea | Review OLED screen layer indicators to confirm they report the intended active ChieftainDots layer state. | Supports state visibility; OLED should report keyboard state rather than owning key behavior. |
| 26 | Idea | Design one coherent Tab/Escape/Close key behavior: tap for Tab, hold for Escape, double tap for Close. | Challenges the current mixed treatment of the same key across layers; supports one physical key having one memorable concept. |
| 27 | Kept | Explore two anchor families: `S` for Numbers/Editing and `A` for Symbols/Tools, with left thumb refinements. | S/L-held numbered commands tested successfully and kept as a checkpoint. Still incomplete without navigation, selection, and extremes. |
| 28 | Idea | Define spatial consistency rules across command layers. | Supports learnability by keeping plus/minus aligned with scroll up/down, Enter/confirm aligned with primary action, and directional actions aligned across navigation sub-layers. |
| 29 | Idea | Make the Backspace position become Delete on non-base command layers. | Supports layer-aware editing: Base corrects typing with Backspace, command layers edit forward with Delete. |
| 30 | Kept | Make the Base Tab key tap for Tab, hold for Escape, and double tap for Alt+F4 close. | Tested successfully and kept as a checkpoint; supports one physical key having one memorable command family. |
| 31 | Idea | Move or keep the Colemak toggle on the future Media/System layer. | Supports persistent-mode controls living with other system/media commands instead of mixed into the numbered-command layer. |
| 32 | Idea | Research each ChieftainDots feature against QMK native support, current QMK docs, community practice, and implementation best practices before building or revising it. | Supports maintainability and avoids custom code when QMK already has a tested feature or common pattern. |
| 33 | Kept | Add navigation, word selection, and extremes refinements from the S/L numbered-command layer using left thumb holds and the `I/J/K/L` directional shape. | Tested successfully and kept; challenges home-row purity in favor of spatial consistency across arrows, selection, and extremes. |
| 34 | Compiled | Add A-family Media layer: hold `A` plus left GUI for media controls. Updated candidate layout: `Y = Play/Pause`, `H = Mute`, `I = Vol+`, `K = Vol-`, `J = Previous`, `L = Next`, Backspace position = Delete, right Shift unused. | Supports A-family tools and reuses spatial direction grammar plus the Y/H positive/suppressing operator pair. |
| 35 | Kept | Add A-family Mouse layer: hold `A` plus left Alt for mouse controls. Requirement: `Y = Wheel Up` and `H = Wheel Down` to match the numbered-command layer where `Y = +` and `H = -`; mouse movement should use `I/J/K/L`; right thumb Enter/Menu positions become mouse buttons 1 and 2. | Supports spatial consistency between calculator operators, scroll direction, pointer movement, and primary/secondary actions. |
| 36 | Compiled | Add A-family Apps/System layer: hold `A` plus left Space, then press any right home-row key to toggle Colemak; Backspace position remains Delete. | Supports keeping persistent keyboard-mode toggles deliberate without duplicating clean Windows shortcuts or filling a sparse layer with unrelated app launches. |
| 37 | Idea | Restore or redesign keyboard-only window snapping because `S/L + left GUI` now enters Navigation, which prevents using GUI+Left/Right to move windows left or right. | Challenges the thumb-refinement model because GUI is both an OS modifier and a layer selector; must preserve common OS window-management shortcuts. |
| 38 | Discussed | Add Mute to the Media layer at `H`, paired vertically under `Y = Play/Pause`; right Shift remains unused on this layer. | Supports A-family media completeness and the positive/suppressing Y/H operator grammar. |
| 39 | Idea | Explore Base-layer launchers now that the corner Shift keys are not being used. Current requirement candidate: move Colemak toggle to bottom-left Base corner, Calculator launch to bottom-right Base corner, and Media Player launch somewhere on Base rather than on the sparse System layer. Keep `KC_CALC` and `KC_MSEL` as candidate keycodes. | Challenges the old Shift-corner finger-memory principle, but supports useful launch commands without filling the sparse System layer. |
| 40 | Implemented | Remove the legacy mixed Function layer from the active Corne build recipe and retire its OLED/RGB layer indicators. | Supports one-layer-one-concept by replacing a mixed legacy layer with explicit Numbers/Commands, Media, Mouse, Apps/System, and Base Menu behavior. |

## Trial Log

| Date | Feature | Result | Notes |
| --- | --- | --- | --- |
| 2026-05-07 | Apply home-row layer/mod wrapper to Numbers, Symbols, and Function layers in `keymaps/corne.json`. | Rolled Back. | Compiled with no size change, but real use showed Symbols and Numbers became confusing because command-layer keys were transformed into nested layer-tap/mod-tap behaviors. Filterpaper's current Corne example also wraps only typing layers (`BASE` and `COLE`) and keeps `SYMB`, `NUMB`, and `FUNC` plain. Future editing/navigation work should use explicit commands, explicit modifiers, or a dedicated editing layer instead. |
| 2026-05-07 | Make RGB indicator-first by default. | Flashed and tested. | Removed decorative Candy default/Colemak animations, added RGB Indicator V1 color vocabulary, kept RGB indicator hooks, and changed size from 28212/28672 to 27432/28672 bytes. Typing works and Caps Lock visibly turns RGB red, but Caps currently uses a rough whole-keyboard red fallback. |
| 2026-05-07 | Investigate Caps Lock RGB mapping. | Follow-up needed. | QMK's `CAP_FLAG` selection did not visibly light Caps on this board. A temporary last-key diagnostic showed confusing split behavior: left-side inner columns `R/T/F/G/V/B` and right-hand letter keys were useful observations, but custom diagnostic state is not synced across split halves. Future work should compare `keyboard.json` LED indexes, split handedness, and physical LED wiring before changing behavior. |
| 2026-05-07 | Audit current combo reachability. | Follow-up needed. | Active combos appear partly inherited from Filterpaper. `tog_fnc` and `tog_num` likely reference old thumb keys no longer present in the current Corne layout. Combos involving `F`, `J`, `K`, and `;` may not fire because the current `HRM(...)` wrapper changes those exact keycodes into mod-tap or layer-tap keycodes. `I+O` is likely reachable, and `M+,` may be reachable. Future combo work should verify generated keycodes before implementation. |
| 2026-05-07 | Rebuild combos around current Corne layout. | Compiled. | Updated active combos to use current exact keycodes: `HM_F+KC_G`, `KC_M+KC_COMM`, `HM_SCLN+KC_SLSH`, and existing HRM key aliases for right-hand navigation. Disabled stale `tog_fnc` and `tog_num` combos because they referenced old Filterpaper thumb keys. Firmware changed from 27456/28672 to 27430/28672 bytes. Needs flash and physical combo trial before marking Kept. |
| 2026-05-07 | Capture OLED layer indicator review. | Backlog added. | The keyboard has screens that appear to show layer state, but their behavior has not been checked against the current ChieftainDots layer model. Future review should inspect OLED rendering code, split state sync, and how Base/Colemak/Numbers/Symbols/Function are displayed. |
| 2026-05-08 | Disable combos after ergonomic trial feedback. | Compiled. | User feedback: HubSpot combo required leaving a home-row index position, Ctrl+Shift+Arrow combos overlap with an existing disliked navigation idea, and volume combos required awkward vertical finger postures. Combo feature is disabled while text snippets, numbers, navigation, and system/media chords are redesigned from ergonomics first. Firmware size improved to 24874/28672 bytes, with 3798 bytes free. |
| 2026-05-08 | Explore two anchor families. | Design note added. | Candidate model: `S` family covers Numbers, Arrows, Selection, and Extremes using left thumb refinements. `A` family covers Symbols, Function, Mouse, and Media/Volume using left thumb refinements. This has not been implemented. Prefer ordinary QMK layer stacking over combos or custom state machines unless plain layers prove inadequate. |
| 2026-05-08 | Implement S-held right-hand Numbers first slice. | Compiled. | `_NUMB` now blanks left-hand non-thumb keys and maps the right hand to `+ 7 8 9 * / - 4 5 6 / . / ( 1 2 3 0 )`. Existing `S` layer hold remains the access method; left thumbs and `L` hold are unchanged for this trial. Compile succeeded at 24874/28672 bytes, with 3798 bytes free. Needs flash and physical trial before marking Kept. |
| 2026-05-08 | Capture S-held numbered-command refinement. | Design note added. | Candidate next slice: put Function keys on the left side of the S-held layer using old number memory: `F12 F7 F8 F9`, `F11 F4 F5 F6`, `F10 F1 F2 F3`; keep right-hand numbers/operators; blank Tab/Caps/Shift positions; make the Backspace position Delete on this non-base layer. |
| 2026-05-08 | Implement S/L-held numbered commands and Tab/Escape/Close. | Kept. | `_NUMB` now uses `F12 F7 F8 F9`, `F11 F4 F5 F6`, `F10 F1 F2 F3` on the left and keeps right-hand numbers/operators with Delete on the Backspace position. Base Tab is now a tap dance: tap Tab, hold Escape, double tap Alt+F4. Compile succeeded at 25756/28672 bytes, with 2916 bytes free; physical testing confirmed functions and Tab behavior work. This is a working checkpoint, but the keyboard remains less useful than intended until navigation, selection, and extremes are added. |
| 2026-05-08 | Capture right-hand directional shape. | Design note added. | Directional command layers should use `I/J/K/L` as Up/Left/Down/Right. Selection and extremes should reuse the same physical shape. This deliberately prioritizes spatial consistency over strict home-row-only command placement. |
| 2026-05-08 | Implement navigation, selection, and extremes refinements. | Kept. | Added `_NAV`, `_SELE`, and `_EXTR` as plain QMK layers above `_NUMB`. Hold `S` or `L`, then use left GUI for arrows, left Alt for Ctrl+Shift selection, and left Space for Home/Page/End extremes. All three use the `I/J/K/L` directional shape. Navigation and extremes keep `D = Ctrl`, `F = Shift`, and Backspace position as Delete. Physical testing confirmed navigation, selection, extremes, Ctrl, and Shift behavior work. |
| 2026-05-08 | Capture A-family media layout. | Design note added. | Agreed `A + left GUI` media layout: `I = Volume Up`, `K = Volume Down`, `J = Previous`, `L = Next`, `, = Play/Pause`, with Backspace position as Delete. Symbols remain unchanged for now. |
| 2026-05-08 | Capture mouse wheel/operator consistency. | Design note added. | Mouse wheel placement should mirror calculator plus/minus: `Y = Wheel Up` corresponds to `Y = +`, and `H = Wheel Down` corresponds to `H = -`. Multiplication/division remain a vertical pair with `P = *` above `; = /`. |
| 2026-05-08 | Implement A-family media, mouse, apps/system, and base Menu. | Compiled. | Base old right-thumb Function position now sends `KC_APP`. Holding `A` plus left GUI enters Media, holding `A` plus left Alt enters Mouse, and holding `A` plus left Space enters Apps/System with Colemak toggle. Compile succeeded at 26546/28672 bytes, with 2126 bytes free. Needs flash and physical trial before marking Kept. |
| 2026-05-08 | Capture navigation/window-snapping conflict. | Follow-up needed. | User reported that keyboard-only window snapping no longer works because the left GUI thumb is now used to access arrows from the S/L family. Future navigation work must decide whether to add explicit GUI+Left/Right commands, move Navigation off GUI, or provide another clean OS-window-management route. |
| 2026-05-08 | Trial A-family Mouse layer. | Kept. | User reported that the mouse layer works as expected. Keep the implemented `A + left Alt` mouse layout while continuing to trial the rest of the A-family. |
| 2026-05-08 | Enable Flow Tap for hold-tap typing accidents. | Compiled. | Added `FLOW_TAP_TERM 150` as the first timing experiment for accidental `S`/`L` number-layer activation that produced characters such as `8` or `(` during normal typing. Compile succeeded, but firmware size rose to 27292/28672 bytes, with 1380 bytes free. Needs flash and typing trial before marking Kept. |
| 2026-05-08 | Capture Media/System candidates. | Design note added. | Add Mute near the volume controls, and consider Apps/System candidates such as Calculator launch. These are candidates only; placement and QMK/native support still need discussion and research. |
| 2026-05-08 | Refine Media Play/Mute placement. | Design note added. | Agreed Media should treat `Y = Play/Pause` like the positive/additive action and `H = Mute` like the subtractive/suppressing action. The previous `, = Play/Pause` placement should move when the next media implementation happens. |
| 2026-05-08 | Move Colemak toggle to right home row on System. | Compiled. | System is now intentionally sparse: hold `A` plus left Space, then press any right home-row key to toggle Colemak. Calculator and Media Player launchers stay in the backlog for a separate non-layer launcher design. Compile succeeded at 27292/28672 bytes, with 1380 bytes free. Needs flash and trial before marking Kept. |
| 2026-05-08 | Capture Base corner launcher requirement. | Requirement added. | User noted the left and right Shift positions are no longer used. Candidate requirement only: place Colemak toggle on the Base bottom-left corner, Calculator on the Base bottom-right corner, and Media Player on the Base layer rather than System. No code change yet. |
| 2026-05-08 | Remove legacy Function layer from active Corne recipe. | Implemented. | Removed `_FUNC` from `keymaps/corne.json`, renumbered active layer constants, and removed the stale Function color/OLED cases. The old `_FUNC` macro body remains in `layout.h` as unreachable inherited reference text until a separate mechanical deletion pass. Needs compile before marking Compiled. |

## Feature Template

Use this shape when promoting an item from Idea:

```text
Feature:
Status:
Intent:
Principle supported or challenged:
Architecture impact:
QMK compatibility notes:
Firmware size risk:
Acceptance criteria:
Rollback plan:
```
