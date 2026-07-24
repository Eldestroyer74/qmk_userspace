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
