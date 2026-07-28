# Printable Guide Renderer

This tool generates the printable ChieftainDots Corne layout poster.

Run from the QMK checkout root. Paper mode is the default and preserves the
existing print-oriented output:

```bash
python users/eldestroyer74/tools/printable-guide/render_corne_guide.py --mode paper --out users/eldestroyer74/imgs
```

The committed output target is:

```text
users/eldestroyer74/imgs/chieftainDots-corne.png
```

Desktop mode places the same complete guide composition on a dark wallpaper
canvas. Set the exact canvas size with `--width` and `--height`, and its
preferred (maximum) composition width with `--content-width`:

```bash
python users/eldestroyer74/tools/printable-guide/render_corne_guide.py --mode desktop --width 3840 --height 1080 --content-width 3200 --out users/eldestroyer74/imgs
```

Desktop defaults are 3840 × 1080 with a preferred content width of 3200. All
three values must be positive integers. The composition scales uniformly to
fit within the canvas and safe margins, so its final width may be less than the
preferred width on a height-constrained canvas. Desktop output uses a distinct
size-specific name, for example:

```text
users/eldestroyer74/imgs/chieftainDots-corne-desktop-3840x1080.png
```

Desktop card titles use Dosis Medium at weight 500, embedded as SVG paths.
This title treatment is desktop-only; paper typography and all non-title
desktop labels retain their existing treatment.

Paper and desktop share semantic card badges rather than sequence numbers.
Base/Colemak use the keyboard mark; Control uses the OLED-derived chevron;
Numbers, Symbols, and Function use `#`, `@`, and `Fn`; Media uses music;
MS Styles uses paragraph; Navigation uses the accepted custom rooster; and
Snap reuses the renderer's right-snap window artwork. Font and icon glyphs are
embedded as paths. The rooster is embedded from the local stable asset
`tools/printable-guide/assets/navigation-rooster-a3.png`.

The generated charcoal/midnight gradient remains the default desktop
background. To render the optional asymmetric blue/purple arc background with
frosted-glass cards, add `--desktop-background image`:

```bash
python users/eldestroyer74/tools/printable-guide/render_corne_guide.py --mode desktop --desktop-background image --width 3840 --height 1080 --content-width 3200 --out users/eldestroyer74/imgs/previews
```

Image mode writes a distinct filename so it cannot overwrite the canonical
gradient output accidentally:

```text
users/eldestroyer74/imgs/previews/chieftainDots-corne-desktop-image-3840x1080.png
```

The stable source asset is
`tools/printable-guide/assets/desktop-arc-background-clean-v2.png`. The
renderer embeds it into the generated SVG, blurs only the background fragments
behind each rounded card, and draws card contents unfiltered above the glass.

For the M64 artefact diagnostic, disable only the Gaussian backdrop blur while
preserving the image, card tint, outline, highlight, shadow, contents, and
geometry:

```bash
python users/eldestroyer74/tools/printable-guide/render_corne_guide.py --mode desktop --desktop-background image --desktop-key-material unused-glass --desktop-glass-blur off --width 3840 --height 1080 --content-width 3200 --out users/eldestroyer74/imgs/previews/glass-diagnostics
```

This preview-only comparison writes a distinct `-glass-no-blur` filename and
must not be promoted as the canonical desktop diagram. If the rectangular card
artefacts disappear, replace the per-card blur with shared, canvas-aligned
focus, alpha, and utility blur surfaces before restoring blur.

The accepted image-mode implementation uses `--desktop-glass-blur shared` by
default. It creates one lossless, full-canvas PNG blurred surface for each card
tier with Pillow, then reveals that aligned surface through every card in the
tier. Edge performs no Gaussian blur in this mode. The rejected browser-filter
implementation remains available only as `--desktop-glass-blur on` and writes
a distinct `-glass-legacy-blur` comparison. This focused glass repair does not
require or invoke Affinity.

For the reversible M59 CONTROL-card material study, use
`--desktop-key-material etched`, `etched-strong`, `etched-bolder`, or
`etched-wayfinding`. The additional `etched-wayfinding-refined` comparison
strengthens the active Control colour, quiets solid sibling cues, and explicitly
shows the mirrored `;` anchor. The wayfinding options prototype the intended universal visual
grammar: solid active chord members and anchor cues, bolder etched payloads,
and lightly etched ordinary keys at the normal utility-card content opacity.
These modes make only CONTROL key wells translucent so the card's single
blurred background pane and lighting remain continuous through them. They write
separately named preview files containing the selected key-material name; the
default `opaque` mode and regular wallpaper filename remain unchanged.

The current full-diagram candidate is `--desktop-key-material unused-glass`.
It keeps every functional, payload, anchor, cue, and repeated functional thumb
solid; only genuinely unused positions reveal the card's shared blurred pane.
It also shows both mirrored home-row anchors on command cards. Like the earlier
experiments, it writes a distinct preview filename and does not replace the
default opaque wallpaper.

The renderer uses the installed user-font copies of Dosis and Font Awesome 5
Free Solid from:

```text
%LOCALAPPDATA%\Microsoft\Windows\Fonts
```

It renders SVG in memory, writes a temporary SVG outside the repository, and
captures it with headless Microsoft Edge. Only the PNG is written to the
userspace. No generated SVG needs to be committed for the current workflow.

If Edge is not installed in the default Windows location, set `EDGE_EXE` to the
`msedge.exe` path.
