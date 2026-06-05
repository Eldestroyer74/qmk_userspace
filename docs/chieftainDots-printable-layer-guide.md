# ChieftainDots Daily Layer Guide

Print this as the desk-side map for the current Corne firmware.

Legend: `---` intentionally blank, `trans` falls through, `DEL` means the
Backspace key sends Delete.

## Daily Anchors

```text
Typing

Tap A / S / D / F       = letters
Hold A / ;             = Function
Hold S / L             = Symbols
Hold D / K             = Numbers
Hold F / J             = Ctrl
Hold Base Space / Enter = Shift
Hold either lower corner = Spanish
Right thumbs on command layers = Enter/Shift / RAlt / Menu
```

```text
Three anchors, one GUI chord each

Hold D/K + left GUI    = Navigation: arrows / selection / extremes
Hold S/L + left GUI    = MS Styles: Alt+Shift+arrows
Hold A/; + left GUI    = Media
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
- either lower corner: hold Spanish, double tap Windows language switch,
  double-tap-hold language selector traversal.
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
«        Á      ---    ---    ---    ---        ---    ---    ---    ---    Ü      »
trans    ---    ---    ---    ---    ---        Ñ      ---    ---    ---    ¿      trans
                         ---    ---    ---         trans  trans  trans
```

Use Shift first for capitals:

- Spanish angle quotes are separate keys: `Spanish + Caps = «`,
  `Spanish + ' = »`.
- hold Space or Enter for Shift, then hold Spanish, then press the vowel.
- opening Spanish punctuation starts a capitalized word in the current auto-caps
  trial: `¿hola` -> `¿Hola`, `¡hola` -> `¡Hola`.
- accented first letters should also capitalize: `¡ánimo` -> `¡Ánimo`.
- tested uppercase accented vowels: `Á Í É Ó Ú`.

Language switch:

- double tap either Spanish key to send `Win+Space` once.
- double tap and hold either Spanish key to hold the Windows selector open.
- while held, press `K` or Space to move through languages.
- release Spanish to choose the highlighted language.

## Numbers

Access: hold `D` or `K`.

```text
`        1      2      3      4      5          6      7      8      9      0      -
---      ---    ---    ---    ---    ---        ---    4      5      6      =      ---
---      ---    ---    ---    ---    ---        ---    1      2      3      /      ---
                         NAV    ALT    SPC/SYM     ENT/SYM RALT  MENU
```

Number-pad tap dances:

- right-hand `2`: tap `2`, hold `,`, double tap `<`.
- right-hand `3`: tap `3`, hold `.`, double tap `>`.
- hold either Space or Enter while in Numbers to reach Symbols.

## Symbols

Access: hold `S` or `L`. Also available from Numbers by holding either Space or Enter.

```text
~        !      @      #      $      %          ^      &      *      (      )      _
---      ---    ---    ---    ---    ---        ---    $      %      ^      +      ---
---      ---    ---    ---    ---    ---        ---    !      @      #      /      ---
                         MS     ALT    SHIFT       ENTER  RALT  MENU
```

Bracket tap dances:

- `(` key: tap `(`, hold `[`, double tap `{`.
- `)` key: tap `)`, hold `]`, double tap `}`.
- `/` key: tap `/`, hold `\`, double tap `|`.
- `?` is still Shift + `/`, not a Symbols-layer key.

## Movement Layers

Navigation: hold `D` or `K`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    UP     ---    ---    DEL
---      ---    ---    ---    CTRL   ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ALT    SHIFT       ENTER  RALT  MENU
```

Navigation key behavior:

```text
Key      Tap      Hold      Double tap        Double-tap-hold
-------------------------------------------------------------
I        Up       PgUp      Shift+Up          Shift+PgUp
J        Left     Home      Ctrl+Shift+Left   Shift+Home
K        Down     PgDn      Shift+Down        Shift+PgDn
L        Right    End       Ctrl+Shift+Right  Shift+End
```

MS Styles: hold `S` or `L`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    AS-UP  ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    AS-LF  AS-DN  AS-RI  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ALT    SHIFT       ENTER  RALT  MENU
```

`AS` means Alt+Shift for PowerPoint paragraph/list style movement.

Snap: double tap and hold any exposed GUI key. While held, GUI stays down and
`I/J/K/L` send plain arrows for repeated Windows snap traversal.

```text
---      ---    ---    ---    ---    ---        ---    ---    UP     ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         GUI    ---    trans       ENTER  RALT  MENU
```

## Tools

Media: hold `A` or `;`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    VOL+   ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    MUTE   VOL-   PLAY   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ALT    SHIFT       ENTER  RALT  MENU
```

Function / System: hold `A` or `;`.

```text
TAB      F1     F2     F3     F4     F5         F6     F7     F8     F9     F10    DEL
---      ---    ---    ---    ---    ---        ---    F4     F5     F6     F11    COLE
---      ---    ---    ---    ---    ---        ---    F1     F2     F3     F12    ---
                         MED    ALT    SHIFT       ENTER  RALT  MENU
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
G = Personal email
N = Name
```

## Tap / Hold / Double Tap

```text
Physical key       Tap       Hold       Double tap          Double-tap-hold
-------------------------------------------------------------------------
Tab                Tab       Esc        Alt+F4              Alt+F4
/                  /         \          |                   |
Symbols (          (         [          {                   {
Symbols )          )         ]          }                   }
Numbers 2          2         ,          <                   <
Numbers 3          3         .          >                   >
Numbers /          /         \          |                   |
Symbols /          /         \          |                   |
Navigation I       Up        PgUp       Shift+Up            Shift+PgUp
Navigation J       Left      Home       Ctrl+Shift+Left     Shift+Home
Navigation K       Down      PgDn       Shift+Down          Shift+PgDn
Navigation L       Right     End        Ctrl+Shift+Right    Shift+End
GUI                GUI       GUI        GUI                 Snap layer
Spanish key        ---       Spanish    Win+Space           Language selector
```

## Current Physical Trial

- `F` and `J` are Ctrl on hold, not Shift.
- Base Space and Enter are Shift keys on hold.
- In Numbers, both Space and Enter hold Symbols.
- In Symbols and Function, Space and Enter hold Shift.
- Auto-caps trial: after a word plus `. `, `? `, or `! `, the next letter is
  capitalized.
- Spanish opening punctuation does not need a space: `¿` and `¡` capitalize
  the next letter immediately.
- Watch normal typing for accidental thumb Shift, especially on Space.
- Also check intentional Shift use such as Space/Enter + `/` for `?`.

After flashing, test:

- Space tap = space; Space hold + letter = capital.
- Enter tap = enter; Enter hold + letter = capital.
- Navigation `I/J/K/L` tap = arrows.
- Navigation `I/J/K/L` hold = PgUp/Home/PgDn/End.
- Navigation `I/J/K/L` double tap = selection movement.
- Navigation `I/J/K/L` double-tap-hold = extreme selection.
- Any exposed GUI double-tap-hold + `I/J/K/L` = Windows snap traversal.
- Spanish double tap = Windows language switch.
- Spanish double-tap-hold + `K` or Space = traverse the language selector.
- Accidental Alt+Shift and Alt+Space should not switch language or open the
  host window menu.
- Media `I/J/K/L` = Volume Up/Mute/Volume Down/Play.
- `D/K + left GUI` = Navigation.
- `S/L + left GUI` = MS Styles.
- `A/; + left GUI` = Media.
- Double-tap-hold `H/W/P/M/E/G/N` snippets fire only when intended.

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
