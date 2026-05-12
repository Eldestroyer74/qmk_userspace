# ChieftainDots Printable Layer Guide

Print this as a desk-side reminder for the current Corne firmware.

Legend:

- `---` means the key is intentionally blank on that layer.
- `trans` means the key falls through to the active typing layer.
- `DEL` means the physical Backspace position sends Delete on that layer.
- Tap/hold/double-tap keys are listed after the diagrams.

## Layer Access

```text
Typing layers

Base:
  Hold A or ;  = Symbols
  Hold S or L  = Numbers / Commands
  Hold D/K     = Ctrl
  Hold F/J     = Shift

Colemak:
  Hold A or O  = Symbols
  Hold R or I  = Numbers / Commands
  Hold S/E     = Ctrl
  Hold T/N     = Shift
```

```text
Command refinements

Hold S or L, then:
  Left GUI thumb    = Navigation / arrows
  Left Alt thumb    = Selection
  Left Space thumb  = Extremes: Home / Page Up / Page Down / End

Hold A or ;, then:
  Left GUI thumb    = Media
  Left Alt thumb    = Mouse
  Left Space thumb  = Function / System
```

## Base

```text
TAB      Q      W      E      R      T          Y      U      I      O      P      BSPC
CAPS     A      S      D      F      G          H      J      K      L      ;      '
MEDIA    Z      X      C      V      B          N      M      ,      .      /      CALC
                         GUI    ALT    SPACE       ENTER  RALT   MENU
```

Base special keys:

- `Tab`: tap Tab, hold Escape, double tap Alt+F4.
- `'`: tap `'`, hold `` ` ``, double tap `~`.
- `/`: tap `/`, hold `\`, double tap `|`.
- Lower-left corner launches Media Player.
- Lower-right corner launches Calculator.
- Right thumb outer key is Menu / context menu.

## Colemak

```text
TAB      Q      W      F      P      G          J      L      U      Y      '      BSPC
CAPS     A      R      S      T      D          H      N      E      I      O      ;
MEDIA    Z      X      C      V      B          K      M      ,      .      /      CALC
                         trans  trans  trans       trans  trans  trans
```

Colemak special keys:

- `'`: tap `'`, hold `` ` ``, double tap `~`.
- `/`: tap `/`, hold `\`, double tap `|`.
- Lower-left corner launches Media Player.
- Lower-right corner launches Calculator.

## Numbers / Commands

Access: hold `S` or `L`.

```text
---      ---    ---    ---    ---    ---        +      7      8      9      *      DEL
---      ---    ---    ---    ---    ---        -      4      5      6      /      .
---      ---    ---    ---    ---    ---        (      1      2      3      0      )
                         NAV    SEL    EXT         trans  trans  trans
```

Useful combinations from Numbers / Commands:

- Hold `S` or `L` + left GUI thumb = Navigation.
- Hold `S` or `L` + left Alt thumb = Selection.
- Hold `S` or `L` + left Space thumb = Extremes.

## Symbols

Access: hold `A` or `;` on Base, or hold `A` or `O` on Colemak.

```text
!        @      #      $      %      ^          &      *      (      )      _      DEL
trans    trans  trans  trans  trans  trans      trans  trans  trans  trans  trans  trans
trans    trans  trans  trans  trans  trans      trans  trans  trans  trans  trans  trans
                         MED    MOUSE  SYS         trans  trans  trans
```

Symbols bracket keys:

- `(` key: tap `(`, hold `[`, double tap `{`.
- `)` key: tap `)`, hold `]`, double tap `}`.

Transparent rows mean ordinary Base/Colemak letters still work while Symbols is held.

## Navigation

Access: hold `S` or `L`, then hold left GUI thumb.

```text
---      ---    ---    ---    ---    ---        ---    ---    UP     ---    ---    DEL
---      ---    ---    CTRL   SHIFT  ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ---    ---         ---    ---    ---
```

Use plain `CTRL` and `SHIFT` positions on the left side with arrows when needed.

## Selection

Access: hold `S` or `L`, then hold left Alt thumb.

```text
---      ---    ---    ---    ---    ---        ---    ---    C-S-UP ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    C-S-L  C-S-DN C-S-R  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    trans  ---         ---    ---    ---
```

`C-S` means Ctrl+Shift. This layer is a candidate for future removal if selection is handled by plain Ctrl/Shift with arrows.

## Extremes

Access: hold `S` or `L`, then hold left Space thumb.

```text
---      ---    ---    ---    ---    ---        ---    ---    PG UP  ---    ---    DEL
---      ---    ---    CTRL   SHIFT  ---        ---    HOME   PG DN  END    ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    ---    trans       ---    ---    ---
```

Use plain `CTRL` and `SHIFT` positions on the left side with Home/End/Page keys when needed.

## Media

Access: hold `A` or `;`, then hold left GUI thumb.

```text
---      ---    ---    ---    ---    ---        PLAY   ---    VOL+   ---    ---    DEL
---      ---    ---    ---    ---    ---        MUTE   PREV   VOL-   NEXT   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ---    ---         ---    ---    ---
```

Known trial note: Play/Pause and Mute worked. Previous/Next may depend on the app.

## Mouse

Access: hold `A` or `;`, then hold left Alt thumb.

```text
---      ---    ---    ---    ---    ---        WH UP  ---    MS UP  ---    ---    DEL
---      ---    ---    ---    ---    ---        WH DN  MS L   MS DN  MS R   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    trans  ---         BTN1   ---    BTN2
```

## Function / System

Access: hold `A` or `;`, then hold left Space thumb.

```text
---      ---    ---    ---    ---    ---        F12    F7     F8     F9     ---    DEL
---      ---    ---    ---    ---    ---        F11    F4     F5     F6     ---    COLE
---      ---    ---    ---    ---    ---        F10    F1     F2     F3     ---    ---
                         ---    ---    trans       ---    ---    ---
```

Press quote on this layer to toggle Colemak.

## Tap / Hold / Double Tap Summary

```text
Physical key       Tap       Hold       Double tap
--------------------------------------------------
Tab                Tab       Esc        Alt+F4
'                  '         `          ~
/                  /         \          |
Symbols (          (         [          {
Symbols )          )         ]          }
```

## Current Pending Redesign Ideas

These are documented requirements, not necessarily active firmware:

- Remove the dedicated Selection layer.
- Use plain Ctrl/Shift with Navigation and Extremes for selection.
- Move Extremes to the left Alt thumb slot.
- Use the left Space thumb slot for GUI+Arrows / window manipulation.
- Function keys now live in `A + Space`; verify and mark kept after physical trial.
