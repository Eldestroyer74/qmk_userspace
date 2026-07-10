# ChieftainDots Unicorne Physical Test Plan

Test target: Boardsource Unicorne flashed from
`users/eldestroyer74/keymaps/unicorne.json`.

Current expectation: core ChieftainDots key behavior should work. RGB and OLED
are still Boardsource/default feedback and should be observed, not judged as
final ChieftainDots behavior yet.

## Before Testing

- Confirm both halves were flashed with the correct handedness:
  - left half from `uf2-split-left`
  - right half from `uf2-split-right`
- Plug USB into the side you normally intend to use first.
- Open a plain text editor for typing tests.
- Keep a normal keyboard available in case a shortcut locks focus or opens a
  host UI.

## Boot And Split Sanity

- Plug USB into the left half.
- Confirm both halves type on the expected physical side.
- Press physical `F`; expected output is `f`, not `j`.
- Press physical `J`; expected output is `j`, not `f`.
- Tap a few keys on each half and confirm there is no left/right swap.
- If left/right are swapped, stop and re-flash both halves with the split UF2
  targets before testing behavior.

Repeat the same quick side check with USB plugged into the right half if that
workflow matters.

## Base Typing

In a text editor:

- Type the alphabet across both hands.
- Confirm Backspace works.
- Confirm Space and Enter work.
- Confirm comma, dot, slash, quote, semicolon, and Tab behave normally.
- Confirm ordinary words with `a`, `s`, `v`, and Shift do not accidentally fire
  shortcuts.

Suggested phrase:

```text
the quick brown fox jumps over a lazy dog, then saves a value.
```

## Thumb Behavior

- Tap Space: expected space.
- Hold Space, then tap another key: expected Alt behavior only when intended.
- Tap Enter: expected enter.
- Hold Enter, then tap another key: expected Alt behavior only when intended.
- Hold either middle thumb Shift and type letters: expected capitals.
- Double-tap-hold either middle thumb Shift: expected Enter / Submit.

Watch for accidental `Alt+Space` menus or language switching.

## Home-Row Anchors

Test each parent layer:

- Hold `A` or `;`: Control anchor.
- Hold `S` or `L`: Symbols.
- Hold `D` or `K`: Numbers.
- Hold `F` or `J`: Function.

For each anchor:

- Confirm the expected layer keys work.
- Release the anchor and confirm typing returns to Base.
- Confirm the mirrored right-hand anchor behaves the same conceptually as the
  left-hand anchor.

## Known Working Spot Checks

These were already reported as working, but repeat after both halves are
settled:

- Numbers layer: hold `D` or `K`, type several numbers.
- Snap: hold Function (`F` or `J`) plus GUI, then test the arrow cluster.

Use a harmless window for Snap testing.

## Function Layer

Hold `F` or `J`:

- Test `F1`, `F2`, `F3`.
- Test the right-hand function cluster.
- Test Colemak toggle only if you are ready to toggle back.
- Confirm Function no longer has duplicate `X/C/V` clipboard shortcuts in this
  trial.

## Mnemonic Double-Tap-Hold Actions

In a text editor:

- Type a short sentence.
- Double-tap-hold `A`: expected Select All.
- Double-tap-hold `V`: expected Paste.
- Double-tap-hold `S`: expected Windows screen snip.
- Double-tap-hold Shift: expected Enter / Submit.

Guardrails:

- Quick double-tap `A`, `S`, or `V` should still behave like ordinary repeated
  letters when not held.
- `X` and `C` should not fire Cut/Copy through double-tap-hold in this trial.

## Spanish

- Hold either lower-corner Spanish key.
- Test accented vowels on their letter positions.
- Test `ñ`, `ü`, `¿`, and `¡`.
- Mistype an unrelated key while Spanish is held and confirm transparent
  fallthrough behaves as expected.
- Double-tap either Spanish key only if you want to test Windows language
  toggle.

## RGB Observation

Current expected state: Unicorne shows Boardsource/default RGB, not the final
ChieftainDots RGB language.

Observe and record:

- Resting RGB pattern and colors.
- What changes on Numbers.
- What changes on Symbols.
- What changes on Function.
- What changes on Snap.
- Whether Caps Lock has any visible indicator.
- Whether LEDs correspond generally to the physical keys or feel offset.

Do not treat mismatched ChieftainDots colors as a failure yet. This is input for
the M28/M30 RGB port.

## OLED Observation

Current expected state: Unicorne shows Boardsource/default OLED, not final
ChieftainDots OLED.

Observe and record:

- What the master side shows with USB plugged into left.
- What the non-master side shows with USB plugged into left.
- Whether either display is upside down.
- Whether layer images change when holding Symbols, Numbers, Function, or Snap.
- Repeat with USB plugged into right if you plan to use either side as master.

Do not treat the Boardsource logo or gear/layer screen as a failure yet. This
is input for the M29 OLED port.

## Pass Criteria For This Flash

This flashed build passes the first physical trial if:

- Both halves have correct handedness.
- Base typing is usable.
- Numbers and Snap work.
- Symbols and Function work enough for normal use.
- Double-tap-hold `A/S/V` and Shift Enter do not create dangerous accidental
  behavior.
- Spanish still works.
- RGB/OLED are documented as current board-default behavior.

## Stop Conditions

Stop testing and report details if:

- halves are swapped
- one half does not respond
- typing produces repeated wrong-side keys
- a normal key triggers destructive shortcuts
- Space/Enter Alt holds open host menus unexpectedly
- firmware becomes hard to recover into bootloader
