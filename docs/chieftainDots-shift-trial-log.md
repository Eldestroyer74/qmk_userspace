# ChieftainDots Shift Trial Log

Use this log to collect missed or suspicious `F`/`J` home-row Shift events
before changing firmware. The goal is to separate firmware problems from
technique-learning problems.

## What To Record

- Intended output.
- Actual output.
- Physical sequence.
- Context, especially punctuation, sentence start, isolated `I`, names, or
  ordinary typing.
- Whether slowing down fixes it.
- Notes about hand order or release order.

## Current Questions

- Are misses concentrated around punctuation, such as `F + /` for `?`?
- Are misses concentrated around capitals after punctuation and space?
- Are misses concentrated around specific letters, such as `N` or `I`?
- Do misses disappear when Shift is pressed clearly before the target key?
- Is home-row Shift learnable with the current timing, or does it need a
  fallback such as one-shot Shift?

## Firmware Logger

The temporary debug firmware runs with console on and OLED off. It logs
candidate `F`/`J` Shift misses at the next word boundary:

```text
shift_trial prev=9 next=12 dt=8 mods=0 tap=0 word=first
```

Fields:

- `prev`: `9` is `F`, `13` is `J`.
- `next`: following keycode, such as `12` for `I`, `17` for `N`, or `56` for
  `/`.
- `dt`: milliseconds between `F`/`J` and the next key.
- `mods`: `0` means no Shift was active; `2` means Shift was active.
- `word`: short alphabetic token captured for context.

For shifted candidates, `word` is intentionally logged as `omitted`. The
physical `F`/`J` key may be acting as Shift instead of a letter, so the logger
cannot reliably reconstruct the word.

It also logs sentence-start candidates for the auto-caps-after-punctuation
trial:

```text
autocaps_trial punct=46 next=17 mods=0 auto=1 word=now
```

Fields:

- `punct`: sentence-ending punctuation as an ASCII value: `46` is `.`, `63` is
  `?`, and `33` is `!`.
- `next`: first alphabetic key after punctuation and space.
- `mods`: `0` means no Shift was active; `2` means Shift was active.
- `auto`: `1` means firmware applied weak Shift; `0` means the key was already
  shifted manually.
- `word`: short alphabetic token captured for context.

The trial behavior follows the community Sentence Case pattern: a word must be
typed before `.`, `?`, or `!`; one or more spaces prime the next alphabetic key;
then firmware applies weak Shift only if no Shift is already active. Non-Shift
modifiers, command/navigation keys, the Spanish layer, and non-word symbols clear
the state. The first exception list treats `vs.` and `etc.` as abbreviations
rather than sentence endings.

Privacy rule: the logger omits the word if the token looks complex. Digits,
symbols, non-word keys, and long tokens are logged as `word=omitted`. Do not run
the logger while typing passwords, private addresses, or other sensitive text.

## Results Table

| Date | Build / Layout State | Sample | Shift Active | No Shift | Slash Events | Top Literal Words | Conclusion | Decision |
| --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 2026-05-22 | `F`/`J` home-row Shift, console logger, OLED off, word-context v2 | 202 candidates | 93 | 109 | 0 | `for` 22, `from` 7, `different` 3, `workflow` 3, `fidelity` 3 | Broad `F`/`J` correction is risky because many ordinary words legitimately begin with `f`; slash-specific fix still lacks captured evidence. | Keep collecting; trial guarded auto-caps-after-punctuation behavior with logging. |

## Entries

| Date | Intended | Actual | Sequence | Context | Slowed Retry | Notes |
| --- | --- | --- | --- | --- | --- | --- |
| 2026-05-21 | `?` | `f/` | `F` intended as Shift, then `/` | punctuation | unknown | Repeated enough to discuss eager punctuation. |
| 2026-05-21 | `N` | `fn` | `F` intended as Shift, then `N` | capital letter | unknown | Track whether this is timing/technique or an unreliable chord. |
