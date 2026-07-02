# ChieftainDots Daily Layer Guide

Print this as the desk-side map for the four-anchor command layout trial.
This guide is intentionally forward-looking so the whole design can be reviewed
as one coherent keyboard.

Stable trial note: OLED/screens are on. The current safe OLED slice makes
Navigation movement-only, flattens Styles and Media to plain keys, and keeps
Snap in a small Snap-specific path instead of the newer shared directional
tap-dance helper that caused phantom modifier reports in QMK key tester. The
OLED label names semantic states, not raw Ctrl/Alt/GUI.

Legend: `---` intentionally blank, `trans` falls through, `DEL` means the
Backspace key sends Delete.

## Daily Anchors

```text
Typing

Tap A / S / D / F       = letters
Hold A / ;             = Control
Hold S / L             = Symbols
Hold D / K             = Numbers
Hold F / J             = Function
Hold Base Space / Enter = Shift
Hold either lower corner = Spanish
Right thumbs: Numbers uses Enter/Symbols; Symbols and Function use Enter/Shift.
Function left thumb: tap Enter, hold Shift for mouse-driven paste workflows;
RAlt and Menu stay available.
```

```text
Four anchors, one GUI chord each

Hold D/K + left GUI    = Navigation: movement only
Hold S/L + left GUI    = Styles: Alt+Shift+Arrow
Hold A/; + left GUI    = Media, Teams-first
Hold F/J + left GUI    = Snap window placement mode
Double tap-hold mnemonic = Text snippets
```

Snap has no tap-only action. It is entered as a held mode from Function plus
the left GUI thumb. During Snap, firmware sends one-shot `Win+Arrow` taps.
Double tap-hold GUI has no separate action in this build; it behaves like the
ordinary GUI/anchor hold path.

```text
Command gesture rule

Tap             = direct daily action
Tap and hold    = sustained / larger / repeated action
Double tap      = secondary reversible action
Double tap-hold = empty unless a real need is proven
```

Styles and Media are tap-only in this build. Snap is mode-only.

## Chord Layer Hierarchies

Read each map as `I/J/K/L` in the right-hand arrow cluster:

```text
      I
J     K     L
```

Navigation: hold `D/K`, then hold left GUI.

```text
Tap: arrows
      Up
Left  Down  Right

Hold: held arrows
      Up held
Left held  Down held  Right held

Double tap: semantic jumps
      Ctrl+Up
Ctrl+Left  Ctrl+Down  Ctrl+Right

Double tap-hold: movement extremes
      PgUp
Home  PgDn  End
```

Styles: hold `S/L`, then hold left GUI.

```text
Tap: PowerPoint / Word structure
      Alt+Shift+Up
Alt+Shift+Left  Alt+Shift+Down  Alt+Shift+Right
```

Media: hold `A/;` for Control, then hold left GUI.

```text
Plain keys only in the current RAM experiment:
      Volume Up
Teams mic mute  Volume Down  Play/Pause
```

Snap window placement: hold `F/J` for Function, then hold left GUI and press
`I/J/K/L`.

```text
One-shot shortcuts
      Win+Up
Win+Left  Win+Down  Win+Right
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
- `Caps`: tap Caps Lock, double tap Long Caps.
- `/`: tap `/`, hold `\`, double tap `|`.
- Shift + Caps = left English quote `“`.
- Shift + `'` = right English quote `”`.
- either lower corner: hold Spanish, double tap quick Windows language toggle
  (`Alt+Shift`).
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

Spanish is a transparent character overlay. If a key is not a Spanish
character, it falls through to the active typing layer so mistakes produce normal
typing feedback instead of silence.

```text
trans    ¡      trans  É      trans  trans      trans  Ú      Í      Ó      trans  trans
«        Á      trans  trans  trans  trans      trans  trans  trans  trans  Ü      »
trans    trans  trans  trans  trans  trans      Ñ      trans  trans  trans  ¿      trans
                         trans  trans  trans       trans  trans  trans
```

Use Shift first for capitals:

- Spanish angle quotes are separate keys: `Spanish + Caps = «`,
  `Spanish + ' = »`.
- hold Space or Enter for Shift, then hold Spanish, then press the vowel.
- tested uppercase accented vowels: `Á Í É Ó Ú`.

Language switch:

- double tap either Spanish key to send `Alt+Shift`.
- `Alt+Shift` is the quick Windows language toggle when enabled.
- `Win+Space` opens the Windows language selector, but that gesture is not on
  the keyboard right now.
- Spanish has no RGB layer light in the current build because the typed
  character confirms the layer was active.

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
                         style  ALT    SHIFT       ENTER  RALT  MENU
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
---      CTRL   ---    ---    ---    ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ALT    SHIFT       ENTER  RALT  MENU
```

Navigation key behavior:

```text
Key      Tap      Hold      Double tap        Double-tap-hold
-------------------------------------------------------------
I        Up       Up held   Ctrl+Up           PgUp
J        Left     Left held Ctrl+Left         Home
K        Down     Down held Ctrl+Down         PgDn
L        Right    Right held Ctrl+Right       End
```

Styles: hold `S` or `L`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    UP     ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ALT    SHIFT       ENTER  RALT  MENU
```

Styles owns PowerPoint/Word structure shortcuts:

```text
Tap = Alt+Shift+Arrow.
Hold / double tap / double-tap-hold = intentionally unused for now.
```

Snap window placement: hold `F/J` for Function, then hold left GUI and use
`I/J/K/L`.

```text
The entry gesture exposes the Snap layer. Each arrow sends one Win+Arrow tap,
with Ctrl masked so Windows does not see Ctrl+Win+Arrow.
      Up
Left  Down  Right
```

## Tools

Media: hold `A` or `;` for Control, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    VOL+   ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    MUTE   VOL-   PLAY   ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ALT    SHIFT       ENTER  RALT  MENU
```

Media key behavior:

```text
Key      Action
---------------------------
I        Volume Up
J        Teams mic mute
K        Volume Down
L        Play/Pause
```

Teams mic mute means `Ctrl+Shift+M`. Media hold and double-tap behaviors are
disabled in this RAM experiment.

Function / System: hold `F` or `J`.

```text
TAB      F1     F2     F3     F4     F5         F6     F7     F8     F9     F10    DEL
---      ---    SNIP   ---    ---    ---        ---    F4     F5     F6     F11    COLE
---      ---    cut    copy   paste  ---        ---    F1     F2     F3     F12    ---
                         SNAP   ALT    ENT/SFT     ENTER  RALT  MENU
```

Press the camera icon (`Function + S`) to send the Windows screen snip shortcut
`Win+Shift+S`. Hold Function first, then tap `S`; holding `S` first enters
Symbols. Press scissors (`Function + X`) to send `Ctrl+X`, copy pages
(`Function + C`) to send `Ctrl+C`, paste clipboard (`Function + V`) to send
`Ctrl+V`, tap the left thumb key to send Enter, and `COLE` to toggle Colemak.
Holding the left thumb key on Function still sends Shift.

Mnemonic shortcuts on Base/Colemak: double tap the letter, then hold the second
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
Physical key       Tap              Hold         Double tap          Double-tap-hold
----------------------------------------------------------------------------------
Tab                Tab              Esc          Alt+F4              Alt+F4
Caps               Caps             ---          Long Caps           Long Caps
/                  /                \            |                   |
Symbols (          (                [            {                   {
Symbols )          )                ]            }                   }
Numbers 2          2                ,            <                   <
Numbers 3          3                .            >                   >
Numbers /          /                \            |                   |
Symbols /          /                \            |                   |
Navigation I       Up               Up held      Ctrl+Up             PgUp
Navigation J       Left             Left held    Ctrl+Left           Home
Navigation K       Down             Down held    Ctrl+Down           PgDn
Navigation L       Right            Right held   Ctrl+Right          End
Styles I           Alt+Shift+Up     ---          ---                 ---
Styles J           Alt+Shift+Left   ---          ---                 ---
Styles K           Alt+Shift+Down   ---          ---                 ---
Styles L           Alt+Shift+Right  ---          ---                 ---
Media I            Volume Up        ---          ---                 ---
Media J            Teams mic mute   ---          ---                 ---
Media K            Volume Down      ---          ---                 ---
Media L            Play/Pause       ---          ---                 ---
Spanish key        ---              Spanish      Alt+Shift           ---
Shift+Caps         “                ---          ---                 ---
Shift+'            ”                ---          ---                 ---
```

## Current Physical Trial

- `A` and `;` are Ctrl on hold; `F` and `J` are Function on hold.
- Base Space and Enter are Shift keys on hold.
- Double-tap Caps enters Long Caps, which stays on across words and uses the
  same red RGB as normal Caps.
- In Numbers, both Space and Enter hold Symbols.
- In Symbols, Space and Enter hold Shift; in Function, the left thumb taps
  Enter and holds Shift.
- Watch normal typing for accidental thumb Shift, especially on Space.
- Also check intentional Shift use such as Space/Enter + `/` for `?`.

After flashing, test:

- Space tap = space; Space hold + letter = capital.
- Enter tap = enter; Enter hold + letter = capital.
- Caps tap = normal Caps; Caps double tap = Long Caps; Caps double tap again =
  Caps off.
- Navigation `I/J/K/L` tap = arrows.
- Navigation `I/J/K/L` hold = held arrows.
- Navigation `I/J/K/L` double tap = semantic jumps.
- Navigation `I/J/K/L` double-tap-hold = movement extremes.
- Spanish double tap = quick Windows language toggle (`Alt+Shift`).
- Avoid pressing `L` while GUI is held unless you intend to lock Windows.
- Accidental Alt+Shift and Alt+Space should not switch language or open the
  host window menu.
- Media: `I/J/K/L` = Volume Up / Teams mic mute / Volume Down / Play-Pause.
- Function + `S` = Windows screen snip (`Win+Shift+S`).
- Function + `X/C/V` = Cut / Copy / Paste (`Ctrl+X/C/V`).
- `D/K + left GUI` = Navigation.
- `S/L + left GUI` = Styles: Alt+Shift+Arrow on `I/J/K/L`.
- `A/; + left GUI` = Media.
- `F/J + left GUI`, then `I/J/K/L` = Snap window placement.
- Snap should move windows predictably without opening Snap Assist layouts or
  desktop/workspace traversal.
- double-tap-hold GUI = no separate action; ordinary GUI/anchor hold behavior.
- Double-tap-hold `H/W/P/M/E/G/N` snippets fire only when intended.

Screen check:

- Screens should draw cleanly on both halves.
- Resting Base should show the normal `corne`/katakana label and the resting
  layer tile.
- Colemak should show `clmk ` and the middle anchor tile.
- Spanish hold should show `spnsh` and the middle anchor tile.
- Caps and Long Caps should both show `caps ` and the middle anchor tile.
- Chord layers should show their labels: ` nav `, `style`, `media`, or `snap `.
- Plain Ctrl, Alt, or GUI should not replace the label unless they form a named
  mode such as Snap.
- If display corruption or phantom key reports return, suspect RAM pressure or
  the newer shared directional tap-dance helper before blaming the keymap.

## RGB Meanings

```text
Caps Lock          red        #FF0000
Colemak            purple     #3C0073
Numbers            blue       #0A195F
Symbols            amber      #693C00
Navigation         cyan       #00FFFF
Styles             yellow     #FFFF00
Media              dark teal  #052323
Function/System    green      #4B7A16
Delete cue         red        Backspace key only on command layers with DEL
```

Base and Colemak idle should stay visually quiet. Holding an anchor shows the
active layer and thumb options. Once a thumb chord is pressed, unused thumb
hints turn off. Spanish intentionally has no RGB layer light because it is a
character palette rather than a command surface. Plain Ctrl/Shift/Alt/GUI do not
have RGB feedback.

The physical keyboard named ChieftainDots has a confirmed unavailable `S` RGB
LED, possibly related to the broader left-side LED/column fault. Both halves
were flashed and `Function + S` works, but the `S` LED also stays dark during
the all-red Caps Lock state. Treat this as hardware, not a missing Function-layer
RGB rule, and do not rely on that LED for shortcut discovery until repaired.
