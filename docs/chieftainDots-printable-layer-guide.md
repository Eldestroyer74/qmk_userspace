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
Hold Base Space / Enter = Shift
Hold either lower corner = Spanish
Right thumbs on command layers = Enter/Shift / RAlt / Menu
```

```text
S or L family: numbers and movement

Hold S or L            = Numbers
Hold S/L + left GUI    = Navigation: arrows / extremes / snap
Hold S/L + left Alt    = MS Styles: Alt+Shift+arrows
```

```text
A or ; family: symbols and tools

Hold A or ;            = Symbols
Hold A/; + left GUI    = Media
Hold A/; + left Alt    = Function / System
Double tap-hold mnemonic = Text snippets
```

## Base

```text
TAB      Q      W      E      R      T          Y      U      I      O      P      BSPC
CAPS     A      S      D      F      G          H      J      K      L      ;      '
ES       Z      X      C      V      B          N      M      ,      .      /      ES
                         GUI    ALT    SPC/SFT     ENT/SFT RALT  MENU
```

Specials:

- `Tab`: tap Tab, hold Escape, double tap Alt+F4.
- `/`: tap `/`, hold `\`, double tap `|`.
- either lower corner: hold Spanish.
- Base Space and Enter: tap normally, hold for Shift.
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
                         ---    ---    ---         trans  trans  trans
```

Use Shift first for capitals:

- Spanish angle quotes are separate keys: `Spanish + Caps = «`,
  `Spanish + ' = »`.
- hold `F` or `J` for Shift, then hold Spanish, then press the vowel.
- opening Spanish punctuation starts a capitalized word in the current auto-caps
  trial: `¿hola` -> `¿Hola`, `¡hola` -> `¡Hola`.
- accented first letters should also capitalize: `¡ánimo` -> `¡Ánimo`.
- tested uppercase accented vowels: `Á Í É Ó Ú`.

## Numbers

Access: hold `S` or `L`.

```text
`        1      2      3      4      5          6      7      8      9      0      -
---      ---    ---    ---    SYM    ---        ---    4      5      6      =      ---
---      ---    ---    ---    ---    ---        ---    1      2      3      /      ---
                         NAV    MS     SPC/SYM     trans  trans  trans
```

Number-pad tap dances:

- right-hand `2`: tap `2`, hold `,`, double tap `<`.
- right-hand `3`: tap `3`, hold `.`, double tap `>`.
- hold left Space while in Numbers to reach Symbols; the `F` position also
  reaches Symbols as a legacy trial path.

## Symbols

Access: hold `A` or `;`. Also available from Numbers by holding left Space.

```text
~        !      @      #      $      %          ^      &      *      (      )      _
---      ---    ---    ---    ---    ---        ---    $      %      ^      +      ---
---      ---    ---    ---    ---    ---        ---    !      @      #      ---    ---
                         MED    SYS    SHIFT       trans  trans  trans
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
                         trans  ---    ---         trans  trans  trans
```

Navigation key behavior:

```text
Key      Tap      Hold      Double-tap-hold
-------------------------------------------
I        Up       PgUp      GUI+Up
J        Left     Home      GUI+Left
K        Down     PgDn      GUI+Down
L        Right    End       GUI+Right
```

MS Styles: hold `S` or `L`, then hold left Alt.

```text
---      ---    ---    ---    ---    ---        ---    ---    AS-UP  ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    AS-LF  AS-DN  AS-RI  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         ---    trans  ---         trans  trans  trans
```

`AS` means Alt+Shift for PowerPoint paragraph/list style movement. `GUI` means
Windows snap from the Navigation double-tap-hold gesture.

## A-Family Tools

Media: hold `A` or `;`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    VOL+   ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    MUTE   VOL-   PLAY   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ---    ---         trans  trans  trans
```

Function / System: hold `A` or `;`, then hold left Alt.

```text
---      ---    ---    ---    ---    ---        F12    F7     F8     F9     ---    DEL
---      ---    ---    ---    ---    ---        F11    F4     F5     F6     ---    COLE
---      ---    ---    ---    ---    ---        F10    F1     F2     F3     ---    ---
                         ---    trans  ---         trans  trans  trans
```

Press `COLE` to toggle Colemak.

Mnemonic snippets on Base/Colemak: double tap the letter, then hold the second
tap.

```text
H = Home
W = Work
P = Phone
M = Meeting
E = Email
N = Name
```

## Tap / Hold / Double-Tap-Hold

```text
Physical key       Tap       Hold       Double-tap-hold
-------------------------------------------------------
Tab                Tab       Esc        Alt+F4
/                  /         \          |
Symbols (          (         [          {
Symbols )          )         ]          }
Numbers 2          2         ,          <
Numbers 3          3         .          >
Navigation I       Up        PgUp       GUI+Up on double-tap-hold
Navigation J       Left      Home       GUI+Left on double-tap-hold
Navigation K       Down      PgDn       GUI+Down on double-tap-hold
Navigation L       Right     End        GUI+Right on double-tap-hold
```

## Current Physical Trial

- `F` and `J` are home-row Shift keys: tap for `f`/`j`, hold for Shift.
- Base Space and Enter are Shift keys on hold. In Numbers, left Space holds
  Symbols while Enter remains Shift.
- Current trial: `F`/`J` do not become Shift immediately just because another
  key is pressed.
- Auto-caps trial: after a word plus `. `, `? `, or `! `, the next letter is
  capitalized.
- Spanish opening punctuation does not need a space: `¿` and `¡` capitalize
  the next letter immediately.
- Watch normal typing for accidental capitals such as `O` when intending `fo`.
- Also check intentional Shift use such as `F + /` for `?` and `F + I` for
  capital `I`.

After flashing, test:

- Space tap = space; Space hold + letter = capital.
- Enter tap = enter; Enter hold + letter = capital.
- Navigation `I/J/K/L` tap = arrows.
- Navigation `I/J/K/L` hold = PgUp/Home/PgDn/End.
- Navigation `I/J/K/L` double-tap-hold = Windows snap.
- Media `I/J/K/L` = Volume Up/Mute/Volume Down/Play.
- `A/; + left Alt` = Function/System.
- Double-tap-hold `H/W/P/M/E/N` snippets fire only when intended.

## RGB Meanings

```text
Caps Lock          red        #FF0000
Colemak            purple     #3C0073
Numbers            blue       #0A195F
Symbols            amber      #693C00
Navigation         cyan       #00FFFF
MS Styles          yellow     #FFFF00
Media              dark teal  #052323
Function/System    green      #4B7A16
Delete cue         red        Backspace key only on command layers with DEL
```

Base and Colemak idle should stay visually quiet. Holding an anchor shows the
active layer and thumb options. Once a thumb chord is pressed, unused thumb
hints turn off. Plain Ctrl/Shift/Alt/GUI do not have RGB feedback.

The leftmost LED columns on the left half may not light on this physical board,
so do not rely on those LEDs for chord discovery.
