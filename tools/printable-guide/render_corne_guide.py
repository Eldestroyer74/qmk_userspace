#!/usr/bin/env python3
"""ChieftainDots Corne keymap poster â€” generates SVG + PNG reference sheet."""

import os as _os
import subprocess as _subprocess
import tempfile as _tempfile
from fontTools.ttLib import TTFont
from fontTools.pens.svgPathPen import SVGPathPen
from fontTools.pens.boundsPen import BoundsPen
from fontTools.varLib.mutator import instantiateVariableFont

_HERE = _os.path.dirname(_os.path.abspath(__file__))
_USER_FONTS = _os.path.join(
    _os.path.expanduser("~"), "AppData", "Local", "Microsoft", "Windows", "Fonts"
)
FONT_PATH  = _os.path.join(_USER_FONTS, "Font Awesome 5 Free-Solid-900.otf")
DOSIS_PATH = _os.path.join(_USER_FONTS, "Dosis-VariableFont_wght.ttf")
_EDGE_EXE = _os.environ.get("EDGE_EXE") or _os.path.join(
    _os.environ.get("ProgramFiles(x86)", r"C:\Program Files (x86)"),
    "Microsoft", "Edge", "Application", "msedge.exe",
)
DOSIS_WEIGHT_TITLE = 800
DOSIS_WEIGHT_KEY = 800   # tap label on each key
DOSIS_WEIGHT_HOLD = 600  # hold + double-tap labels
DOSIS_WEIGHT_STRIP = 800 # symbols strip above BASE/COLEMAK
DOSIS_WEIGHT_WORDMARK = 700  # Dosis Bold for the ChieftainDots h1

# ---------- styles ----------
PAGE_W, PAGE_H = 1680, 1188
PAGE_W_MM, PAGE_H_MM = 420, 297
TITLE_H = 144

BLUE       = "#1e40af"
BLUE_DEEP  = "#1e3a8a"
INK        = "#0f172a"
MUTED      = "#64748b"
BG         = "#f3f5f8"
CARD_FILL  = "#ffffff"
CARD_STROKE = "#d8dee6"
CARD_RX    = 12

DARK_LAYER = {"BASE", "COLEMAK", "SYMBOLS", "NUMBERS", "NAVIGATION",
              "MSSTYLES", "MEDIA", "FUNCTION", "CTRL", "SNAP"}

# ---------- keymap data ----------
# K(tap, hold, double, spanish):
#   tap     â€” large centred glyph
#   hold    â€” top-right, faint
#   double  â€” top-left, faint (tap-dance double tap)
#   spanish â€” bottom-left, faint (Spanish-compose overlay)
def K(tap, hold="", double="", spanish="", dhold=""): return (tap, hold, double, spanish, dhold)

BASE = [
    [K("_KEY_TAB","Esc","Cls"), K("Q","","","Â¡"), K("W","","","","_TEXT_WORK"), K("E","","","Ã‰","_TEXT_EMAIL"), K("R"), K("T"),
     K("Y"), K("U","","","Ãš"), K("I","","","Ã"), K("O","","","Ã“"), K("P","","","","_TEXT_PHONE"), K("_KEY_BSP")],
    [K("_KEY_CAPS"), K("A","Fn","","Ã"), K("S","Sym"), K("D","Num"), K("F","Ctrl"), K("G"),
     K("H","","","","_TEXT_HOME"), K("J","Ctrl"), K("K","Num"), K("L","Sym"), K(";","Fn","","Ãœ"), K("'")],
    [K("_KEY_GLOBE"), K("Z"), K("X"), K("C"), K("V"), K("B"),
     K("N","","","Ã‘","_TEXT_NAME"), K("M","","","","_TEXT_MEET"), K(","), K("."), K("/","\\","|","Â¿"), K("_KEY_GLOBE")],
    [K("_KEY_GUI"), K("_KEY_ALT"), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

COLEMAK = [
    [K("_KEY_TAB","Esc","Cls"), K("Q","","","Â¡"), K("W","","","","_TEXT_WORK"), K("F","","","Ã‰"), K("P","","","","_TEXT_PHONE"), K("G"),
     K("J"), K("L","","","Ãš"), K("U","","","Ã"), K("Y","","","Ã“"), K("'"), K("_KEY_BSP")],
    [K("_KEY_CAPS"), K("A","Fn","","Ã"), K("R","Sym"), K("S","Num"), K("T","Ctrl"), K("D"),
     K("H","","","","_TEXT_HOME"), K("N","Ctrl","","","_TEXT_NAME"), K("E","Num","","","_TEXT_EMAIL"), K("I","Sym"), K("O","Fn","","Ãœ"), K(";")],
    [K("_KEY_GLOBE"), K("Z"), K("X"), K("C"), K("V"), K("B"),
     K("K","","","Ã‘"), K("M","","","","_TEXT_MEET"), K(","), K("."), K("/","\\","|","Â¿"), K("_KEY_GLOBE")],
    [K("_KEY_GUI"), K("_KEY_ALT"), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

# SYMBOLS â€” shifted-number top row + right-hand shifted numpad echoes.
# Row 0 col 0 is now blank (no symbol above Tab) so the strip aligns with the
# new layout. Row 1 right side mirrors the Numbers pad with `$ % ^ +`.
# Row 2 right side gives `! @ # > ?` for one-handed shifted reach.
SYMBOLS = [
    [K("~"), K("!"), K("@"), K("#"), K("$"), K("%"),
     K("^"), K("&"), K("*"), K("(","[","{"), K(")","]","}"), K("_")],
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K("$"), K("%"), K("^"), K("+"), K("")],
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K("!"), K("@"), K("#"), K(""), K("")],
    [K(""), K("_KEY_ALT"), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

# NUMBERS â€” full top-row digits, right-hand numpad below.
# Row 1 col 4 holds an EMBEDDED CHORD: holding the F-position while S or L is
# already held reaches the Symbols layer. Rendered as a blue highlighted key
# with the SYM icon so the user reads it as "chord, not a typeable key".
NUMBERS = [
    [K("`"), K("1"), K("2"), K("3"), K("4"), K("5"),
     K("6"), K("7"), K("8"), K("9"), K("0"), K("-")],
    [K(""), K(""), K(""), K(""), K("Sym"), K(""),
     K(""), K("4"), K("5"), K("6"), K("="), K("")],
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K("1"), K("2", ",", "<"), K("3", ".", ">"), K("/"), K("")],
    [K(""), K("_KEY_ALT"), K("_KEY_SPACE","Sym"),
     K("_KEY_ENTER","Sym"), K("_KEY_ALT"), K("_KEY_MENU")],
]

# NAV folds the old Extremes + Snap sub-layers into faint annotations on the
# 4-tier tap-dance on each arrow key:
#   tap (centre)      = move 1 char      arrow-*
#   hold (top-right)  = autorepeat       arrow-* (same icon, corner = held)
#   double (top-left) = jump 1 word      fast-forward rotated (_WORD_*)
#   dhold (bot-left)  = go to extreme    step-forward rotated (_EXTREME_*)
NAV = [
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K(""), K("_ARROW_UP","_ARROW_UP","_WORD_UP","","_EXTREME_UP"), K(""), K(""), K("_KEY_DEL")],
    [K(""), K(""), K(""), K(""), K("Ctrl"), K(""),
     K(""), K("_ARROW_LEFT","_ARROW_LEFT","_WORD_LEFT","","_EXTREME_LEFT"),
     K("_ARROW_DOWN","_ARROW_DOWN","_WORD_DOWN","","_EXTREME_DOWN"),
     K("_ARROW_RIGHT","_ARROW_RIGHT","_WORD_RIGHT","","_EXTREME_RIGHT"), K(""), K("")],
    [K("")]*12,
    [K("â–½"), K("_KEY_ALT"), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

# MSSTYLES â€” Alt+Shift+arrows (PowerPoint paragraph/list style movement).
# Reached by holding S/L, then left GUI.
MSSTYLES = [
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K(""), K("_REORDER_UP"), K(""), K(""), K("_KEY_DEL")],
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K("_OUTDENT"), K("_REORDER_DN"), K("_INDENT"), K(""), K("")],
    [K("")]*12,
    [K(""), K("_KEY_ALT"), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

SNAP_ICONS = {"_SNAP_MAX", "_SNAP_MIN", "_SNAP_LEFT", "_SNAP_RIGHT"}
ARROW_ICONS = {"_ARROW_UP", "_ARROW_DOWN", "_ARROW_LEFT", "_ARROW_RIGHT"}
MEDIA_ICONS = {"_MEDIA_PLAY", "_MEDIA_PAUSE", "_MEDIA_PREV", "_MEDIA_NEXT",
               "_MEDIA_MUTE", "_MEDIA_VOLUP", "_MEDIA_VOLDN"}
TEXT_ICONS = {"_TEXT_HOME", "_TEXT_WORK", "_TEXT_PHONE",
              "_TEXT_EMAIL", "_TEXT_MEET", "_TEXT_NAME"}
EXT_ICONS = {"_PG_UP", "_PG_DN", "_HOME_KEY", "_END_KEY"}
# Paragraph promote/demote + reorder (Alt+Shift+arrows in PowerPoint outlines)
STYLE_ICONS = {"_OUTDENT", "_INDENT", "_REORDER_UP", "_REORDER_DN"}
# Universal keyboard-key icons (across BASE/COLEMAK + any layer with Del)
# _KEY_SYM is the embedded-chord glyph used on NUMBERS row 1 col 4.
KEY_ICONS = {"_KEY_TAB", "_KEY_MEDIA", "_KEY_CALC", "_KEY_GUI",
             "_KEY_SPACE", "_KEY_MENU", "_KEY_BSP", "_KEY_DEL",
             "_KEY_ENTER", "_KEY_ALT", "_KEY_SYM", "_KEY_GLOBE",
             "_KEY_SNIP", "_KEY_CUT", "_KEY_COPY", "_KEY_PASTE"}
# Nav 4-tier: word-jump (fast-forward rotated) + extremes (step-forward rotated)
WORD_ICONS    = {"_WORD_LEFT", "_WORD_RIGHT", "_WORD_UP", "_WORD_DOWN"}
EXTREME_ICONS = {"_EXTREME_LEFT", "_EXTREME_RIGHT", "_EXTREME_UP", "_EXTREME_DOWN"}
# Rotation (degrees CW) applied to the base glyph when rendering
# _WORD_* base = fast-forward (â–¶â–¶), _EXTREME_* base = step-forward (â–º|)
ICON_ROTATION = {
    "_WORD_LEFT":     180,
    "_WORD_RIGHT":      0,
    "_WORD_UP":       270,
    "_WORD_DOWN":      90,
    "_EXTREME_LEFT":  180,
    "_EXTREME_RIGHT":   0,
    "_EXTREME_UP":    270,
    "_EXTREME_DOWN":   90,
}
# _KEY_CAPS is drawn with a custom path (FA has no caps-lock glyph)
ICON_TOKENS = (SNAP_ICONS | ARROW_ICONS | MEDIA_ICONS | TEXT_ICONS
               | EXT_ICONS | STYLE_ICONS | KEY_ICONS | WORD_ICONS | EXTREME_ICONS)

# Per-icon vertical-centre tuning + size factor (font glyphs sit on a baseline,
# so we need to nudge for visual centring).  size_factor is multiplied into key_size.
ICON_TUNING = {
    # Text icons keep moderate size, font baseline ~ 0.35
    "_TEXT_HOME":   (0.50, 0.36),
    "_TEXT_WORK":   (0.50, 0.36),
    "_TEXT_PHONE":  (0.50, 0.36),
    "_TEXT_EMAIL":  (0.46, 0.36),
    "_TEXT_MEET":   (0.50, 0.36),
    "_TEXT_NAME":   (0.50, 0.36),
    # Media + arrows can be a touch bolder
    "_MEDIA_PLAY":  (0.50, 0.36),
    "_MEDIA_PAUSE": (0.50, 0.36),
    "_MEDIA_NEXT":  (0.48, 0.36),
    "_MEDIA_PREV":  (0.48, 0.36),
    "_MEDIA_MUTE":  (0.50, 0.36),
    "_MEDIA_VOLUP": (0.50, 0.36),
    "_MEDIA_VOLDN": (0.50, 0.36),
    # Extremes â€” angle-double chevrons are visually thinner, bump size
    "_PG_UP":       (0.58, 0.36),
    "_PG_DN":       (0.58, 0.36),
    "_HOME_KEY":    (0.58, 0.36),
    "_END_KEY":     (0.58, 0.36),
    # Promote / demote â€” indent glyphs read at normal icon size
    "_OUTDENT":     (0.52, 0.36),
    "_INDENT":      (0.52, 0.36),
    # Reorder up / down â€” sort-amount glyphs
    "_REORDER_UP":  (0.52, 0.36),
    "_REORDER_DN":  (0.52, 0.36),
    # Word-jump (fast-forward â–¶â–¶ rotated) â€” slightly larger to read at corner size
    "_WORD_LEFT":   (0.54, 0.36),
    "_WORD_RIGHT":  (0.54, 0.36),
    "_WORD_UP":     (0.54, 0.36),
    "_WORD_DOWN":   (0.54, 0.36),
    # Extremes (step-forward â–º| rotated) â€” same size
    "_EXTREME_LEFT":  (0.54, 0.36),
    "_EXTREME_RIGHT": (0.54, 0.36),
    "_EXTREME_UP":    (0.54, 0.36),
    "_EXTREME_DOWN":  (0.54, 0.36),
    # Universal key icons
    "_KEY_TAB":     (0.62, 0.36),   # long arrow â€” needs width
    "_KEY_MEDIA":   (0.48, 0.36),   # music note
    "_KEY_CALC":    (0.50, 0.36),
    "_KEY_GUI":     (0.46, 0.36),   # grid
    "_KEY_SPACE":   (0.62, 0.36),   # grip-lines â€” wide bar
    "_KEY_MENU":    (0.50, 0.36),   # bars
    "_KEY_BSP":     (0.56, 0.36),   # backspace key
    "_KEY_DEL":     (0.50, 0.36),   # eraser
    "_KEY_ALT":     (0.52, 0.36),   # option/alt escalator
    "_KEY_SYM":     (0.56, 0.36),   # </> â€” wide-ish, give it room
    "_KEY_GLOBE":   (0.56, 0.36),   # globe â€” Spanish-compose anchor
    "_KEY_SNIP":    (0.54, 0.36),   # camera â€” Windows screen snip
    "_KEY_CUT":     (0.54, 0.36),   # scissors
    "_KEY_COPY":    (0.52, 0.36),   # overlapping pages
    "_KEY_PASTE":   (0.50, 0.36),   # clipboard
}

# MEDIA â€” guide-aligned 4-key cluster: VOL+ above the home row, with
# MUTE / VOL- / PLAY across J / K / L. Track prev/next dropped in the
# updated printable guide.
MEDIA = [
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K(""), K("_MEDIA_VOLUP"), K(""), K(""), K("_KEY_DEL")],
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K("_MEDIA_MUTE"), K("_MEDIA_VOLDN"), K("_MEDIA_PLAY"), K(""), K("")],
    [K("")]*12,
    [K("â–½"), K("_KEY_ALT"), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

TEXT = [
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K("_TEXT_HOME"), K(""), K("_TEXT_PHONE"), K(""), K(""), K("_KEY_DEL")],
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K("_TEXT_WORK"), K("_TEXT_MEET"), K("_TEXT_EMAIL"), K("_TEXT_NAME"), K(""), K("")],
    [K("")]*12,
    [K(""), K("â–½"), K(""), K(""), K(""), K("")],
]

FUNCTION = [
    [K("_KEY_TAB"), K("F1"), K("F2"), K("F3"), K("F4"), K("F5"),
     K("F6"), K("F7"), K("F8"), K("F9"), K("F10"), K("_KEY_DEL")],
    [K(""), K(""), K("_KEY_SNIP"), K(""), K(""), K(""),
     K(""), K("F4"), K("F5"), K("F6"), K("F11"), K("Cole","Tog")],
    [K(""), K(""), K("_KEY_CUT"), K("_KEY_COPY"), K("_KEY_PASTE"), K(""),
     K(""), K("F1"), K("F2"), K("F3"), K("F12"), K("")],
    [K(""), K("_KEY_ALT"), K("_KEY_ENTER","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

CTRL = [
    [K("")]*12,
    [K("")]*12,
    [K("")]*12,
    [K(""), K(""), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

SNAP = [
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K(""), K("_SNAP_MAX"), K(""), K(""), K("_KEY_DEL")],
    [K(""), K(""), K(""), K(""), K(""), K(""),
     K(""), K("_SNAP_LEFT"), K("_SNAP_MIN"), K("_SNAP_RIGHT"), K(""), K("")],
    [K("")]*12,
    [K("â–½"), K("_KEY_ALT"), K("_KEY_SPACE","Sft"), K("_KEY_ENTER","Sft"), K("_KEY_ALT"), K("_KEY_MENU")],
]

# ACCESS lists every key that gets highlighted on a layer card. Most entries
# are the chord members used to REACH that layer; NUMBERS additionally lists
# (1, 4) as an embedded chord that reaches Symbols FROM Numbers. The embedded
# chord is filtered out of chord_parts() so it doesn't appear in the chord
# header â€” it shows up as a blue key with the SYM icon on the keymap itself.
ACCESS = {
    "BASE": [],
    "COLEMAK": [],
    "FUNCTION":   [(1, 1), (1, 10)],            # A or ;
    "MEDIA":      [(1, 1), (1, 10), (3, 0)],    # (A or ;) + GUI
    "SYMBOLS":    [(1, 2), (1, 9)],             # S or L
    "MSSTYLES":   [(1, 2), (1, 9), (3, 0)],     # (S or L) + GUI
    "NUMBERS":    [(1, 3), (1, 8)],             # D or K
    "NAVIGATION": [(1, 3), (1, 8), (3, 0)],     # (D or K) + GUI
    "CTRL":       [(1, 4), (1, 7)],             # F or J
    "SNAP":       [(1, 4), (1, 7), (3, 0)],     # (F or J) + GUI
}

# Faint sky-blue cue: render as an unlabelled coloured tile every anchor that
# could reach a sub-layer but isn't the one in use on this card. Lets the eye
# locate the active anchor in relation to its siblings. Built per layer from
# ANCHOR_POSITIONS minus the active access set.
ANCHOR_POSITIONS = [
    (1, 1),   # A
    (1, 2),   # S
    (3, 0),   # GUI thumb
    (3, 1),   # Alt thumb
]

# EMBED: positions on a layer that render in the sky-blue cue palette while
# still showing their content (icons or text). Two use cases on NUMBERS:
#   - (1, 4) is the SYM portal â€” a chord that takes you to Symbols. It is
#     not the chord that REACHES Numbers, so it should not look "active"
#     (highlight blue). Sky-blue + the literal text "Sym" signals "this is
#     a hop, not the focus".
#   - (3, 3..5) are carry-over thumb keys that fall through to BASE behavior
#     (Enter / Alt / Menu). Drawing them sky-blue with dark-blue icons keeps
#     them readable while letting the digit row carry the visual weight.
EMBED = {
    # Only the SYM portal stays as an "embed" â€” the left-thumb cluster is now
    # handled as anchor cues so each thumb takes on the destination layer's
    # pale hue plus its base icon (GUI/Alt/Space).
    "NUMBERS": {(1, 4)},
}

# PORTAL_DEST: per-layer position â†’ destination-layer. Renders that key in the
# destination layer's pale palette so the colour bridges the two layers
# visually (e.g. "Sym" on NUMBERS draws in pale-amber to flag a hop into
# SYMBOLS). Positions not listed fall back to the current layer's palette.
PORTAL_DEST = {
    "NUMBERS": {(1, 4): "SYMBOLS"},
}

# LAYER_PALETTE: per-layer chord + anchor-cue palette. Mirrors the RGB scheme
# defined in chieftainDots-rgb-palette-proposal.md.
#   chord       â€” fill on a highlighted chord member
#   chord_text  â€” label/icon colour on a chord member
#   chord_hold  â€” pale-hue secondary label colour ON a chord member
#   cue         â€” fill on an anchor-cue / embed / carry-over tile
#   cue_stroke  â€” stroke on cue / embed tiles
#   cue_text    â€” label/icon colour on a cue / embed tile
DEFAULT_PALETTE = {
    "chord":      "#3b82f6",
    "chord_text": "#ffffff",
    "chord_hold": "#bfdbfe",
    "cue":        "#bfdbfe",
    "cue_stroke": "#93c5fd",
    "cue_text":   "#1e40af",
}

LAYER_PALETTE = {
    # Hybrid print-safe â€” hue family matches firmware LEDs, darker shades for white-on-chord legibility
    "SYMBOLS":    {"chord": "#92400e", "chord_text": "#ffffff", "chord_hold": "#fde68a",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "MEDIA":      {"chord": "#134e4a", "chord_text": "#ffffff", "chord_hold": "#ccfbf1",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "TEXT":       {"chord": "#9f1239", "chord_text": "#ffffff", "chord_hold": "#fecdd3",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "FUNCTION":   {"chord": "#4d7c0f", "chord_text": "#ffffff", "chord_hold": "#d9f99d",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "NUMBERS":    {"chord": "#1e40af", "chord_text": "#ffffff", "chord_hold": "#bfdbfe",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "NAVIGATION": {"chord": "#0e7490", "chord_text": "#ffffff", "chord_hold": "#cffafe",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "MSSTYLES":   {"chord": "#a16207", "chord_text": "#ffffff", "chord_hold": "#fef08a",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "COLEMAK":    {"chord": "#5b21b6", "chord_text": "#ffffff", "chord_hold": "#ddd6fe",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "CTRL":       {"chord": "#334155", "chord_text": "#ffffff", "chord_hold": "#cbd5e1",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
    "SNAP":       {"chord": "#1e3a5f", "chord_text": "#ffffff", "chord_hold": "#bfdbfe",
                   "cue":        "#bfdbfe", "cue_stroke": "#93c5fd", "cue_text":   "#1e40af"},
}

# Payload Del key uses the firmware Caps red (print-safe).
DEL_PAL = {
    "chord":      "#b91c1c",
    "chord_text": "#ffffff",
    "chord_hold": "#fecaca",
    "cue":        "#bfdbfe",
    "cue_stroke": "#93c5fd",
    "cue_text":   "#1e40af",
}

def palette_for(layer):
    return LAYER_PALETTE.get(layer, DEFAULT_PALETTE)

def portal_palette(layer, row, col):
    """If (row, col) is a portal, return the destination layer's palette."""
    dest = PORTAL_DEST.get(layer, {}).get((row, col))
    return LAYER_PALETTE.get(dest) if dest else None

# Anchor identity â€” every anchor seat carries its own palette regardless of
# which sub-layer card it appears on. Home row A/S map to their root layers;
# left thumbs map to the destination they reach from the current parent
# family (warm = A+thumb, cool = S+thumb).
WARM_LAYERS = {"SYMBOLS", "MEDIA", "FUNCTION"}
COOL_LAYERS = {"NUMBERS", "NAVIGATION", "MSSTYLES"}

ANCHOR_IDENTITY = {
    (1, 1): "SYMBOLS",
    (1, 2): "NUMBERS",
}

THUMB_IDENTITY = {
    "warm": {(3, 0): "MEDIA",      (3, 1): "FUNCTION"},
    "cool": {(3, 0): "NAVIGATION", (3, 1): "MSSTYLES"},
}

# Family root cards. On these, every thumb anchor IS a chord-access seat â€”
# pressing GUI / Alt / Space (while still holding A or S) is what advances to
# the next layer. Render them in their destination layer's full chord palette
# + white icon so the launch pads read at a glance. On any other sub-layer
# card the thumbs that aren't part of the active chord stay as silent pale
# tiles (no icon) so the eye lands on the single active chord member.
ROOT_LAYERS = {"NUMBERS", "SYMBOLS"}

def anchor_palette(layer, row, col):
    """Identity palette for an anchor seat, or None if (row, col) isn't an
    anchor or has no identity under the current layer's family."""
    if (row, col) in ANCHOR_IDENTITY:
        return LAYER_PALETTE.get(ANCHOR_IDENTITY[(row, col)])
    if row == 3:
        if layer in WARM_LAYERS:
            family = "warm"
        elif layer in COOL_LAYERS:
            family = "cool"
        else:
            return None
        dest = THUMB_IDENTITY[family].get((row, col))
        if dest:
            return LAYER_PALETTE.get(dest)
    return None

def embed_for(layer):
    return EMBED.get(layer, set())

def secondary_for(layer):
    if layer in ("BASE", "COLEMAK"):
        return set()
    active = set(ACCESS.get(layer, []))
    embed = embed_for(layer)
    return {pos for pos in ANCHOR_POSITIONS if pos not in active and pos not in embed}

ANCHOR_LABEL = {
    (1, 1):  "A",
    (1, 2):  "S",
    (1, 3):  "D",
    (1, 4):  "F",
    (1, 7):  "J",
    (1, 8):  "K",
    (1, 9):  "L",
    (1, 10): ";",
    (3, 0):  "GUI",
    (3, 1):  "Alt",
    (3, 2):  "Spc",
}

# When highlighting an anchor on a sub-layer card, prefer the icon over the text
# label so it matches the BASE/COLEMAK keymap. Alt stays as text (no FA glyph).
ANCHOR_ICON = {
    (3, 0): "_KEY_GUI",
    (3, 1): "_KEY_ALT",
    (3, 2): "_KEY_SPACE",
}

# ---------------------------------------------------------------------------
# Page-2 sub-layer highlight model (the agreed ChieftainDots design).
#
# Every sub-layer card lights EXACTLY TWO keys in the card's OWN layer hue:
#   1. the active home-row anchor (A=(1,1) / S=(1,2) / D=(1,3)), showing its
#      letter, and
#   2. the left GUI thumb (3,0), showing the GUI/windows icon.
# The OTHER two home-row anchors render as PALE CUES in their sibling family's
# hue (parent hue on a parent card, child hue on a child card) with no label.
HOME_ANCHORS = [(1, 1), (1, 2), (1, 3), (1, 4)]   # A / S / D / F
GUI_THUMB = (3, 0)                        # left GUI thumb = data[3][0]

ACTIVE_ANCHOR = {
    "FUNCTION": (1, 1), "MEDIA":      (1, 1),   # A family
    "SYMBOLS":  (1, 2), "MSSTYLES":   (1, 2),   # S family
    "NUMBERS":  (1, 3), "NAVIGATION": (1, 3),   # D family
    "CTRL":     (1, 4), "SNAP":       (1, 4),   # F family
}

# Parent (bare-hold) layers vs child (+GUI) layers.
PARENT_LAYERS = {"FUNCTION", "SYMBOLS", "NUMBERS", "CTRL"}

# Sibling-cue hue per anchor position.
PARENT_HUE = {(1, 1): "FUNCTION", (1, 2): "SYMBOLS",  (1, 3): "NUMBERS", (1, 4): "CTRL"}
CHILD_HUE  = {(1, 1): "MEDIA",    (1, 2): "MSSTYLES", (1, 3): "NAVIGATION", (1, 4): "CTRL"}

def card_highlights(layer):
    """Return (access_set, secondary_set, cue_map) for a sub-layer card.

    access_set  â€” the two saturated keys (active anchor + GUI thumb).
    secondary_set â€” the two sibling-anchor cues (pale).
    cue_map     â€” {(row,col): sibling_layer} so each cue takes its own
                  family hue rather than the board hue.
    """
    # F-family (CTRL / SNAP) â€” anchor at (1,4), cues are A/S/D
    if layer == "CTRL":
        access = {(1, 4), GUI_THUMB}
        cues = [(1, 1), (1, 2), (1, 3)]
        cue_map = {(1, 1): "FUNCTION", (1, 2): "SYMBOLS", (1, 3): "NUMBERS"}
        return access, set(cues), cue_map
    if layer == "SNAP":
        access = {(1, 4), GUI_THUMB}
        cues = [(1, 1), (1, 2), (1, 3)]
        cue_map = {(1, 1): "MEDIA", (1, 2): "MSSTYLES", (1, 3): "NAVIGATION"}
        return access, set(cues), cue_map

    active = ACTIVE_ANCHOR.get(layer)
    if not active:
        return set(), set(), {}
    access = {active, GUI_THUMB}
    cues = [p for p in HOME_ANCHORS if p != active]
    hue = PARENT_HUE if layer in PARENT_LAYERS else CHILD_HUE
    cue_map = {p: hue[p] for p in cues}
    return access, set(cues), cue_map

NOTES = {}

# Per-layer font boost for the main tap label. Defaults to 1.0 (no boost);
# layers whose content is mostly single-glyph operators/punctuation need to
# read at a similar weight to alphanumeric layers, so we bump them.
LAYER_FONT_BOOST = {
    "NUMBERS": 1.35,
}

# ---------- helpers ----------
def esc(s):
    return s.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")

STAGGER = [0.30, 0.30, 0.08, 0.0, 0.08, 0.18]
def col_y_offset(col):
    return STAGGER[col if col < 6 else 11 - col]

_FA_CACHE = {}
_DOSIS_INSTANCES = {}   # weight -> {font, glyphs, cmap, upem, hmtx}
_DOSIS_GLYPH_CACHE = {} # (weight, char) -> {d, adv}

def _load_dosis_at(weight):
    if weight in _DOSIS_INSTANCES:
        return _DOSIS_INSTANCES[weight]
    f = TTFont(DOSIS_PATH)
    inst = instantiateVariableFont(f, {"wght": weight})
    _DOSIS_INSTANCES[weight] = {
        "font":   inst,
        "glyphs": inst.getGlyphSet(),
        "cmap":   inst["cmap"].getBestCmap(),
        "upem":   inst["head"].unitsPerEm,
        "hmtx":   inst["hmtx"],
    }
    return _DOSIS_INSTANCES[weight]

def _dosis_glyph(weight, ch):
    key = (weight, ch)
    if key in _DOSIS_GLYPH_CACHE:
        return _DOSIS_GLYPH_CACHE[key]
    info = _load_dosis_at(weight)
    gname = info["cmap"].get(ord(ch))
    if gname is None:
        _DOSIS_GLYPH_CACHE[key] = None
        return None
    g = info["glyphs"][gname]
    sp = SVGPathPen(info["glyphs"])
    g.draw(sp)
    adv, _ = info["hmtx"][gname]
    _DOSIS_GLYPH_CACHE[key] = {"d": sp.getCommands(), "adv": adv}
    return _DOSIS_GLYPH_CACHE[key]

def dosis_text(x, y, text, font_size, fill, anchor="start",
               letter_spacing=0, weight=DOSIS_WEIGHT_TITLE):
    """Render `text` in Dosis as SVG paths. `y` is the baseline position."""
    info = _load_dosis_at(weight)
    upem = info["upem"]
    scale = font_size / upem
    width = 0
    glyphs = []
    for ch in text:
        ginfo = _dosis_glyph(weight, ch)
        if ginfo is None:
            continue
        glyphs.append(ginfo)
        width += ginfo["adv"] * scale + letter_spacing
    if glyphs:
        width -= letter_spacing
    if anchor == "middle":
        x0 = x - width / 2
    elif anchor == "end":
        x0 = x - width
    else:
        x0 = x
    parts = [f'<g fill="{fill}" transform="translate({x0:.2f} {y:.2f})">']
    cur = 0.0
    for ginfo in glyphs:
        parts.append(
            f'<g transform="translate({cur:.2f} 0) scale({scale:.4f} {-scale:.4f})">'
            f'<path d="{ginfo["d"]}"/></g>'
        )
        cur += ginfo["adv"] * scale + letter_spacing
    parts.append('</g>')
    return "".join(parts)

def _load_fa_glyphs():
    """Extract path data + bbox for each FA token from the OTF."""
    if _FA_CACHE:
        return
    codepoints = {
        "_TEXT_HOME":   0xf015,
        "_TEXT_WORK":   0xf0b1,
        "_TEXT_PHONE":  0xf095,
        "_TEXT_EMAIL":  0xf0e0,
        "_TEXT_MEET":   0xf133,
        "_TEXT_NAME":   0xf007,
        "_MEDIA_PLAY":  0xf04b,
        "_MEDIA_PAUSE": 0xf04c,
        "_MEDIA_NEXT":  0xf051,
        "_MEDIA_PREV":  0xf048,
        "_MEDIA_MUTE":  0xf6a9,
        "_MEDIA_VOLUP": 0xf028,
        "_MEDIA_VOLDN": 0xf027,
        "_ARROW_UP":    0xf062,
        "_ARROW_DOWN":  0xf063,
        "_ARROW_LEFT":  0xf060,
        "_ARROW_RIGHT": 0xf061,
        "_PG_UP":       0xf102,  # angle-double-up
        "_PG_DN":       0xf103,  # angle-double-down
        "_HOME_KEY":    0xf100,  # angle-double-left
        "_END_KEY":     0xf101,  # angle-double-right
        "_KEY_TAB":     0xf30b,  # long-arrow-alt-right
        "_KEY_MEDIA":   0xf001,  # music
        "_KEY_CALC":    0xf1ec,  # calculator
        "_KEY_GUI":     0xf009,  # th-large
        "_KEY_SPACE":   0xf7a4,  # grip-lines
        "_KEY_MENU":    0xf0c9,  # bars
        "_KEY_BSP":     0xf55a,  # backspace
        "_KEY_DEL":     0xf00d,  # times (Ã—) â€” plain dark Del key
        "_KEY_SYM":     0xf121,  # code </>
        "_KEY_GLOBE":   0xf7a2,  # globe-europe â€” Spanish-compose anchor
        "_KEY_SNIP":    0xf030,  # camera â€” Windows screen snip
        "_KEY_CUT":     0xf0c4,  # cut / scissors
        "_KEY_COPY":    0xf0c5,  # copy
        "_KEY_PASTE":   0xf0ea,  # paste / clipboard
        "_OUTDENT":     0xf03b,  # outdent â€” promote paragraph (Alt+Shift+Left)
        "_INDENT":      0xf03c,  # indent â€” demote paragraph (Alt+Shift+Right)
        "_REORDER_UP":  0xf161,  # sort-amount-up â€” move paragraph up (Alt+Shift+Up)
        "_REORDER_DN":  0xf160,  # sort-amount-down â€” move paragraph down (Alt+Shift+Down)
        # Nav word-jump: fast-forward (â–¶â–¶) base glyph, rotated per ICON_ROTATION
        "_WORD_LEFT":   0xf051,
        "_WORD_RIGHT":  0xf051,
        "_WORD_UP":     0xf051,
        "_WORD_DOWN":   0xf051,
        # Nav extremes: step-forward (â–º|) base glyph, rotated per ICON_ROTATION
        "_EXTREME_LEFT":  0xf050,
        "_EXTREME_RIGHT": 0xf050,
        "_EXTREME_UP":    0xf050,
        "_EXTREME_DOWN":  0xf050,
    }
    font = TTFont(FONT_PATH)
    cmap = font["cmap"].getBestCmap()
    glyphs = font.getGlyphSet()
    for token, cp in codepoints.items():
        gname = cmap[cp]
        g = glyphs[gname]
        sp = SVGPathPen(glyphs)
        g.draw(sp)
        bp = BoundsPen(glyphs)
        g.draw(bp)
        xmin, ymin, xmax, ymax = bp.bounds
        _FA_CACHE[token] = {
            "d": sp.getCommands(),
            "bbox": (xmin, ymin, xmax, ymax),
            "cx": (xmin + xmax) / 2.0,
            "cy": (ymin + ymax) / 2.0,
            "w":  xmax - xmin,
            "h":  ymax - ymin,
        }

def font_face_defs():
    """Emit each FA glyph as a reusable <symbol id='icon-TOKEN'> in <defs>."""
    _load_fa_glyphs()
    out = ['<defs>']
    for token, info in _FA_CACHE.items():
        xmin, ymin, xmax, ymax = info["bbox"]
        vb_w = info["w"]
        vb_h = info["h"]
        out.append(
            f'<symbol id="icon-{token}" overflow="visible" '
            f'viewBox="{xmin:.2f} {-ymax:.2f} {vb_w:.2f} {vb_h:.2f}">'
            f'<g transform="scale(1,-1)"><path d="{info["d"]}"/></g>'
            f'</symbol>'
        )
    out.append('</defs>')
    return "".join(out)

def draw_fa_glyph(cx, cy, key_size, token, fg):
    """Render an FA glyph centred at (cx, cy) sized to fit key_size * factor."""
    _load_fa_glyphs()
    info = _FA_CACHE.get(token)
    if not info:
        return ""
    size_factor, _ = ICON_TUNING.get(token, (0.50, 0.0))
    target = key_size * size_factor
    rot = ICON_ROTATION.get(token, 0)
    # For 90/270Â° rotations the visual footprint swaps wâ†”h; fit to the longer
    # post-rotation dimension so the icon stays within the target square.
    if rot % 180 == 90:
        longest = max(info["w"], info["h"])
    else:
        longest = max(info["w"], info["h"])
    s = target / longest
    use_w = info["w"] * s
    use_h = info["h"] * s
    x = cx - use_w / 2
    y = cy - use_h / 2
    use_el = (
        f'<use href="#icon-{token}" x="{x:.2f}" y="{y:.2f}" '
        f'width="{use_w:.2f}" height="{use_h:.2f}" fill="{fg}"/>'
    )
    if rot:
        return f'<g transform="rotate({rot} {cx:.2f} {cy:.2f})">{use_el}</g>'
    return use_el

def draw_fa_corner(cx, cy, box, token, fg):
    """Render a small FA glyph fit to a `box`-sized square centred at (cx, cy).

    Used for the faint double-tap snippet icons that sit in a key's top-left
    corner (e.g. the Base-layer string macros: home / work / phone / etc.).
    Ignores ICON_TUNING so every corner glyph fits the same target box.
    """
    _load_fa_glyphs()
    info = _FA_CACHE.get(token)
    if not info:
        return ""
    longest = max(info["w"], info["h"])
    s = box / longest
    use_w = info["w"] * s
    use_h = info["h"] * s
    x = cx - use_w / 2
    y = cy - use_h / 2
    rot = ICON_ROTATION.get(token, 0)
    use_el = (
        f'<use href="#icon-{token}" x="{x:.2f}" y="{y:.2f}" '
        f'width="{use_w:.2f}" height="{use_h:.2f}" fill="{fg}"/>'
    )
    if rot:
        return f'<g transform="rotate({rot} {cx:.2f} {cy:.2f})">{use_el}</g>'
    return use_el

def draw_fa_pair(cx, cy, key_size, token_a, token_b, fg, gap_factor=0.10):
    """Render two FA glyphs side by side, centred at (cx, cy)."""
    _load_fa_glyphs()
    info_a = _FA_CACHE.get(token_a)
    info_b = _FA_CACHE.get(token_b)
    if not info_a or not info_b:
        return ""
    half_target = key_size * 0.34
    def fit(info, target):
        longest = max(info["w"], info["h"])
        s = target / longest
        return info["w"] * s, info["h"] * s
    wa, ha = fit(info_a, half_target)
    wb, hb = fit(info_b, half_target)
    gap = key_size * gap_factor
    total_w = wa + gap + wb
    x0 = cx - total_w / 2
    xa = x0
    xb = x0 + wa + gap
    ya = cy - ha / 2
    yb = cy - hb / 2
    return (
        f'<use href="#icon-{token_a}" x="{xa:.2f}" y="{ya:.2f}" '
        f'width="{wa:.2f}" height="{ha:.2f}" fill="{fg}"/>'
        f'<use href="#icon-{token_b}" x="{xb:.2f}" y="{yb:.2f}" '
        f'width="{wb:.2f}" height="{hb:.2f}" fill="{fg}"/>'
    )

def draw_caps_icon(cx, cy, key_size, fg):
    aw = key_size * 0.40
    ah = key_size * 0.20
    sw = key_size * 0.14
    sh = key_size * 0.16
    bw = key_size * 0.46
    bh = key_size * 0.06
    bar_gap = key_size * 0.06

    total_h = ah + sh + bar_gap + bh
    top = cy - total_h / 2

    head_top = top
    stem_top = head_top + ah
    stem_bot = stem_top + sh
    bar_y = stem_bot + bar_gap

    path_d = (
        f"M {cx:.2f},{head_top:.2f} "
        f"L {cx - aw/2:.2f},{stem_top:.2f} "
        f"L {cx - sw/2:.2f},{stem_top:.2f} "
        f"L {cx - sw/2:.2f},{stem_bot:.2f} "
        f"L {cx + sw/2:.2f},{stem_bot:.2f} "
        f"L {cx + sw/2:.2f},{stem_top:.2f} "
        f"L {cx + aw/2:.2f},{stem_top:.2f} Z"
    )
    return (
        f'<path d="{path_d}" fill="{fg}" stroke="{fg}" stroke-width="1.2" '
        f'stroke-linejoin="round" stroke-linecap="round"/>'
        f'<rect x="{cx - bw/2:.2f}" y="{bar_y:.2f}" '
        f'width="{bw:.2f}" height="{bh:.2f}" rx="0.6" fill="{fg}"/>'
    )

def draw_enter_icon(cx, cy, key_size, fg):
    W = key_size * 0.52
    H = key_size * 0.42 * 0.7
    sw = max(2.0, key_size * 0.085)
    head_w = key_size * 0.16
    head_h = key_size * 0.20

    left = cx - W / 2
    right = cx + W / 2
    top = cy - H / 2
    bot = cy + H / 2

    arrow_base = left + head_w

    stem_d = (
        f"M {right:.2f},{top:.2f} "
        f"L {right:.2f},{bot:.2f} "
        f"L {arrow_base:.2f},{bot:.2f}"
    )
    head_d = (
        f"M {left:.2f},{bot:.2f} "
        f"L {arrow_base:.2f},{bot - head_h / 2:.2f} "
        f"L {arrow_base:.2f},{bot + head_h / 2:.2f} Z"
    )
    return (
        f'<path d="{stem_d}" fill="none" stroke="{fg}" '
        f'stroke-width="{sw:.2f}" stroke-linecap="round" '
        f'stroke-linejoin="round"/>'
        f'<path d="{head_d}" fill="{fg}" stroke="{fg}" stroke-width="1.2" '
        f'stroke-linejoin="round" stroke-linecap="round"/>'
    )

def draw_tab_icon(cx, cy, key_size, fg):
    total_w = key_size * 0.58
    head_h = key_size * 0.26
    head_w = key_size * 0.16
    shaft_h = key_size * 0.10
    wall_w = max(2.2, key_size * 0.08)
    wall_h = key_size * 0.40

    x_left = cx - total_w / 2
    x_right = cx + total_w / 2
    wall_x = x_right - wall_w
    head_tip_x = wall_x - 1.0
    head_base_x = head_tip_x - head_w
    shaft_left = x_left
    shaft_right = head_base_x

    head_top = cy - head_h / 2
    head_bot = cy + head_h / 2
    shaft_top = cy - shaft_h / 2
    shaft_bot = cy + shaft_h / 2

    arrow_d = (
        f"M {shaft_left:.2f},{shaft_top:.2f} "
        f"L {shaft_right:.2f},{shaft_top:.2f} "
        f"L {shaft_right:.2f},{head_top:.2f} "
        f"L {head_tip_x:.2f},{cy:.2f} "
        f"L {shaft_right:.2f},{head_bot:.2f} "
        f"L {shaft_right:.2f},{shaft_bot:.2f} "
        f"L {shaft_left:.2f},{shaft_bot:.2f} Z"
    )
    return (
        f'<path d="{arrow_d}" fill="{fg}" stroke="{fg}" stroke-width="1.2" '
        f'stroke-linejoin="round" stroke-linecap="round"/>'
        f'<rect x="{wall_x:.2f}" y="{cy - wall_h/2:.2f}" '
        f'width="{wall_w:.2f}" height="{wall_h:.2f}" rx="0.8" fill="{fg}"/>'
    )

def draw_alt_icon(cx, cy, key_size, fg):
    w = key_size * 0.56
    h = key_size * 0.34
    sw = max(1.8, key_size * 0.085)

    x_left = cx - w / 2
    x_right = cx + w / 2
    y_top = cy - h / 2
    y_bot = cy + h / 2

    seg = w / 3.0
    top_stub_right = x_left + seg
    bot_stub_left = x_right - seg

    detached_left = bot_stub_left
    detached_right = x_right

    parts = [
        f'<path d="M {x_left:.2f},{y_top:.2f} L {top_stub_right:.2f},{y_top:.2f}" '
        f'fill="none" stroke="{fg}" stroke-width="{sw:.2f}" '
        f'stroke-linecap="round"/>',
        f'<path d="M {top_stub_right:.2f},{y_top:.2f} L {bot_stub_left:.2f},{y_bot:.2f}" '
        f'fill="none" stroke="{fg}" stroke-width="{sw:.2f}" '
        f'stroke-linecap="round"/>',
        f'<path d="M {bot_stub_left:.2f},{y_bot:.2f} L {x_right:.2f},{y_bot:.2f}" '
        f'fill="none" stroke="{fg}" stroke-width="{sw:.2f}" '
        f'stroke-linecap="round"/>',
        f'<path d="M {detached_left:.2f},{y_top:.2f} L {detached_right:.2f},{y_top:.2f}" '
        f'fill="none" stroke="{fg}" stroke-width="{sw:.2f}" '
        f'stroke-linecap="round"/>',
    ]
    return "".join(parts)

def draw_delete_icon(cx, cy, key_size, fg):
    half = key_size * 0.18
    sw = max(2.0, key_size * 0.09)
    return (
        f'<path d="M {cx - half:.2f},{cy - half:.2f} L {cx + half:.2f},{cy + half:.2f} '
        f'M {cx + half:.2f},{cy - half:.2f} L {cx - half:.2f},{cy + half:.2f}" '
        f'fill="none" stroke="{fg}" stroke-width="{sw:.2f}" stroke-linecap="round"/>'
    )

def draw_space_icon(cx, cy, key_size, fg):
    w = key_size * 0.58
    depth = key_size * 0.16
    sw = max(2.0, key_size * 0.09)
    x0 = cx - w / 2
    x1 = cx + w / 2
    y_top = cy - depth / 2
    y_bot = cy + depth / 2
    path_d = (
        f"M {x0:.2f},{y_top:.2f} "
        f"L {x0:.2f},{y_bot:.2f} "
        f"L {x1:.2f},{y_bot:.2f} "
        f"L {x1:.2f},{y_top:.2f}"
    )
    return (
        f'<path d="{path_d}" fill="none" stroke="{fg}" '
        f'stroke-width="{sw:.2f}" stroke-linecap="round" stroke-linejoin="round"/>'
    )

def draw_snap_icon(cx, cy, key_size, token, fg):
    sw = key_size * 0.62
    sh = sw * 0.72
    sx = cx - sw / 2
    sy = cy - sh / 2
    pad = max(1.5, sw * 0.10)
    iw = sw - pad * 2
    ih = sh - pad * 2
    parts = [
        f'<rect x="{sx:.2f}" y="{sy:.2f}" width="{sw:.2f}" height="{sh:.2f}" '
        f'rx="1.5" ry="1.5" fill="none" stroke="{fg}" stroke-width="1.2" '
        f'stroke-linejoin="round"/>'
    ]
    if token == "_SNAP_MAX":
        wh_d = max(1.4, ih * 0.48)
        wx, wy, ww, wh = sx + pad, sy + pad, iw, wh_d
    elif token == "_SNAP_MIN":
        wh_d = max(1.4, ih * 0.48)
        wx, wy, ww, wh = sx + pad, sy + sh - pad - wh_d, iw, wh_d
    elif token == "_SNAP_LEFT":
        ww_d = iw * 0.48
        wx, wy, ww, wh = sx + pad, sy + pad, ww_d, ih
    elif token == "_SNAP_RIGHT":
        ww_d = iw * 0.48
        wx, wy, ww, wh = sx + sw - pad - ww_d, sy + pad, ww_d, ih
    else:
        return ""
    parts.append(
        f'<rect x="{wx:.2f}" y="{wy:.2f}" width="{ww:.2f}" height="{wh:.2f}" '
        f'rx="0.6" fill="{fg}"/>'
    )
    return "\n".join(parts)

# SHIFT_MAP â€” glyph produced with Shift, shown top-centre on Base/Colemak so
# the shifted character is discoverable on the typing layers.
SHIFT_MAP = {",": "<", ".": ">", "/": "?", ";": ":", "'": '"'}

def draw_key(x, y, w, h, tap, hold, double, dark, highlight, anchor_label,
             font_main, font_hold, secondary=False, secondary_label=None,
             embed=False, font_boost=1.0, palette=None,
             embed_palette=None, payload=False, payload_palette=None,
             spanish="", double_hold="", shift="", muted=False):
    blank = (tap == "" and not highlight and not secondary and not embed)
    trans = (tap == "â–½")

    pal = palette or DEFAULT_PALETTE
    emb_pal = embed_palette or pal
    pay_pal = payload_palette or pal

    if highlight:
        fill = pal["chord"]
        stroke = pal["chord"]
        text = pal["chord_text"]
        hold_color = pal["chord_hold"]
    elif payload:
        fill = pay_pal["chord"]
        stroke = pay_pal["chord"]
        text = pay_pal["chord_text"]
        hold_color = pay_pal["chord_hold"]
    elif embed:
        fill = emb_pal["cue"]
        stroke = emb_pal["cue_stroke"]
        text = emb_pal["cue_text"]
        hold_color = emb_pal["cue_text"]
    elif secondary:
        fill = pal["cue"]
        stroke = pal["cue_stroke"]
        text = pal["cue_text"]
        hold_color = pal["cue_text"]
    elif muted:
        fill = "#eaeef3"
        stroke = "#cbd5e1"
        text = MUTED
        hold_color = MUTED
    elif dark:
        if blank or trans:
            fill = "#eaeef3"
            stroke = "#cbd5e1"
            text = MUTED
            hold_color = MUTED
        else:
            fill = "#0b1220"
            stroke = "#1f2937"
            text = "#f1f5f9"
            hold_color = "#94a3b8"
    else:
        if blank or trans:
            fill = "#eaeef3"
            stroke = "#cbd5e1"
            text = MUTED
            hold_color = MUTED
        else:
            fill = "#ffffff"
            stroke = "#c7ced8"
            text = INK
            hold_color = MUTED

    rx = 6
    out = [
        f'<rect x="{x:.1f}" y="{y:.1f}" width="{w}" height="{h}" '
        f'rx="{rx}" ry="{rx}" fill="{fill}" stroke="{stroke}" stroke-width="1"/>'
    ]
    display = tap
    if highlight and anchor_label:
        display = anchor_label
    if blank and not highlight:
        display = ""
    if secondary:
        display = ""

    is_icon = False
    if display in SNAP_ICONS:
        out.append(draw_snap_icon(x + w/2, y + h/2, w, display, text))
        is_icon = True
    elif display == "_KEY_CAPS":
        out.append(draw_caps_icon(x + w/2, y + h/2, w, text))
        is_icon = True
    elif display == "_KEY_TAB":
        out.append(draw_tab_icon(x + w/2, y + h/2, w, text))
        is_icon = True
    elif display == "_KEY_ENTER":
        out.append(draw_enter_icon(x + w/2, y + h/2, w, text))
        is_icon = True
    elif display == "_KEY_SPACE":
        out.append(draw_space_icon(x + w/2, y + h/2, w, text))
        is_icon = True
    elif display == "_KEY_DEL":
        out.append(draw_delete_icon(x + w/2, y + h/2, w, text))
        is_icon = True
    elif display == "_KEY_ALT":
        out.append(draw_alt_icon(x + w/2, y + h/2, w, text))
        is_icon = True
    elif display == "_MEDIA_PLAY":
        out.append(draw_fa_pair(x + w/2, y + h/2, w, "_MEDIA_PLAY",
                                "_MEDIA_PAUSE", text))
        is_icon = True
    elif display in ICON_TOKENS:
        out.append(draw_fa_glyph(x + w/2, y + h/2, w, display, text))
        is_icon = True

    if display and not is_icon:
        fs = font_main
        text_weight = DOSIS_WEIGHT_KEY
        if highlight or secondary:
            fs = int(round(font_main * 1.45))
        elif embed:
            # Portals (e.g. "Sym"): regular weight at a slightly smaller size
            # so the label reads as a wayfinding cue, not a primary key glyph.
            fs = max(10, int(round(font_main * 0.85)))
            text_weight = DOSIS_WEIGHT_HOLD
        else:
            fs = int(round(fs * font_boost))
        if len(display) >= 4: fs = max(10, fs - 1)
        if len(display) >= 6: fs = max(9, fs - 3)
        cy = y + h/2 + fs*0.34
        if hold and not blank and not trans:
            cy = y + h/2 + fs*0.45
        out.append(
            dosis_text(x + w/2, cy, display, fs, text,
                       anchor="middle", weight=text_weight)
        )
    # When a key carries BOTH a hold (top-right) and a double-tap (top-left)
    # label, the two share the top edge. On narrow keys (e.g. the 38u arrow
    # keys with 4-char "PgUp" + "G+Up") that collides, so shrink the faint
    # font just enough for both to sit side by side with a centre gap.
    # Clockwise corner convention (from top-right):
    #   top-right    = hold
    #   top-left     = double-tap (text or FA icon)
    #   bottom-left  = double-tap-hold
    #   bottom-right = Spanish-compose overlay
    faint_fs = font_hold
    if hold and double and double not in ICON_TOKENS:
        # When hold (top-right) and a TEXT double (top-left) share the top edge,
        # shrink both just enough to leave a ~4px centre gap. Icon doubles live
        # in the corner box and don't compete for the text row, so skip them.
        longest = max(len(hold), len(double))
        fit = (w - 12) / (1.2 * max(1, longest))
        faint_fs = max(5, int(min(font_hold, fit)))
    if hold and not blank and not trans and not highlight and not secondary:
        if hold in ICON_TOKENS:
            box = w * 0.26
            out.append(draw_fa_corner(x + w - 5 - box/2, y + 5 + box/2,
                                      box, hold, hold_color))
        else:
            out.append(
                dosis_text(x + w - 4, y + faint_fs + 2, hold, faint_fs,
                           hold_color, anchor="end", weight=DOSIS_WEIGHT_HOLD)
            )
    if double and not blank and not trans and not highlight and not secondary:
        if double in ICON_TOKENS:
            box = w * 0.26
            out.append(draw_fa_corner(x + 5 + box/2, y + 5 + box/2,
                                      box, double, hold_color))
        else:
            out.append(
                dosis_text(x + 4, y + faint_fs + 2, double, faint_fs,
                           hold_color, anchor="start", weight=DOSIS_WEIGHT_HOLD)
            )
    if double_hold and not blank and not trans and not highlight and not secondary:
        if double_hold in SNAP_ICONS:
            box = w * 0.34
            out.append(draw_snap_icon(x + 5 + box/2, y + h - 5 - box/2,
                                      box / 0.62, double_hold, hold_color))
        elif double_hold in ICON_TOKENS:
            box = w * 0.26
            out.append(draw_fa_corner(x + 5 + box/2, y + h - 5 - box/2,
                                      box, double_hold, hold_color))
        else:
            out.append(
                dosis_text(x + 4, y + h - 4, double_hold, font_hold,
                           hold_color, anchor="start", weight=DOSIS_WEIGHT_HOLD)
            )
    if spanish and not blank and not trans and not highlight and not secondary:
        out.append(
            dosis_text(x + w - 4, y + h - 4, spanish, font_hold,
                       hold_color, anchor="end", weight=DOSIS_WEIGHT_HOLD)
        )
    if shift and not blank and not trans and not highlight and not secondary:
        out.append(
            dosis_text(x + w/2, y + faint_fs + 2, shift, faint_fs,
                       hold_color, anchor="middle", weight=DOSIS_WEIGHT_HOLD)
        )
    return "\n".join(out)

def draw_sym_strip(kb_x, kb_y, unit, sym_row):
    """Render row 0 of SYMBOLS as small dark labels above each column."""
    gap = max(2, unit * 0.06)
    split_gap = unit * 0.45
    fs = max(11, int(round(unit * 0.28)))
    parts = []
    strip_text = {"_KEY_DEL": "Del"}
    for c in range(12):
        tap = sym_row[c][0]
        if not tap or tap == "â–½":
            continue
        label = strip_text.get(tap, tap)
        # Skip icon tokens (e.g. _KEY_DEL) but let the literal "_" glyph through.
        if label.startswith("_") and len(label) > 1:
            continue
        stagger = col_y_offset(c) * (unit + gap) * 0.5
        x_centre = kb_x + c * (unit + gap) + (split_gap if c >= 6 else 0) + unit / 2
        y_base = kb_y + stagger - 7
        parts.append(
            dosis_text(x_centre, y_base, label, fs, INK,
                       anchor="middle", letter_spacing=0.3,
                       weight=DOSIS_WEIGHT_STRIP)
        )
    return "\n".join(parts)

def draw_keyboard(ox, oy, unit, data, dark, access_set,
                  secondary_set=None, embed_set=None, font_boost=1.0,
                  layer_name=None, cue_map=None):
    secondary_set = secondary_set or set()
    embed_set = embed_set or set()
    cue_map = cue_map or {}
    default_pal = palette_for(layer_name) if layer_name else DEFAULT_PALETTE
    gap = max(2, unit * 0.06)
    split_gap = unit * 0.45
    font_main = max(11, int(round(unit * 0.42)))
    font_hold = max(7, int(round(unit * 0.26)))
    parts = []

    def is_repeated_thumb(r, c, tap, hold, double, spanish, double_hold):
        if layer_name in ("BASE", "COLEMAK") or r != 3:
            return False
        if double or spanish or double_hold:
            return False
        repeated = {
            (3, 1): ("_KEY_ALT", ""),
            (3, 2): ("_KEY_SPACE", "Sft"),
            (3, 3): ("_KEY_ENTER", "Sft"),
            (3, 4): ("_KEY_ALT", ""),
            (3, 5): ("_KEY_MENU", ""),
        }
        return repeated.get((r, c)) == (tap, hold)

    def render(r, c, x, y, tap, hold, double, spanish="", double_hold=""):
        hl = (r, c) in access_set
        sec = (r, c) in secondary_set and not hl and tap != "_KEY_SNIP"
        emb = (r, c) in embed_set and not hl and not sec
        muted = (not hl and not sec and not emb and
                 is_repeated_thumb(r, c, tap, hold, double, spanish, double_hold))

        if hl:
            # Highlighted anchor / GUI thumb: paint in the BOARD's own hue with
            # white legend. Force the anchor letter (A/S/D) or GUI icon even if
            # the seat carries layer content (e.g. NAV maps "Ctrl" to D) â€” the
            # anchor identity wins on these two designated keys.
            key_pal = default_pal
            label = ANCHOR_ICON.get((r, c)) or ANCHOR_LABEL.get((r, c))
        elif sec:
            # Sibling-anchor cue: pale tile in the SIBLING family's hue, no
            # label (parent hue on a parent card, child hue on a child card).
            key_pal = palette_for(cue_map.get((r, c), layer_name))
            label = None
        else:
            key_pal = default_pal
            label = None

        if emb:
            portal_pal = portal_palette(layer_name, r, c) if layer_name else None
            emb_pal = portal_pal or key_pal
        else:
            emb_pal = None

        return draw_key(x, y, unit, unit, tap, hold, double, dark, hl, label,
                        font_main, font_hold,
                        secondary=sec, embed=emb, font_boost=font_boost,
                        palette=key_pal, embed_palette=emb_pal,
                        spanish=spanish, double_hold=double_hold,
                        muted=muted,
                        shift=SHIFT_MAP.get(tap, "") if layer_name in ("BASE", "COLEMAK") else "")

    def _unpack(cell):
        # Cells may be 4-tuple (legacy) or 5-tuple (with double-tap-hold).
        if len(cell) == 5:
            return cell
        if len(cell) == 4:
            return (cell[0], cell[1], cell[2], cell[3], "")
        return (cell[0], cell[1], cell[2], "", "")

    for r in range(3):
        for c in range(12):
            stagger = col_y_offset(c) * (unit + gap) * 0.5
            x = ox + c * (unit + gap) + (split_gap if c >= 6 else 0)
            y = oy + r * (unit + gap) + stagger
            tap, hold, double, spanish, dhold = _unpack(data[r][c])
            parts.append(render(r, c, x, y, tap, hold, double, spanish, dhold))

    thumb_y = oy + 3 * (unit + gap) + unit * 0.35
    for i in range(6):
        col_anchor = (3 + i) if i < 3 else (6 + (i - 3))
        x = ox + col_anchor * (unit + gap) + (split_gap if col_anchor >= 6 else 0)
        tap, hold, double, spanish, dhold = _unpack(data[3][i])
        parts.append(render(3, i, x, thumb_y, tap, hold, double, spanish, dhold))

    kb_w = 12 * (unit + gap) + split_gap - gap
    kb_h = thumb_y - oy + unit
    return "\n".join(parts), kb_w, kb_h

def chord_parts(name):
    """Return (access_keys, anchor_keys) for the chord that reaches this layer.

    Keys in ACCESS that don't correspond to an ANCHOR_LABEL position are
    embedded chords (e.g. SYM on NUMBERS) â€” they describe how to reach OTHER
    layers from inside this one and are filtered out so the chord header only
    shows how to reach the CURRENT layer.
    """
    keys = ACCESS.get(name, [])
    access_keys = []
    anchor_keys = []
    for (r, c) in keys:
        label = ANCHOR_LABEL.get((r, c))
        if not label:
            continue
        if r == 3:
            anchor_keys.append(label)
        else:
            access_keys.append(label)
    return access_keys, anchor_keys

def format_chord(access_keys, anchor_keys):
    """Build the top-right chord string.

    Examples:
        ['A', ';'], []        -> 'A or ;'
        ['S', 'L'], []        -> 'S or L'
        ['S', 'L'], ['GUI']   -> '(S or L) + GUI'
        ['A'], ['GUI']        -> 'A + GUI'
        [],     ['GUI']       -> 'GUI'
    """
    if not access_keys and not anchor_keys:
        return ""
    if access_keys and anchor_keys:
        access_str = " or ".join(access_keys)
        if len(access_keys) > 1:
            access_str = f"({access_str})"
        anchor_str = " + ".join(anchor_keys)
        return f"{access_str} + {anchor_str}"
    if access_keys:
        return " or ".join(access_keys)
    return " + ".join(anchor_keys)

def draw_section_card(x, y, w, h, num, title, sub, layer_name=None):
    out = []
    pal = palette_for(layer_name) if layer_name else DEFAULT_PALETTE
    has_pal = bool(layer_name and layer_name in LAYER_PALETTE)
    badge_fill = pal["chord"] if has_pal else BLUE
    badge_text = pal.get("chord_text", "#ffffff") if has_pal else "#ffffff"
    badge_stroke = pal.get("cue_stroke", badge_fill) if has_pal else badge_fill
    chord_fill = pal.get("cue_text", pal["chord"]) if has_pal else BLUE
    out.append(
        f'<rect x="{x}" y="{y}" width="{w}" height="{h}" '
        f'rx="{CARD_RX}" ry="{CARD_RX}" fill="{CARD_FILL}" '
        f'stroke="{CARD_STROKE}" stroke-width="1"/>'
    )
    out.append(
        f'<rect x="{x+14}" y="{y+14}" width="26" height="26" rx="6" ry="6" '
        f'fill="{badge_fill}" stroke="{badge_stroke}" stroke-width="1"/>'
    )
    out.append(
        f'<text x="{x+27}" y="{y+32}" text-anchor="middle" '
        f'font-family="Inter, Segoe UI, system-ui, sans-serif" '
        f'font-size="14" font-weight="800" fill="{badge_text}">{num}</text>'
    )
    out.append(
        f'<text x="{x+48}" y="{y+33}" '
        f'font-family="Inter, Segoe UI, system-ui, sans-serif" '
        f'font-size="18" font-weight="800" fill="{INK}" letter-spacing="0.6">'
        f'{esc(title)}</text>'
    )
    if layer_name is not None:
        access_keys, anchor_keys = chord_parts(layer_name)
        chord = format_chord(access_keys, anchor_keys)
        if chord:
            out.append(
                f'<text x="{x+w-14}" y="{y+33}" text-anchor="end" '
                f'font-family="Inter, Segoe UI, system-ui, sans-serif" '
                f'font-size="13" font-weight="800" fill="{chord_fill}" '
                f'letter-spacing="0.4">{esc(chord)}</text>'
            )
    return "\n".join(out)

NOTES_HEADER = {
    "SNAP":     "WINDOW SNAP",
    "MEDIA":    "PLAYBACK",
    "TEXT":     "STRING SNIPPETS",
    "FUNCTION": "FUNCTION KEYS",
}

def draw_inline_icon(x_left, y_top, size, token, fg):
    if token in SNAP_ICONS:
        cx = x_left + size / 2
        cy = y_top + size / 2
        return draw_snap_icon(cx, cy, size / 0.62, token, fg)
    _load_fa_glyphs()
    info = _FA_CACHE.get(token)
    if not info:
        return ""
    longest = max(info["w"], info["h"])
    s = size / longest
    use_w = info["w"] * s
    use_h = info["h"] * s
    x = x_left + (size - use_w) / 2
    y = y_top + (size - use_h) / 2
    return (
        f'<use href="#icon-{token}" x="{x:.2f}" y="{y:.2f}" '
        f'width="{use_w:.2f}" height="{use_h:.2f}" fill="{fg}"/>'
    )

def notes_block(x, y, lines, header="TAP / HOLD / DOUBLE-TAP"):
    out = []
    out.append(
        f'<text x="{x}" y="{y}" '
        f'font-family="Inter, Segoe UI, system-ui, sans-serif" '
        f'font-size="9" font-weight="700" fill="{BLUE}" letter-spacing="0.8">'
        f'{esc(header)}</text>'
    )
    line_h = 15
    icon_sz = 13
    for i, ln in enumerate(lines):
        ly = y + 16 + i * line_h
        if isinstance(ln, tuple):
            token, text = ln
            icon_top = ly - icon_sz + 2
            out.append(draw_inline_icon(x, icon_top, icon_sz, token, BLUE))
            out.append(
                f'<text x="{x + icon_sz + 7:.1f}" y="{ly:.1f}" '
                f'font-family="Inter, Segoe UI, system-ui, sans-serif" '
                f'font-size="10.5" fill="{INK}">{esc(text)}</text>'
            )
        else:
            out.append(
                f'<text x="{x}" y="{ly}" '
                f'font-family="Inter, Segoe UI, system-ui, sans-serif" '
                f'font-size="10.5" fill="{INK}">{esc(ln)}</text>'
            )
    return "\n".join(out)

def _kb_width_est(unit):
    return 12 * (unit + max(2, unit * 0.06)) + unit * 0.45

# ---------- build ----------
def build():
    """
    Single landscape page (1680 Ã— 1188):
      Top band  : BASE | COLEMAK  (two alpha cards, full width)
      Row 1     : FUNCTION | MEDIA  Â·  SYMBOLS | MSSTYLES
      Row 2     : NUMBERS | NAVIGATION  Â·  CTRL | SNAP
      Bottom    : KEY ANATOMY (bare, no card background)
    """
    FONTFAM = 'font-family="Inter, Segoe UI, system-ui, sans-serif"'
    parts = [
        font_face_defs(),
        f'<rect x="0" y="0" width="{PAGE_W}" height="{PAGE_H}" fill="{BG}"/>',
    ]

    # â”€â”€ Geometry â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    MARGIN     = 34
    usable_w   = PAGE_W - 2 * MARGIN          # 1612

    TOP_H      = 290                           # top band (Base + Colemak)
    ANAT_H     = 168                           # KEY ANATOMY bottom strip
    BOT_MARGIN = 20                            # bottom whitespace
    GAP_V      = 14                            # vertical gap between bands
    GAP        = 14                            # consistent gap between ALL cards

    # Row heights â€” 3 content rows (2-col, 4-col, 3-col) + anatomy
    # total = PAGE_H - MARGIN - BOT_MARGIN - 4*GAP_V (gaps: before row1, between rows, before anatomy)
    matrix_total_h = PAGE_H - MARGIN - ANAT_H - BOT_MARGIN - 4 * GAP_V
    row_h = matrix_total_h / 3

    # Y positions
    top_y     = MARGIN
    matrix_y  = [top_y + i * (row_h + GAP_V) for i in range(3)]
    anat_y    = matrix_y[2] + row_h + GAP_V

    # Card widths â€” equal within each row, consistent gap between cards
    alpha_card_w = (usable_w - GAP) / 2          # row 1: 2 equal cards
    mid_card_w   = (usable_w - 3 * GAP) / 4      # row 2: 4 equal cards
    bot_card_w   = (usable_w - 2 * GAP) / 3      # row 3: 3 equal cards

    # Key unit sizes scaled to fill each card width with consistent margins
    alpha_unit = 43
    mid_unit   = 27
    bot_unit   = 33

    # â”€â”€ Title kicker â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    parts.append(
        f'<text x="{MARGIN}" y="{MARGIN-2}" {FONTFAM} '
        f'font-size="10" font-weight="700" fill="{BLUE}" letter-spacing="3.0">'
        f'CORNE / CRKBD / KEYMAP REFERENCE</text>'
    )

    # â”€â”€ Top band: BASE + COLEMAK â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    alpha_specs = [
        ("BASE",    "QWERTY",      BASE,    1),
        ("COLEMAK", "alt typing",  COLEMAK, 2),
    ]
    for i, (name, sub, data, num) in enumerate(alpha_specs):
        cx = MARGIN + i * (alpha_card_w + GAP)
        cy = top_y
        parts.append(draw_section_card(cx, cy, alpha_card_w, row_h,
                                       num, name, sub, name))
        kb_w_est = _kb_width_est(alpha_unit)
        kb_x = cx + (alpha_card_w - kb_w_est) / 2
        kb_y = cy + 100
        parts.append(draw_sym_strip(kb_x, kb_y, alpha_unit, SYMBOLS[0]))
        dark = name in DARK_LAYER
        kb, kw, kh = draw_keyboard(kb_x, kb_y, alpha_unit, data, dark,
                                   set(), layer_name=name)
        parts.append(kb)

    # â”€â”€ Matrix rows â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    # Each row: (left pair) hold, +GUI  |divider|  (right pair) hold, +GUI
    # row 2: 4-col (Function/Media/Symbols/Styles), card_w=mid_card_w, unit=mid_unit
    row2_specs = [
        ("FUNCTION",   "FUNCTION",   FUNCTION, 3, "hold"),
        ("MEDIA",      "MEDIA",      MEDIA,    4, "+ GUI"),
        ("SYMBOLS",    "SYMBOLS",    SYMBOLS,  5, "hold"),
        ("MS STYLES",  "MSSTYLES",   MSSTYLES, 6, "+ GUI"),
    ]
    # row 3: 3-col (Numbers/Navigation/Snap), card_w=bot_card_w, unit=bot_unit
    row3_specs = [
        ("NUMBERS",    "NUMBERS",    NUMBERS,  7, "hold"),
        ("NAVIGATION", "NAVIGATION", NAV,      8, "+ GUI"),
        ("SNAP",       "SNAP",       SNAP,     9, "Ctrl + GUI"),
    ]

    def render_row(specs, row_idx, card_w, unit):
        by = matrix_y[row_idx]
        for ci, (title, lk, data, num, role) in enumerate(specs):
            cx = MARGIN + ci * (card_w + GAP)
            cy = by
            parts.append(draw_section_card(cx, cy, card_w, row_h,
                                           num, title, role, lk))
            parts.append(
                f'<text x="{cx+48:.1f}" y="{cy+50:.1f}" {FONTFAM} '
                f'font-size="11" font-weight="600" fill="{MUTED}" '
                f'letter-spacing="0.3">{esc(role)}</text>'
            )
            kb_w_est = _kb_width_est(unit)
            kb_x = cx + (card_w - kb_w_est) / 2
            kb_y = cy + 96
            access_set, secondary_set, cue_map = card_highlights(lk)
            kb, kw, kh = draw_keyboard(kb_x, kb_y, unit, data,
                                       lk in DARK_LAYER, access_set,
                                       secondary_set=secondary_set,
                                       embed_set=embed_for(lk), cue_map=cue_map,
                                       font_boost=LAYER_FONT_BOOST.get(lk, 1.0),
                                       layer_name=lk)
            parts.append(kb)

    render_row(row2_specs, 1, mid_card_w, mid_unit)
    render_row(row3_specs, 2, bot_card_w, bot_unit)

    # â”€â”€ KEY ANATOMY (bare â€” no card background) â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€â”€
    label_color = MUTED
    lg_cx = MARGIN
    lg_cy = anat_y
    parts.append(
        f'<text x="{lg_cx+22}" y="{lg_cy+28}" {FONTFAM} font-size="15" '
        f'font-weight="800" fill="{INK}" letter-spacing="0.6">KEY ANATOMY</text>'
    )
    lg_unit = 56
    lg_y = lg_cy + 50
    lg_font_main = max(11, int(round(lg_unit * 0.42)))
    lg_font_hold = max(7, int(round(lg_unit * 0.26)))
    # Three sample keys spread across lower-centre of anatomy strip
    key_spacing = 180   # horizontal gap between key x-origins
    total_keys_w = 2 * key_spacing + lg_unit
    anat_left = lg_cx + (usable_w - total_keys_w) / 2
    lg_x = [
        anat_left,
        anat_left + key_spacing,
        anat_left + 2 * key_spacing,
    ]
    label_gap = 7
    label_y = lg_y + 14
    bot_label_y = lg_y + lg_unit - 4
    parts.append(draw_key(lg_x[0], lg_y, lg_unit, lg_unit, "A", "Sym", "Wk",
                          True, False, None, lg_font_main, lg_font_hold,
                          spanish="Ã", double_hold="G+W"))
    parts.append(f'<text x="{lg_x[0]+lg_unit+label_gap:.1f}" y="{label_y:.1f}" text-anchor="start" {FONTFAM} font-size="10.5" font-weight="600" fill="{label_color}">hold</text>')
    parts.append(f'<text x="{lg_x[0]-label_gap:.1f}" y="{label_y:.1f}" text-anchor="end" {FONTFAM} font-size="10.5" font-weight="600" fill="{label_color}">double-tap</text>')
    parts.append(f'<text x="{lg_x[0]-label_gap:.1f}" y="{bot_label_y:.1f}" text-anchor="end" {FONTFAM} font-size="10.5" font-weight="600" fill="{label_color}">dbl-hold</text>')
    parts.append(f'<text x="{lg_x[0]+lg_unit+label_gap:.1f}" y="{bot_label_y:.1f}" text-anchor="start" {FONTFAM} font-size="10.5" font-weight="600" fill="{label_color}">Spanish</text>')
    parts.append(draw_key(lg_x[1], lg_y, lg_unit, lg_unit, "", "", "A",
                          True, True, "A", lg_font_main, lg_font_hold))
    parts.append(draw_key(lg_x[2], lg_y, lg_unit, lg_unit, "", "", "",
                          True, False, None, lg_font_main, lg_font_hold,
                          secondary=True))
    caption_y = lg_y + lg_unit + 18
    captions = ["key anatomy", "chord member", "chord cue"]
    for i, text in enumerate(captions):
        parts.append(f'<text x="{lg_x[i]+lg_unit/2:.1f}" y="{caption_y:.1f}" text-anchor="middle" {FONTFAM} font-size="11" font-weight="600" fill="{label_color}" letter-spacing="0.3">{esc(text)}</text>')

    svg = (
        f'<svg xmlns="http://www.w3.org/2000/svg" '
        f'viewBox="0 0 {PAGE_W} {PAGE_H}" '
        f'width="{PAGE_W_MM}mm" height="{PAGE_H_MM}mm">'
        + "\n".join(parts) +
        f'</svg>'
    )
    return svg

def write_png(svg, png_path, output_width):
    """Rasterize the in-memory SVG to PNG using headless Microsoft Edge."""
    png_path = _os.path.abspath(png_path)
    output_height = int(round(output_width * PAGE_H / PAGE_W))
    html = (
        "<!doctype html><html><head><meta charset=\"utf-8\">"
        "<style>html,body{margin:0;width:100%;height:100%;overflow:hidden;}"
        f"svg{{width:{output_width}px;height:{output_height}px;display:block;}}"
        "</style></head><body>"
        f"{svg}"
        "</body></html>"
    )
    with _tempfile.NamedTemporaryFile(
        "w", encoding="utf-8", suffix=".html", delete=False
    ) as tmp:
        tmp.write(html)
        render_path = tmp.name
    try:
        if _os.path.exists(png_path):
            _os.unlink(png_path)
        _subprocess.run(
            [
                _EDGE_EXE,
                "--headless=new",
                "--disable-gpu",
                "--hide-scrollbars",
                f"--window-size={output_width},{output_height}",
                f"--screenshot={png_path}",
                render_path,
            ],
            check=True,
        )
    finally:
        try:
            _os.unlink(render_path)
        except OSError:
            pass

if __name__ == "__main__":
    import argparse
    ap = argparse.ArgumentParser(description="Render ChieftainDots Corne keymap poster")
    ap.add_argument("--out", default=_HERE,
                    help="Output directory (default: same folder as this script)")
    args = ap.parse_args()
    _os.makedirs(args.out, exist_ok=True)
    svg = build()
    png_path = _os.path.join(args.out, "chieftainDots-corne.png")
    write_png(svg, png_path, PAGE_W * 2)
    print(f"Wrote {png_path} from {len(svg):,} bytes of SVG")
