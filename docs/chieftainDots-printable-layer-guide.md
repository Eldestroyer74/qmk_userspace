# ChieftainDots Daily Layer Guide

Print this as the desk-side map for the future three-anchor command layout.
This guide is intentionally forward-looking so the whole design can be reviewed
as one coherent keyboard.

Stable trial note: OLED/screens are on. The current safe OLED slice keeps the
older, smaller Navigation/MS Styles tap-dance logic, flattens Media to plain
keys, and avoids the newer shared directional tap-dance helper that caused
phantom modifier reports in QMK key tester.

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

Hold D/K + left GUI    = Navigation: movement only
Hold S/L + left GUI    = Office: arrows / structure / selection
Hold A/; + left GUI    = Media, Teams-first
Double tap-hold mnemonic = Text snippets
```

Snap is not active in the current stable trial. Keep it as a deferred chord
candidate, not a daily-use instruction.

```text
Command gesture rule

Tap             = direct daily action
Tap and hold    = sustained / larger / repeated action
Double tap      = secondary reversible action
Double tap-hold = empty unless a real need is proven
```

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

Office: hold `S/L`, then hold left GUI.

```text
Tap: arrow navigation inside Office content
      Up
Left  Down  Right

Hold: PowerPoint / Word structure
      Alt+Shift+Up
Alt+Shift+Left  Alt+Shift+Down  Alt+Shift+Right

Double tap: select word / line
      Shift+Up
Ctrl+Shift+Left  Shift+Down  Ctrl+Shift+Right

Double tap-hold: select extremes
      Shift+PgUp
Shift+Home  Shift+PgDn  Shift+End
```

Media: hold `A/;`, then hold left GUI.

```text
Plain keys only in the current RAM experiment:
      Volume Up
Teams mic mute  Volume Down  Play/Pause
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
- tested uppercase accented vowels: `Á Í É Ó Ú`.

Language switch:

- double tap either Spanish key to send `Win+Space` once.
- double tap and hold either Spanish key to hold the Windows selector open.
- left Spanish double-tap-hold starts backward/up.
- right Spanish double-tap-hold starts forward/down.
- while held, press `I` to move backward/up or `K` to move forward/down.
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
---      ---    ---    ---    CTRL   ---        ---    LEFT   DOWN   RIGHT  ---    ---
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

Office: hold `S` or `L`, then hold left GUI.

```text
---      ---    ---    ---    ---    ---        ---    ---    UP     ---    ---    DEL
---      ---    ---    ---    ---    ---        ---    LEFT   DOWN   RIGHT  ---    ---
---      ---    ---    ---    ---    ---        ---    ---    ---    ---    ---    ---
                         trans  ALT    SHIFT       ENTER  RALT  MENU
```

Office owns content editing and selection:

```text
Tap = plain arrow navigation.
Hold = Alt+Shift+Arrow for structure.
Double tap = selection movement.
Double tap-hold = extreme selection.
```

Deferred Snap candidate: no active entry path in the current stable trial.

```text
Tap = snap current window
      Win+Alt+Up
Win+Left   Win+Alt+Down   Win+Right

Hold = current-window state or monitor movement
      Win+Up
Win+Shift+Left   Win+Down   Win+Shift+Right

Double tap = desktop / workspace
      Win+Tab
Win+Ctrl+Left   Win+D   Win+Ctrl+Right
```

Keep this candidate parked until the Ctrl + GUI chord is rebuilt safely.

## Tools

Media: hold `A` or `;`, then hold left GUI.

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
Office I           Up               Alt+Shift+Up Shift+Up            Shift+PgUp
Office J           Left             Alt+Shift+Left Ctrl+Shift+Left   Shift+Home
Office K           Down             Alt+Shift+Down Shift+Down        Shift+PgDn
Office L           Right            Alt+Shift+Right Ctrl+Shift+Right Shift+End
Media I            Volume Up        ---          ---                 ---
Media J            Teams mic mute   ---          ---                 ---
Media K            Volume Down      ---          ---                 ---
Media L            Play/Pause       ---          ---                 ---
Spanish key        ---              Spanish      Win+Space           Language selector
Shift+Caps         “                ---          ---                 ---
Shift+'            ”                ---          ---                 ---
```

## Current Physical Trial

- `F` and `J` are Ctrl on hold, not Shift.
- Base Space and Enter are Shift keys on hold.
- Double-tap Caps enters Long Caps, which stays on across words and uses the
  same red RGB as normal Caps.
- In Numbers, both Space and Enter hold Symbols.
- In Symbols and Function, Space and Enter hold Shift.
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
- Spanish double tap = Windows language switch.
- Spanish double-tap-hold + `I/K` = traverse the language selector.
- Avoid pressing `L` while GUI is held unless you intend to lock Windows.
- Accidental Alt+Shift and Alt+Space should not switch language or open the
  host window menu.
- Media: `I/J/K/L` = Volume Up / Teams mic mute / Volume Down / Play-Pause.
- `D/K + left GUI` = Navigation.
- `S/L + left GUI` = Office: arrows, structure, selection, extreme selection.
- `A/; + left GUI` = Media.
- Double-tap-hold `H/W/P/M/E/G/N` snippets fire only when intended.

Screen check:

- Screens should draw cleanly on both halves.
- If display corruption or phantom key reports return, suspect RAM pressure or
  the newer shared directional tap-dance helper before blaming the keymap.

## RGB Meanings

```text
Caps Lock          red        #FF0000
Colemak            purple     #3C0073
Numbers            blue       #0A195F
Symbols            amber      #693C00
Navigation         cyan       #00FFFF
Office             yellow     #FFFF00
Media              dark teal  #052323
Function/System    green      #4B7A16
Delete cue         red        Backspace key only on command layers with DEL
```

Base and Colemak idle should stay visually quiet. Holding an anchor shows the
active layer and thumb options. Once a thumb chord is pressed, unused thumb
hints turn off. Plain Ctrl/Shift/Alt/GUI do not have RGB feedback.

The leftmost LED columns on the left half may not light on this physical board,
so do not rely on those LEDs for chord discovery.
