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
| 1 | Idea | Investigate hold-tap timing because accidental symbol/number layers occur while typing. | Challenges home-row layer access and hold timing. |
| 2 | Rolled Back | Audit and rebuild combos around the current ChieftainDots Corne layout. | Useful audit, but trial feedback showed the rebuilt inherited placements were not ergonomic enough to keep. |
| 3 | Idea | Design explicit editing/navigation behavior so selecting text does not depend on home-row modifier press order. | Challenges cross-layer modifiers, chord timing, and home-row behavior; avoid solving this by wrapping command layers with `HRM(...)`. |
| 4 | Idea | Investigate why Alt+Enter deletes or copies the last word. | Bug investigation before feature work; may relate to hidden clipboard holds or stale combos. |
| 5 | Idea | Reconsider CAGS/home-row strategy so frequent layers are easier to reach and GUI/Alt/Fn move to conventional positions. | Challenges home-row modifiers and thumb principles. |
| 6 | Idea | Explore a better way to access Function/mouse shortcuts from both hands because Function currently lives on one side. | Challenges one-handed access and layer reachability. |
| 7 | Idea | Bring back Menu and place it where one corner Shift currently is. | Challenges retain-finger-memory and corner modifier placement. |
| 8 | Idea | Add or restore a contextual Menu key because typo fixes are harder without it. | Supports editing workflow and may reuse an underused corner key. |
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
| 27 | Idea | Explore chorded right-hand numbers because the current number access is not working well enough. | Challenges the current Numbers/Nav layer and may support calculator-style entry without extra layer memory. |
| 28 | Idea | Explore chorded navigation for arrows, Ctrl+Shift+arrows, Home, End, Page Up, and Page Down. | Supports editing workflow, but chords must be ergonomic and avoid accidental text selection or deletion. |

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
