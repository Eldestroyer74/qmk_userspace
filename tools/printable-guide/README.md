# Printable Guide Renderer

This tool generates the printable ChieftainDots Corne layout poster.

Run from the QMK checkout root:

```bash
python users/eldestroyer74/tools/printable-guide/render_corne_guide.py --out users/eldestroyer74/imgs
```

The committed output target is:

```text
users/eldestroyer74/imgs/chieftainDots-corne.png
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
