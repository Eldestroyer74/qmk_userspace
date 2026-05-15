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
  Left Alt thumb    = Extremes: Home / Page Up / Page Down / End
  Left Space thumb  = Snap: Windows GUI + arrows

Hold A or ;, then:
  Left GUI thumb    = Media
  Left Alt thumb    = Text snippets
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
- `'`: tap `'`.
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

- `'`: tap `'`.
- `/`: tap `/`, hold `\`, double tap `|`.
- Lower-left corner launches Media Player.
- Lower-right corner launches Calculator.

## Numbers / Commands

Access: hold `S` or `L`.

```text
`        1      2      3      4      5          6      7      8      9      0      -
---      ---    ---    ---    SYM    ---        ---    4      5      6      =      ---
---      ---    ---    ---    ---    ---        ---    1      2 , <  3 . >  /      ---
                         NAV    EXT    SNP         trans  trans  trans
```

Useful combinations from Numbers / Commands:

- Hold `S` or `L` + left GUI thumb = Navigation.
- Hold `S` or `L` + left Alt thumb = Extremes.
- Hold `S` or `L` + left Space thumb = Snap.
- Hold `S` or `L` + the `F` position = shifted-number Symbols.
- On the right-hand pad, `2` taps `2`, holds `,`, and double-taps `<`.
- On the right-hand pad, `3` taps `3`, holds `.`, and double-taps `>`.

## Symbols

Access: hold `A` or `;` on Base, or hold `A` or `O` on Colemak.
Also available from Numbers by holding the `F` position while `S` or `L` is
held.

```text
~        !      @      #      $      %          ^      &      *      (      )      _
---      ---    ---    ---    ---    ---        ---    $      %      ^      +      ---
---      ---    ---    ---    ---    ---        ---    !      @      #      ---    ---
                         MED    TEXT   SYS         trans  trans  trans
```

Symbols bracket keys:

- `(` key: tap `(`, hold `[`, double tap `{`.
- `)` key: tap `)`, hold `]`, double tap `}`.

Symbols is currently a trial surface for shifted-number memory. The top row
keeps the normal shifted number row; the right-hand lower rows mirror the
Numbers pad with shifted equivalents. `?` is intentionally absent here because
it is reached from Base/Colemak with Shift plus `/`, not through Symbols.

## Navigation

Access: hold `S` or `L`, then hold left GUI thumb.

```text
---      ---    ---    ---    ---    ---        ---    ---    UP     ---    ---    DEL
---      ---    ---    CTRL   SHIFT  ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ---    ---         ---    ---    ---
```

Use plain `CTRL` and `SHIFT` positions on the left side with arrows when needed.

## Extremes

Access: hold `S` or `L`, then hold left Alt thumb.

```text
---      ---    ---    ---    ---    ---        ---    ---    PG UP  ---    ---    DEL
---      ---    ---    CTRL   SHIFT  ---        ---    HOME   PG DN  END    ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    trans  ---         ---    ---    ---
```

Use plain `CTRL` and `SHIFT` positions on the left side with Home/End/Page keys when needed.

## Snap

Access: hold `S` or `L`, then hold left Space thumb.

```text
---      ---    ---    ---    ---    ---        ---    ---    G-UP   ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    G-LEFT G-DOWN G-RIGHT ---   ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    ---    trans       ---    ---    ---
```

`G` means GUI/Windows. Use this for keyboard-only window snapping.

## Media

Access: hold `A` or `;`, then hold left GUI thumb.

```text
---      ---    ---    ---    ---    ---        PLAY   ---    VOL+   ---    ---    DEL
---      ---    ---    ---    ---    ---        MUTE   PREV   VOL-   NEXT   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ---    ---         ---    ---    ---
```

Known trial note: Play/Pause and Mute worked. Previous/Next may depend on the app.

## Text Snippets

Access: hold `A` or `;`, then hold left Alt thumb.

```text
---      ---    ---    ---    ---    ---        HOME   ---    PHONE  ---    ---    DEL
---      ---    ---    ---    ---    ---        WORK   MEET   EMAIL  NAME   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    trans  ---         ---    ---    ---
```

Text snippets are compiled from a local private header. Do not commit private
address values.

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
/                  /         \          |
Symbols (          (         [          {
Symbols )          )         ]          }
```

## Timing Trial

- `F` and `J` home-row Shift now bypass Flow Tap and become eager holds like the
  modifier keys. Number/Symbol layer keys stay more deliberate. Test `F + /`
  for `?`, `F + I` for capital `I`, ordinary words containing `fi`, and normal
  typing that previously produced accidental numbers.

## RGB Reminders

- Base/Colemak idle should stay visually quiet.
- Caps Lock should show whole-board flat red immediately after startup:
  `#FF0000`.
- Colemak mode should show whole-board dark purple: `#3C0073`.
- Numbers is dark blue: `#0A195F`.
- Symbols is dark amber: `#693C00`.
- Navigation is cyan: `#00FFFF`.
- Extremes is yellow: `#FFFF00`.
- Snap is white: `#FFFFFF`.
- Media is dark teal: `#052323`.
- Text snippets are dark pink: `#73142D`.
- Function/System is fluoro green: `#4B7A16`.
- Holding only `S`/`L` or `A`/`;` shows the active layer plus future thumb
  options. The thumb options use the colors of the layers they would open.
- Once a thumb chord is pressed, unused thumb hints turn off. The active layer,
  anchor key, and selected thumb key remain lit where the board has working LEDs.
- Delete lights red only on thumb-refined command layers where the Backspace key
  has become Delete. Numbers and Symbols do not use the red Delete cue.
- Function/System uses its own layer color, but the `COLE` toggle key should be
  Colemak purple.
- Plain Ctrl/Shift/Alt/GUI no longer have RGB feedback. Global modifier-state
  lighting was removed because it lit opposite-hand modifiers and was confusing.
- The leftmost LED columns on the left half may not light on this physical board.
  Do not rely on those LEDs for chord discovery.

## Current Pending Redesign Ideas

These are documented requirements, not necessarily active firmware:

- Function keys now live in `A + Space`; verify and mark kept after physical trial.
