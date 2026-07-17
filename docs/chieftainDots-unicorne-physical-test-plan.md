# ChieftainDots Unicorne Physical Test Plan

Test target: Boardsource Unicorne flashed from
`users/eldestroyer74/keymaps/unicorne.json`.

## Phase 1/2 acceptance build

Before testing visual adapters, compile and retain a phase-1/2 UF2 with the
inherited pointing device disabled and the shared ChieftainDots layout intact.
That build is the rollback artifact for input and behavior testing.

```bash
qmk compile -e CHIEFTAINDOTS_UNICORNE_VISUALS=no users/eldestroyer74/keymaps/unicorne.json
```

- Confirm the pointer remains still for at least two minutes after USB attach.
- Confirm ordinary typing does not emit mouse buttons, wheel, or pointer motion.
- Test Base and Colemak, then all four home-row anchors.
- Test Numbers, Symbols, Navigation, Snap, Media, MS Styles, and Spanish.
- Test smart quotes and only non-private text-stub examples.
- On Function, test Select All, Snip, Undo, and the M40 third-left-thumb
  `Enter` tap / left-`Alt` hold.
- Confirm Base/Colemak keep `Space` tap / left-`Alt` hold at that same thumb.
- Confirm Caps behavior and the daily host shortcuts you rely on.

Record differences rather than assuming the shared layer wrapper proves parity.

## Phase 3 recovery and split checks

Perform these only after phase 1/2 input behavior is acceptable:

1. Prove Bootmagic independently on each physical half.
2. Prove the running-firmware boot shortcut independently on each half.
3. Attach USB to each half in turn and record master/left/right behavior.
4. Verify inter-half USB-C operation and recovery after flashing each half.
5. Confirm reset access and a recovery path before testing visual adapters.

Do not apply Corne TRRS assumptions to this board; Unicorne uses its own
inter-half USB-C hardware path.

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

Phase-1/2 artifact expectation: Boardsource/default RGB. Phase-4 source
expectation after a separate manual compile: ChieftainDots indicator RGB using
the board's generated matrix-to-LED lookup. Phase 4 is not compile-verified yet.

Enable the phase-4 visual adapters explicitly:

```bash
qmk compile -e CHIEFTAINDOTS_UNICORNE_VISUALS=yes users/eldestroyer74/keymaps/unicorne.json
```

Compile again with the switch set to `no` for a no-edit phase-4 rollback.

Observe and record:

- Resting RGB pattern and colors.
- What changes on Numbers.
- What changes on Symbols.
- What changes on Function.
- What changes on Snap.
- Whether Caps Lock has any visible indicator.
- Whether LEDs correspond generally to the physical keys or feel offset.

For phase 4, report any physical offset as an adapter defect; do not patch it
with guessed hard-coded LED numbers before checking `g_led_config`.

## OLED Observation

Phase-1/2 artifact expectation: Boardsource/default OLED. Phase-4 source
expectation: Bongocat on the USB-connected master and ChieftainDots status on
the non-master, with Boardsource's default task suppressed. The Unicorne
userspace adapter owns the accepted role-aware rotations: left-master Bongocat
at 0 degrees, right-master Bongocat at 180 degrees, and status on either
non-master at 270 degrees.

Observe and record:

- With USB left, confirm Bongocat is upright on the left and status is upright
  on the right.
- With USB right, confirm status is upright on the left and Bongocat is upright
  on the right.
- Whether layer images change when holding Symbols, Numbers, Function, or Snap.
- Let both screens sleep, then confirm the first key wakes both and still
  produces its normal input/animation.
- Confirm swapping the USB side swaps roles without stale or overwritten art.

In a phase-4 build, the Boardsource logo or gear/layer screen means the
userspace adapter did not take callback ownership as intended.

## Pass Criteria For This Flash

This flashed build passes the first physical trial if:

- Both halves have correct handedness.
- Base typing is usable.
- Numbers and Snap work.
- Symbols and Function work enough for normal use.
- Double-tap-hold `A/S/V` and Shift Enter do not create dangerous accidental
  behavior.
- Spanish still works.
- RGB/OLED match the phase being tested: board defaults for phase 1/2, or the
  documented ChieftainDots adapters for phase 4.

## Stop Conditions

Stop testing and report details if:

- halves are swapped
- one half does not respond
- typing produces repeated wrong-side keys
- a normal key triggers destructive shortcuts
- Space/Enter Alt holds open host menus unexpectedly
- firmware becomes hard to recover into bootloader
