# ChieftainDots Change Workflow

This workflow protects ChieftainDots from accumulating tangled behavior. It is
intended for one feature at a time.

## Required Loop

1. Capture the feature as an Idea in the roadmap.
2. Discuss the user goal, ergonomics, and success criteria.
3. Research QMK native support, current QMK docs, community practice, best
   implementation patterns, and local code implications.
4. Challenge the feature against the current principles.
5. Verify every QMK hook, callback, keycode, and config option against local QMK
   source, official QMK docs, or an existing working userspace pattern.
6. If there is a conflict, decide whether to change the feature or revise the
   principle before implementation.
7. Check architecture ownership: layout, feature module, OLED, RGB, combo, or
   userspace hook.
8. Check the proposed implementation for elegance, readability, and
   maintainability before editing.
9. Estimate firmware size risk.
10. Compile the current baseline.
11. Implement only the approved feature.
12. Compile again and record firmware size.
13. Review the finished code for elegance, readability, maintainability, and
   byte cost.
14. Fix bugs or optimize size if needed.
15. Flash the firmware.
16. Trial the behavior on the physical keyboard.
17. Decide to keep, revise, or roll back.
18. If kept, stage and commit only the files that belong to that feature.

No second feature starts until the current feature is marked Kept or Rolled Back.

## Private Data Gate

Do not commit personal addresses, passwords, recovery keys, private URLs with
tokens, or other sensitive strings. Text snippets that are safe to share may
live in committed source. Private snippets must live in an ignored local file,
currently `features/text_stubs_private.h`.

Before staging or committing any text-snippet feature:

1. Run `git status --short --ignored`.
2. Confirm `features/text_stubs_private.h` is ignored and not staged.
3. Inspect staged diffs for private address strings.
4. Replace any committed private value with a placeholder before committing.

## Question And Bug Triage Gate

When the user asks a question about behavior, first answer the question and teach
the likely cause. Do not edit code in the same step unless the user explicitly
asks for the fix.

When a bug is observed after flashing:

1. Explain what the behavior probably means.
2. Name the file or subsystem likely involved.
3. Describe the smallest safe fix and any tradeoff.
4. Wait for confirmation before changing code.

This keeps diagnosis, approval, and implementation separate.

## Principle Gate

Each feature must answer one of these before implementation:

- It supports an existing principle.
- It exposes a gap in the principles and the principle will be updated.
- It conflicts with a principle and the feature will be changed.
- It conflicts with a principle and the principle will be deliberately revised.

## Code Quality And Byte Gate

Before keeping a code change, review it against these questions:

- Is it elegant: does it solve the problem at the right level of the
  architecture, without duplicating another source of truth?
- Is it readable: can future-us understand the behavior from names, structure,
  and a small number of useful comments?
- Is it maintainable: if the layout or feature changes later, how many places
  must be updated?
- Is it byte-aware: does it reuse QMK or existing local structures instead of
  adding parallel tables, duplicate masks, or extra state?

The preferred byte-saving pattern is to derive behavior from the canonical
source of truth instead of mirroring it. For example, RGB layer masks should
follow the actual keymap when possible, rather than maintaining a separate
manual row/column map that can drift and consume firmware space.

## Compile Gate

Use the canonical Corne build:

```bash
qmk compile users/eldestroyer74/keymaps/corne.json
```

Record whether the build succeeded and how much flash space remains. If the
firmware no longer fits, do not start another feature until the size issue is
resolved or the feature is rolled back.

## Flash Gate

A compiled feature is not Kept until it has been flashed and tried on the actual
keyboard. If the feature feels worse in real use, revise it or roll it back.

For the split Corne, use the canonical JSON flash commands when RGB, OLED, or
split-half behavior is being tested. These commands set the `EE_HANDS`
left/right handedness markers as part of flashing:

```bash
qmk flash users/eldestroyer74/keymaps/corne.json -bl dfu-split-left
qmk flash users/eldestroyer74/keymaps/corne.json -bl dfu-split-right
```

Flash the left half with USB plugged into the left controller, then flash the
right half with USB plugged into the right controller. Press the reset button on
the half being flashed when QMK waits for the bootloader.

Do not use `qmk flash -kb crkbd/rev1 -km eldestroyer74` for ChieftainDots; that
path can pick up the legacy `keyboards/crkbd/keymaps/eldestroyer74` folder
instead of the canonical userspace JSON build.

If split RGB behaves strangely, verify this handedness flash workflow before
changing RGB code. Wrong or stale handedness can make per-key indicators appear
only on one side even when the keymap itself works.

## Firmware Archive Gate

After a source commit is accepted, compile from that committed state and copy the
generated firmware into a local ignored archive. Do not rename QMK's normal
output file in place; keep `crkbd_rev1_eldestroyer74.hex` as the build output
and copy it into `firmware-history/`.

Use Julian-style dates without dashes, followed by the short commit SHA and a
short behavior name:

```text
firmware-history/2026128_6920586_checkpoint-cleanup.hex
firmware-history/2026128_s-held-numbers-trial.hex
```

The archive is local and ignored by git. The commit remains the source of truth;
the archived firmware is a quick physical rollback file for QMK Toolbox.

## Git Checkpoint Gate

Keep experimental work unstaged while a feature is being researched, compiled,
flashed, and revised. This makes it easy to inspect or roll back that feature
without mixing it with older accepted work.

After the feature is flashed and accepted as Kept:

1. Review `git status`.
2. Stage only the files that belong to the accepted feature.
3. Commit with a message that names the behavior, not just the file touched.
4. Leave unrelated, unfinished, or exploratory changes unstaged.

Do not stage, commit, or push without explicit approval.
