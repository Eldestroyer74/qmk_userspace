# ChieftainDots Engineering Guide

ChieftainDots is the Corne keyboard project, with a compiled Boardsource
Unicorne port target under trial. The active implementation lives in the
`eldestroyer74` QMK userspace. The Corne build recipe is `keymaps/corne.json`;
the Unicorne build recipe is `keymaps/unicorne.json`.

## Current Summary

- Personal userspace is the source of truth; upstream QMK is the build
  dependency.
- Canonical Corne target: `qmk compile users/eldestroyer74/keymaps/corne.json`.
- Boardsource Unicorne target:
  `qmk compile users/eldestroyer74/keymaps/unicorne.json`.
- Current default verification target is Corne only. Compile Unicorne when the
  change is explicitly about Unicorne, board-specific wiring/OLED/RGB, or the
  user asks to bring the Unicorne port back into the active loop.
- Ask before compiling or flashing; only one QMK build should run at a time.
- Last recorded Corne build: 28248 / 28672 bytes, 424 bytes free.
- Last recorded Boardsource Unicorne build: ELF text 65248 bytes; UF2 produced.
- Future keyboard support starts from a fresh current-model recipe and wrapper,
  not old Filterpaper recipes.

## Working Model

- Treat `qmk_firmware` as the upstream build dependency.
- Treat ChieftainDots as personal userspace, not an upstream QMK contribution.
- Treat the personal `qmk_userspace` repository as the intended source of truth.
- Treat any `users/eldestroyer74` copy inside `qmk_firmware` as a build copy
  unless Git confirms it is tracked in the intended userspace repository.
- Treat Filterpaper as ancestry and reference material, not the active identity.
- Make one feature change at a time and compile before moving on.

## Research Before Editing

Before changing behavior, check the current QMK docs, the local QMK source, and
the existing userspace code. Prefer current QMK names and build hooks over older
examples found in inherited docs.

Any QMK callback, hook, config option, or keycode must be verified against at
least one current source before use:

- local QMK source or docs in this checkout
- official QMK docs
- an existing working pattern in this userspace

Do not add a hook just because a name sounds plausible. If the hook is not found
in QMK docs or source, route behavior through a documented callback such as
`process_record_user`, `post_process_record_user`, `keyboard_post_init_user`, or
the documented RGB Matrix indicator callbacks.

Use local inspection first:

```bash
rg "symbol_or_feature_name" users/eldestroyer74 quantum keyboards
```

Then compile the current baseline:

```bash
qmk compile users/eldestroyer74/keymaps/corne.json
```

For the Boardsource Unicorne port target, only when that target is in scope:

```bash
qmk compile users/eldestroyer74/keymaps/unicorne.json
```

Ask before compiling. Only one QMK build should run at a time; if a QMK MSYS
compile is already running, wait for that build to finish before starting
another.

## Firmware Checkpoints

QMK should keep writing its normal output file:

```text
C:\Users\RicardoEscalon\Documents\qmk_firmware\crkbd_rev1_eldestroyer74.hex
```

The Boardsource Unicorne target writes:

```text
C:\Users\RicardoEscalon\Documents\qmk_firmware\boardsource_unicorne_eldestroyer74.uf2
```

After an accepted source commit, copy that file into the local ignored
`firmware-history/` folder with a Julian-style date, short commit SHA, and short
behavior name:

```text
firmware-history/2026128_6920586_checkpoint-cleanup.hex
```

These firmware files are for quick QMK Toolbox rollback only. They are not the
source of truth and should not be committed.

## Coding Style

Follow the userspace `.editorconfig`: UTF-8, tabs, and four-column tab width.
Keep C changes small and module-owned:

- Put RGB color names and constants in `rgb/rgb-matrix.h`.
- Put RGB behavior in `rgb/rgb-matrix.c`.
- Put key-event routing in `eldestroyer74.c`.
- Put layer and key-placement concepts in `layout.h`.
- Put build feature flags in `rules.mk`.
- Put QMK configuration in `config.h`.

Prefer short helper functions when they clarify ownership, but avoid clever
abstractions that hide QMK behavior. Add comments only for rules a future reader
would not infer from the code, such as "Caps Lock is a whole-keyboard warning
state."

## QMK MSYS

The working Windows path should avoid spaces:

```text
C:\QMK_MSYS
```

Run QMK from MINGW64. From PowerShell, Codex can use:

```powershell
& 'C:\QMK_MSYS\shell_connector.cmd' -lc 'cd /c/Users/RicardoEscalon/Documents/qmk_firmware && qmk compile users/eldestroyer74/keymaps/corne.json'
```

### Local Toolchain Stack Updates

Treat local toolchain maintenance as a check-first pass. Record installed and
latest versions before updating anything, and update only the tool surface that
is actually stale.

The local stack has these separate surfaces:

- Windows Git, used by PowerShell and normal Windows tooling.
- QMK MSYS installer, the Windows distribution rooted at `C:\QMK_MSYS`.
- QMK MSYS pacman packages, including the QMK CLI package, Python, compilers,
  libraries, the MSYS2 keyring, and the MSYS Git.
- QMK Toolbox, the Windows GUI flasher.
- The outer `qmk_firmware` checkout, which is not part of routine toolchain
  updating and must only be pulled when that is the explicit task.

Check Windows Git from PowerShell:

```powershell
git --version
```

Compare with the latest official Git for Windows release:

```powershell
(Invoke-RestMethod -Headers @{ 'User-Agent'='Git-version-check' } `
  -Uri 'https://api.github.com/repos/git-for-windows/git/releases/latest').tag_name
```

Check installed Windows applications for QMK Toolbox and QMK MSYS:

```powershell
Get-ItemProperty `
  'HKLM:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*', `
  'HKCU:\Software\Microsoft\Windows\CurrentVersion\Uninstall\*' `
  -ErrorAction SilentlyContinue |
  Where-Object { $_.DisplayName -match 'QMK|Toolbox|Git' } |
  Select-Object DisplayName,DisplayVersion,Publisher,InstallLocation
```

Compare QMK Toolbox with the latest official release:

```powershell
(Invoke-RestMethod -Headers @{ 'User-Agent'='QMK-Toolbox-version-check' } `
  -Uri 'https://api.github.com/repos/qmk/qmk_toolbox/releases/latest').tag_name
```

Do not update QMK Toolbox while it is flashing a keyboard. Do not compile or
flash firmware just because the local tools were updated; that remains a
separate approved action.

### Updating QMK MSYS

Treat these as three separate update surfaces:

- The QMK MSYS installer version controls the Windows distribution in
  `C:\QMK_MSYS`.
- Pacman controls the packages inside that installation, including the QMK CLI,
  Python, Git, compilers, libraries, and the MSYS2 keyring.
- The outer `qmk_firmware` Git checkout is the firmware source/build dependency.
  Updating QMK MSYS does not pull or modify that repository, and it does not
  modify the nested ChieftainDots userspace repository.

Check the installed QMK MSYS release from PowerShell:

```powershell
(Get-Item 'C:\QMK_MSYS\unins000.exe').VersionInfo.ProductVersion
```

Compare it with the latest official QMK MSYS release at
`https://github.com/qmk/qmk_distro_msys/releases/latest`. The GitHub API can be
used for a scriptable check:

```powershell
(Invoke-RestMethod -Headers @{ 'User-Agent'='QMK-MSYS-version-check' } `
  -Uri 'https://api.github.com/repos/qmk/qmk_distro_msys/releases/latest').tag_name
```

Check the active tool versions and pending package updates without changing
anything:

```powershell
& 'C:\QMK_MSYS\shell_connector.cmd' -lc 'qmk --version; python --version; git --version; pacman -Q mingw-w64-x86_64-python-qmk; pacman -Qu'
```

If the installer is current but `pacman -Qu` lists updates, close other QMK MSYS
sessions and builds, then run the full package upgrade:

```powershell
& 'C:\QMK_MSYS\shell_connector.cmd' -lc 'pacman -Syu'
```

Accept the proposed package replacements when prompted. For an unattended run,
use `pacman --noconfirm -Syu` only after reviewing the pending package list.
Large compiler or Python transitions can take several minutes and may appear
quiet through `shell_connector.cmd`; do not start a second updater while the
first `pacman` process is still running.

Afterward, open a fresh shell and verify that QMK starts and no updates remain:

```powershell
& 'C:\QMK_MSYS\shell_connector.cmd' -lc 'qmk --version; python --version; git --version; pacman -Q mingw-w64-x86_64-python-qmk; pacman -Qu'
```

An empty `pacman -Qu` result means the package set is current. Messages about no
job control or not being able to set a terminal process group are expected when
using the non-interactive shell connector; use the command exit code and package
transaction result to judge success. A firmware compile is a separate action
and still requires approval under the Compile Gate.

Maintenance record, 2026-06-19: installed QMK MSYS `1.12.0` matched the latest
official release. `pacman -Syu` upgraded 151 packages (277.41 MiB download),
including the MSYS2 keyring, Git, GCC/Clang, Python, QMK CLI package, OpenSSL,
and supporting libraries. It replaced five old MinGW `-git` runtime packages
with their stable package equivalents. Verification reported QMK CLI `1.2.0`,
Python `3.14.6`, Git `2.54.0`,
`mingw-w64-x86_64-python-qmk 1.2.0-4`, and zero pending package updates. No QMK
firmware was compiled or flashed, and neither Git repository was changed by the
toolchain update.

Maintenance check, 2026-07-01: Windows Git reported
`2.54.0.windows.1`, while the latest official Git for Windows release was
`v2.55.0.windows.1`, so Windows Git was the only stale surface found. Installed
QMK MSYS was `1.12.0`, matching the latest official QMK MSYS release. QMK MSYS
reported QMK CLI `1.2.0`, Python `3.14.6`, MSYS Git `2.54.0`, and
`mingw-w64-x86_64-python-qmk 1.2.0-4`; `pacman -Qu` produced no pending package
list. Installed QMK Toolbox was `0.3.3`, matching the latest official QMK
Toolbox release. No package update, firmware compile, flash, userspace edit, or
outer `qmk_firmware` pull was performed as part of this check.

Maintenance update, 2026-07-01: `winget` metadata still offered Git `2.54.0`
after `winget source update`, so Windows Git was updated directly from the
official Git for Windows GitHub release asset `Git-2.55.0-64-bit.exe` using the
installer arguments `/VERYSILENT /NORESTART`. Verification reported
`git version 2.55.0.windows.1`, and Windows uninstall metadata reported Git
`2.55.0`. QMK MSYS remained `1.12.0`, and QMK Toolbox remained `0.3.3`. No QMK
firmware was compiled or flashed, and the outer `qmk_firmware` checkout was not
pulled.

For split Corne flashing, prefer the canonical JSON flash commands so QMK uses
the active ChieftainDots userspace and writes the correct `EE_HANDS` handedness
marker to each half:

```bash
qmk flash users/eldestroyer74/keymaps/corne.json -bl dfu-split-left
qmk flash users/eldestroyer74/keymaps/corne.json -bl dfu-split-right
```

Flash each half while USB is plugged directly into that half. Press the reset
button near the screen when QMK waits for the bootloader. This matters for RGB
and OLED behavior because each half must know whether it is left or right.

QMK Tools/QMK Toolbox can also flash the already-compiled hex quickly. The
2026-07-09 `QK_BOOT` test flashed both Corne halves with QMK Tools faster than
the command-line flash flow, then confirmed the running-firmware bootloader
shortcut worked. Keep compiling from the canonical userspace recipe first so
the flashed file is known-good.

The previous `C:\Program Files\QMK_MSYS` path caused AVR LTO linking to fail
because the toolchain mishandled the space in `Program Files`.

## Known QMK Compatibility Notes

- `EE_HANDS` requires the halves to be flashed with the correct split target.
  If layer keys work but per-key RGB appears on only one side, or right-side
  command-layer indicators do not light, check handedness flashing before
  changing RGB masks.
  If USB is plugged into the right half and physical keys behave as if the
  halves are swapped, such as physical `F` producing `J` or physical Backspace
  firing the left-side Tab/Esc/Close tap dance, treat that as wrong or stale
  right-half handedness first. Re-flash the right half with `dfu-split-right`
  before changing layout code.
- The physical ChieftainDots Corne has a known left-side RGB fault affecting
  the left-side lower columns. If a key action works there but its RGB color is
  missing, compare against the right side and Caps all-red before changing RGB
  code.
- Flow Tap is the preferred first experiment for accidental home-row tap-hold
  activation during normal typing flow. QMK documents `FLOW_TAP_TERM 150` as a
  starting point; ChieftainDots should trial it before making broader
  home-row timing or layer-placement changes.
- Old mouse keycodes such as `KC_MS_L`, `KC_WH_U`, and `KC_BTN1` should use the
  current `MS_LEFT`, `MS_WHLU`, and `MS_BTN1` style names.
- `IGNORE_MOD_TAP_INTERRUPT` has been removed from current QMK and should not be
  used as live guidance.
- Combo source that defines `key_combos` must be visible to QMK keymap
  introspection; use the current introspection build path instead of treating it
  as ordinary shared `SRC`.
- Tap Dance source that defines `tap_dance_actions` must also be visible to QMK
  keymap introspection. Put the action table in a feature file such as
  `features/tap_dance.c`, include it from the introspection aggregator, wire that
  aggregator with `INTROSPECTION_KEYMAP_C`, and do not add the same file to
  `SRC`.
- QMK keymap introspection accepts one include file. Use
  `features/introspection.c` as the aggregator for Tap Dance, future combos, and
  any other introspected tables.
- `layout.h` is included through `config.h`, so it can be seen by AVR assembly
  sources during the build. Keep declarations in `layout.h` preprocessor-safe;
  avoid C-only declarations such as `enum` there.
- Legacy keymap wiring that names `filterpaper` should not be copied into the
  active ChieftainDots userspace unless the goal is explicitly to build
  Filterpaper's original userspace.

## Size Pressure

The last recorded Corne build succeeded at 22392 / 28672 bytes, leaving 6280
bytes free. Any feature that adds code must include a size-risk check. If a
feature pushes the firmware over size, optimize that feature or roll it back
before starting another one.

ChieftainDots now uses more than eight layers, so `config.h` must use
`LAYER_STATE_16BIT`. If QMK reports that the number of keymap layers exceeds
`LAYER_STATE_(8|16|32)BIT`, check the layer count before changing behavior.

Good practice is part of size discipline, but it must be measured. Prefer
cleanup that clarifies ownership and exposes unused compiled behavior: remove
unreachable layer tables, remove handler paths that no active keycode can call,
centralize shared constants, and derive feedback from the keymap where practical.
Do not count comments, docs, ignored private files, archived files, or
preprocessor-only aliases as firmware savings unless a compile proves a size
change.

## OLED Lessons

ChieftainDots currently keeps Filterpaper's OLED architecture: Bongocat renders
on the master/primary side, and `oled-icons.c` renders the compact status panel
on the secondary side. Do not casually change this to physical-left/physical-
right behavior; the cat wake/sleep timer and split activity sync need deliberate
testing if the cat moves away from the master side.

The current status-screen direction is deliberately minimal. Reuse the existing
logo/text slot, fixed middle tile slot, and bottom 2x2 status stack before
adding new renderers. Name firmware art by ChieftainDots state, not inherited
source art: `base_layer`, `anchor_layer`, and `chord_layer`. The middle tile
should stay fixed while its image changes. The small five-cell text slot may
show compact lowercase chord names such as `nav` and `style`; avoid full-screen
raw chord labels unless a compile proves the bytes and refresh cost are worth
it.

Generated 30x16 outline/filled icon artwork for all concepts is too expensive
for the current firmware budget. A generated `oledfont_icons.c` trial used glyph
codes beyond `0xFF`, conflicted with existing font positions, and represented
roughly 1KB or more of new font data. Future OLED art should be tiny: start with
only the four home-row concepts (Symbols, Numbers, Control, Shift), reuse or
replace a small number of existing glyphs, and measure size before flashing.

Do not stack Filterpaper's existing Ctrl/Shift modifier glyph fragments as if
they were standalone full-height tiles. They were designed for the original
horizontal modifier panel and look partially drawn when used vertically by
themselves. Either preserve the original panel, create purpose-built tiny
home-row glyphs, or use text labels as a temporary readable fallback.

For the bottom home-row status stack, preserve the existing composition grammar
in `oled-icons.c`: `render_home_pair()` writes a two-cell left tile, one real
connector glyph, and a two-cell right tile, then repeats the same structure for
the bottom row. Do not invent a new connector or treat the connector column as a
separate concept icon. If the status buttons look wrong after a concept reorder,
replace the correct top/bottom two-cell concept tiles (`fn_*`, `sym_*`,
`num_*`, `ctrl_*`) and preview them with the real `off_off`, `on_off`,
`off_on`, and `on_on` connector glyphs before changing firmware bytes. Buttons
should use the full 12-pixel width of their two glyph cells so they do not look
narrower than the inherited status buttons.

The home-row status tiles are also position-shaped, not just semantic icons.
Left buttons and right buttons use different 12x16 frame shapes inside the same
30-pixel-wide screen. If a concept moves from one side of the 2x2 stack to the
other, redraw or remap the concept glyph into the destination side's frame
instead of only moving the old named tile. Then check optical centering inside
the visible frame: small symbols such as the Control caret may need a one-pixel
shift to look centered, while text-like symbols such as `Fn` should be compared
against the inherited `@`/`#` tile balance. Regenerate the OLED asset preview
from real `oledfont.c` bytes before compiling.

When changing these four buttons, follow this narrow workflow:

1. Treat each button as a 12x16 tile made from two 6x8 font glyphs on the top
   row and two 6x8 font glyphs on the bottom row. The full row is always left
   tile + 6x16 connector + right tile.
2. Decide whether the concept is in a left-side or right-side slot before
   editing art. Do not reuse a right-side tile on the left or a left-side tile
   on the right just because the semantic name matches.
3. Change the existing glyph bytes in `oled/oledfont.c` for the relevant
   `fn_*`, `sym_*`, `num_*`, or `ctrl_*` slots instead of changing
   `render_home_pair()` or connector logic.
4. Preserve both the off and on versions of the same symbol. The on tile is the
   filled-button version with the symbol cut out as negative space.
5. Regenerate `docs/chieftainDots-oled-assets-preview.html` and inspect
   `docs/chieftainDots-oled-status-preview.png` before compiling. Compare
   left-side concepts against the `#` tile and right-side concepts against the
   `@` tile for optical centering.
6. Only compile after the preview looks right; glyph-byte replacements should
   keep the `oledfont.c` table length unchanged.

If considering a full chord-discovery OLED concept, first measure how many bytes
are recovered by removing Bongocat. Treat that as a separate product decision:
the tradeoff is visual personality versus functional discoverability.

## Rollback Rule

Every behavior change should be small enough to undo alone. If a feature fails
to compile, cannot fit, or feels worse after flashing, revert only that feature
and keep the rest of the project intact.
