# ChieftainDots Change Workflow

This workflow protects ChieftainDots from accumulating tangled behavior. It is
intended for one feature at a time.

## Current Summary

- Treat questions as questions: explain first, then edit only after approval.
- Work one feature at a time from roadmap idea to physical trial.
- On a fresh session, start from the roadmap Current Focus before reading long
  history.
- Check Git state before editing, especially userspace source-of-truth state.
- Ask before compiling or flashing.
- Do not stage, commit, or push without explicit approval.
- Check for private values before staging text-snippet work.

## Session Resume Protocol

Use this when moving to a new chat, returning after time away, or recovering
from a long experimental thread. The goal is to rebuild context from stable
project documents instead of guessing from memory.

Start with these sources, in this order:

1. `README.md`: quick project identity, build target, active architecture shape,
   and daily-doc index.
2. `docs/chieftainDots-roadmap.md`, especially **Current Focus**: authoritative
   resume point, current trial, latest measurements, rollback notes, and parked
   decisions.
3. `docs/chieftainDots-change-workflow.md`: process rules for research,
   approval, compiling, flashing, staging, private data, and session handoff.
4. `docs/chieftainDots-architecture.md`: current layer model, ownership
   boundaries, and stable principles before code changes.
5. `docs/chieftainDots-printable-layer-guide.md`: current user-facing physical
   behavior and test map.
6. Topic-specific docs only when needed: the OLED screen brief and asset audit
   for OLED work, the shift trial log for typing-timing work, or Git history for
   old Filterpaper reference material.

Before editing in a resumed session:

1. Run Git status inside the personal userspace repository, not only the outer
   QMK checkout.
2. Identify the latest accepted commit and any uncommitted trial files.
3. Identify whether the current request is a question, a requirement discussion,
   an implementation request, or a cleanup request.
4. Confirm the active trial and rollback point from **Current Focus**.
5. If behavior changed physically since the docs were last updated, update the
   roadmap before treating the old notes as current truth.

At the end of a session or before switching threads, leave enough context for
the next session:

1. Update **Current Focus** with the active trial, compile result, flash/test
   state, firmware bytes, and rollback commit or tag.
2. Update the printable guide when user-facing key behavior changed.
3. Update architecture or principles when a decision changes the model, not just
   one key.
4. Record dismissed experiments with the reason and measured byte result when
   relevant, then remove dead code unless it is intentionally behind a small
   named switch.
5. State the nested userspace Git status, including staged and unstaged files.
6. Do not stage, commit, compile, or flash merely to make the handoff tidy; those
   still require explicit approval.

Decision locations:

- Roadmap: active focus, requirements, trial status, evidence, dismissed ideas,
  and rollback references.
- Architecture: stable layer model, ownership boundaries, and design rules.
- Change workflow: collaboration protocol and safety gates.
- Printable guide: daily physical behavior, not speculative future design.
- Git commits and tags: exact return points for accepted slices and preserved
  experiments.

## Required Loop

1. Check the current repository state before changing files: branch, short
   status, recent commits, and whether the ChieftainDots userspace is tracked or
   untracked in this checkout.
2. Capture the feature as an Idea in the roadmap.
3. Discuss the user goal, ergonomics, and success criteria.
4. Research QMK native support, current QMK docs, community practice, best
   implementation patterns, and local code implications.
5. Challenge the feature against the current principles.
6. Verify every QMK hook, callback, keycode, and config option against local QMK
   source, official QMK docs, or an existing working userspace pattern.
7. If there is a conflict, decide whether to change the feature or revise the
   principle before implementation.
8. Check architecture ownership: layout, feature module, OLED, RGB, combo, or
   userspace hook.
9. Check the proposed implementation for elegance, readability, and
   maintainability before editing.
10. Estimate firmware size risk.
11. Ask before compiling. Only one QMK build should run at a time, because QMK
    MSYS builds can collide or waste time when another compile is already in
    progress.
12. Compile the current baseline only after confirmation.
13. Implement only the approved feature.
14. Ask again before the post-change compile.
15. Compile again and record firmware size.
16. Review the finished code for elegance, readability, maintainability, and
   byte cost.
17. Fix bugs or optimize size if needed.
18. Ask before flashing.
19. Flash the firmware.
20. Trial the behavior on the physical keyboard.
21. Decide to keep, revise, or roll back.
22. If kept, stage and commit only the files that belong to that feature.

No second feature starts until the current feature is marked Kept or Rolled Back.

## Git Userspace Gate

ChieftainDots is personal userspace. It should not be prepared as a contribution
to upstream `qmk/qmk_firmware` unless that goal is explicitly changed.

Use Git from the ChieftainDots userspace root:

```bash
cd users/eldestroyer74
git status --short --branch
```

Do not use the outer `qmk_firmware` Git status as the source of truth for
ChieftainDots decisions. The outer checkout is mainly the QMK build dependency;
it may report `users/eldestroyer74` as untracked, ignored, or nested, even when
the userspace itself has its own real history. Commits, tags, staging, diffs,
and rollback points for ChieftainDots belong to the nested userspace repository
unless the task explicitly says otherwise.

When reporting state, name which repository was checked:

- `userspace git`: the project history for ChieftainDots work.
- `outer qmk_firmware git`: the upstream firmware dependency and build checkout.

Use this source-of-truth model:

- `qmk_firmware` is the upstream build dependency.
- The personal `qmk_userspace` repository is the intended home for
  ChieftainDots history.
- A `users/eldestroyer74` folder inside `qmk_firmware` may be a convenient
  build copy, but it is risky as the only source of truth if Git reports it as
  untracked.

Before code or documentation work that should be kept:

1. Run `git status --short --branch`.
2. Run `git ls-files users/eldestroyer74`.
3. If `users/eldestroyer74` is untracked, do not assume the current checkout
   contains recoverable ChieftainDots history.
4. Check whether the personal userspace remote or clone contains the latest
   ChieftainDots commits, tags, and docs.
5. Do not clean, delete, reset, or move untracked userspace files until the
   personal source-of-truth repository is confirmed.

Recommended destination: keep ChieftainDots in a personal External Userspace
repository and configure QMK to use it as an overlay. The QMK checkout can still
be used for compile and flash work, but personal behavior commits should live in
the userspace repository.

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

Byte is king when code is unused. Before enabling or keeping a QMK feature,
source file, helper, or config option, ask what active behavior uses it. If the
answer is "nothing in the current Corne build," disable or remove it and measure
the byte difference. Git history and the roadmap are the archive; compiled
firmware should not carry dormant experiments.

Good practice can create byte savings by making dead compiled behavior visible.
Run this as a repeatable audit, not as a broad refactor:

1. Pick one ownership problem: duplicated layer ids, stale layer tables,
   repeated handler paths, enabled-but-unused QMK features, or custom code that
   duplicates a simpler keymap/QMK mechanism.
2. Identify the active source of truth before editing.
3. Separate clarity-only cleanup from compiled behavior. Comments,
   preprocessor-only aliases, ignored private files, archived code, and docs
   usually do not change firmware size.
4. Make the smallest behavior-preserving cleanup.
5. Compile with the canonical QMK MSYS path and record the exact firmware size.
6. Call it a byte win only if the compile proves it. Otherwise record it as
   readability or maintainability cleanup.

Useful repeat candidates:

- remove unreachable layer tables only after confirming no active recipe names
  them
- remove unused `process_record_user()` branches only after confirming no
  active keycode can call them
- centralize shared constants so feature modules stop carrying hardcoded
  numeric copies
- prefer deriving RGB/OLED feedback from the keymap where practical instead of
  maintaining parallel maps
- disable QMK feature flags only when their visible behavior is not part of the
  current trial

Do not treat visible features as free cleanup. OLED, RGB, Caps Unlock, text
snippets, Tap Dance, Extra Keys, and split recovery settings may be expensive,
but they are design choices unless the user explicitly decides they are no
longer worth their bytes.

## Compile Gate

Use the canonical Corne build:

```bash
qmk compile users/eldestroyer74/keymaps/corne.json
```

Ask before running this command. If the user already has QMK MSYS compiling,
wait until that build finishes before starting another compile.

Record whether the build succeeded and how much flash space remains. If the
firmware no longer fits, do not start another feature until the size issue is
resolved or the feature is rolled back.

## Flash Gate

A compiled feature is not Kept until it has been flashed and tried on the actual
keyboard. Ask before flashing. If the feature feels worse in real use, revise it
or roll it back.

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
