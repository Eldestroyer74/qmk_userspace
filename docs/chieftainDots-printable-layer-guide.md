# ChieftainDots Daily Layer Guide

Print this as the desk-side map for the current Corne firmware.

Legend: `---` intentionally blank, `trans` falls through, `DEL` means the
Backspace key sends Delete.

## Daily Anchors

```text
Typing

Tap A / S / D / F       = letters
Hold A                 = Symbols
Hold S or L            = Numbers
Hold D / K             = Ctrl
Hold F / J             = Shift
Hold either lower corner = Spanish
```

```text
S or L family: numbers and movement

Hold S or L            = Numbers
Hold S/L + left GUI    = Navigation arrows
Hold S/L + left Alt    = Extremes: Home / Page / End
Hold S/L + left Space  = Snap: Windows + arrows
```

```text
A or ; family: symbols and tools

Hold A or ;            = Symbols
Hold A/; + left GUI    = Media
Hold A/; + left Alt    = Text snippets
Hold A/; + left Space  = Function / System
```

## Base

```text
TAB      Q      W      E      R      T          Y      U      I      O      P      BSPC
CAPS     A      S      D      F      G          H      J      K      L      ;      '
ES       Z      X      C      V      B          N      M      ,      .      /      ES
                         GUI    ALT    SPACE       ENTER  RALT   MENU
```

Specials:

- `Tab`: tap Tab, hold Escape, double tap Alt+F4.
- `/`: tap `/`, hold `\`, double tap `|`.
- either lower corner: hold Spanish.
- right outer thumb: Menu / context menu.

## Colemak

```text
TAB      Q      W      F      P      G          J      L      U      Y      '      BSPC
CAPS     A      R      S      T      D          H      N      E      I      O      ;
ES       Z      X      C      V      B          K      M      ,      .      /      ES
                         trans  trans  trans       trans  trans  trans
```

Colemak uses the same Spanish lower corners and `/` tap dance as Base.

## Spanish

Access: hold either lower corner.

```text
---      ¡      ---    É      ---    ---        ---    Ú      Í      Ó      ---    ---
---      Á      ---    ---    ---    ---        ---    ---    ---    ---    Ü      ---
trans    ---    ---    ---    ---    ---        Ñ      ---    ---    ---    ¿      trans
                         ---    ---    ---         ---    ---    ---
```

Use Shift first for capitals:

- hold `F` or `J` for Shift, then hold Spanish, then press the vowel.
- tested uppercase accented vowels: `Á Í É Ó Ú`.

## Numbers

Access: hold `S` or `L`.

```text
`        1      2      3      4      5          6      7      8      9      0      -
---      ---    ---    ---    SYM    ---        ---    4      5      6      =      ---
---      ---    ---    ---    ---    ---        ---    1      2      3      /      ---
                         NAV    EXT    SNP         trans  trans  trans
```

Number-pad tap dances:

- right-hand `2`: tap `2`, hold `,`, double tap `<`.
- right-hand `3`: tap `3`, hold `.`, double tap `>`.
- hold the `F` position while in Numbers to reach shifted-number Symbols.

## Symbols

Access: hold `A` or `;`. Also available from Numbers by holding `F`.

```text
~        !      @      #      $      %          ^      &      *      (      )      _
---      ---    ---    ---    ---    ---        ---    $      %      ^      +      ---
---      ---    ---    ---    ---    ---        ---    !      @      #      ---    ---
                         MED    TEXT   SYS         trans  trans  trans
```

Bracket tap dances:

- `(` key: tap `(`, hold `[`, double tap `{`.
- `)` key: tap `)`, hold `]`, double tap `}`.
- `?` is still Shift + `/`, not a Symbols-layer key.

## Movement Layers

Navigation: hold `S` or `L`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    UP     ---    ---    DEL
---      ---    ---    CTRL   SHIFT  ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ---    ---         ---    ---    ---
```

Extremes: hold `S` or `L`, then hold left Alt.

```text
---      ---    ---    ---    ---    ---        ---    ---    PG UP  ---    ---    DEL
---      ---    ---    CTRL   SHIFT  ---        ---    HOME   PG DN  END    ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    trans  ---         ---    ---    ---
```

Snap: hold `S` or `L`, then hold left Space.

```text
---      ---    ---    ---    ---    ---        ---    ---    G-UP   ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    G-LEFT G-DOWN G-RIGHT ---   ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    ---    trans       ---    ---    ---
```

`G` means GUI / Windows.

## A-Family Tools

Media: hold `A` or `;`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        PLAY   ---    VOL+   ---    ---    DEL
---      ---    ---    ---    ---    ---        MUTE   PREV   VOL-   NEXT   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ---    ---         ---    ---    ---
```

Text snippets: hold `A` or `;`, then hold left Alt.

```text
---      ---    ---    ---    ---    ---        HOME   ---    PHONE  ---    ---    DEL
---      ---    ---    ---    ---    ---        WORK   MEET   EMAIL  NAME   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    trans  ---         ---    ---    ---
```

Function / System: hold `A` or `;`, then hold left Space.

```text
---      ---    ---    ---    ---    ---        F12    F7     F8     F9     ---    DEL
---      ---    ---    ---    ---    ---        F11    F4     F5     F6     ---    COLE
---      ---    ---    ---    ---    ---        F10    F1     F2     F3     ---    ---
                         ---    ---    trans       ---    ---    ---
```

Press `COLE` to toggle Colemak.

## Tap / Hold / Double Tap

```text
Physical key       Tap       Hold       Double tap
--------------------------------------------------
Tab                Tab       Esc        Alt+F4
/                  /         \          |
Symbols (          (         [          {
Symbols )          )         ]          }
Numbers 2          2         ,          <
Numbers 3          3         .          >
```

## Current Timing Trial

- `F` and `J` are home-row Shift keys: tap for `f`/`j`, hold for Shift.
- Current trial: `F`/`J` do not become Shift immediately just because another
  key is pressed.
- Watch normal typing for accidental capitals such as `O` when intending `fo`.
- Also check intentional Shift use such as `F + /` for `?` and `F + I` for
  capital `I`.

## RGB Meanings

```text
Caps Lock          red        #FF0000
Colemak            purple     #3C0073
Numbers            blue       #0A195F
Symbols            amber      #693C00
Navigation         cyan       #00FFFF
Extremes           yellow     #FFFF00
Snap               white      #FFFFFF
Media              dark teal  #052323
Text snippets      dark pink  #73142D
Function/System    green      #4B7A16
Delete cue         red        Backspace key only on command layers with DEL
```

Base and Colemak idle should stay visually quiet. Holding an anchor shows the
active layer and thumb options. Once a thumb chord is pressed, unused thumb
hints turn off. Plain Ctrl/Shift/Alt/GUI do not have RGB feedback.

The leftmost LED columns on the left half may not light on this physical board,
so do not rely on those LEDs for chord discovery.
