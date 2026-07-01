const fs = require("fs");
const path = require("path");

const root = path.resolve(__dirname, "..");
const fontPath = path.join(root, "oled", "oledfont.c");
const iconsPath = path.join(root, "oled", "oled-icons.c");
const catPath = path.join(root, "oled", "oled-bongocat.c");
const outPath = path.join(root, "docs", "chieftainDots-oled-assets-preview.html");
const auditPath = path.join(root, "docs", "chieftainDots-oled-assets-audit.json");

function read(filePath) {
  return fs.readFileSync(filePath, "utf8");
}

function parseNumbers(body) {
  return Array.from(body.matchAll(/0x[0-9a-fA-F]+|\b\d+\b/g), (match) => Number(match[0]));
}

function extractArray(source, name) {
  const pattern = new RegExp(`${name.replace(/[.*+?^${}()|[\]\\]/g, "\\$&")}\\s*(?:\\[\\])?\\s*(?:PROGMEM\\s*)?=\\s*\\{([\\s\\S]*?)\\};`);
  const match = source.match(pattern);
  if (!match) {
    throw new Error(`Could not find array: ${name}`);
  }
  return parseNumbers(match[1]);
}

function extractNamedArrays(source, typePattern) {
  const arrays = {};
  const pattern = new RegExp(`${typePattern}\\s+(\\w+)\\[\\]\\s+PROGMEM\\s*=\\s*\\{([\\s\\S]*?)\\};`, "g");
  for (const match of source.matchAll(pattern)) {
    arrays[match[1]] = parseNumbers(match[2]);
  }
  return arrays;
}

function glyphRects(font, code, x, y, scale, fill = "#d7f7ff") {
  const offset = code * 6;
  const bytes = font.slice(offset, offset + 6);
  const rects = [];
  for (let col = 0; col < 6; col += 1) {
    const byte = bytes[col] || 0;
    for (let row = 0; row < 8; row += 1) {
      if (byte & (1 << row)) {
        rects.push(`<rect x="${x + col * scale}" y="${y + row * scale}" width="${scale}" height="${scale}" fill="${fill}"/>`);
      }
    }
  }
  return rects.join("");
}

function glyphSequenceSvg(font, codes, options = {}) {
  const scale = options.scale || 4;
  const fill = options.fill || "#d7f7ff";
  const visible = codes.filter((code) => code !== 0);
  const width = visible.length * 6 * scale;
  const height = 8 * scale;
  const rects = visible
    .map((code, index) => glyphRects(font, code, index * 6 * scale, 0, scale, fill))
    .join("");
  return `<svg viewBox="0 0 ${width} ${height}" width="${width}" height="${height}" role="img" aria-label="OLED glyph sequence">${rects}</svg>`;
}

function stringCodes(text) {
  return Array.from(text, (char) => char.charCodeAt(0));
}

function codesFor(nameOrCodes) {
  if (Array.isArray(nameOrCodes)) {
    return nameOrCodes;
  }
  if (iconArrays[nameOrCodes]) {
    return iconArrays[nameOrCodes];
  }
  return stringCodes(nameOrCodes);
}

function writeChars(screen, col, row, codes, cols = 5) {
  let x = col;
  let y = row;
  for (const code of codes.filter((item) => item !== 0)) {
    if (y >= screen.length) {
      break;
    }
    screen[y][x] = code;
    x += 1;
    if (x >= cols) {
      x = 0;
      y += 1;
    }
  }
}

function makeScreen() {
  return Array.from({ length: 16 }, () => Array(5).fill(0x20));
}

function oledScreenSvg(font, rows, options = {}) {
  const scale = options.scale || 5;
  const fill = options.fill || "#d7f7ff";
  const cols = rows[0].length;
  const width = cols * 6 * scale;
  const height = rows.length * 8 * scale;
  const rects = rows.flatMap((row, y) =>
    row.map((code, x) => glyphRects(font, code, x * 6 * scale, y * 8 * scale, scale, fill))
  ).join("");
  return `<svg viewBox="0 0 ${width} ${height}" width="${width}" height="${height}" role="img" aria-label="OLED status screen">${rects}</svg>`;
}

function homePairRows(leftTop, leftBottom, rightTop, rightBottom, leftOn, rightOn) {
  const connectorTop = leftOn && rightOn ? "on_on_1" : leftOn ? "on_off_1" : rightOn ? "off_on_1" : "off_off_1";
  const connectorBottom = leftOn && rightOn ? "on_on_2" : leftOn ? "on_off_2" : rightOn ? "off_on_2" : "off_off_2";
  return [
    [...codesFor(leftTop), ...codesFor(connectorTop), ...codesFor(rightTop)].filter((code) => code !== 0),
    [...codesFor(leftBottom), ...codesFor(connectorBottom), ...codesFor(rightBottom)].filter((code) => code !== 0),
  ];
}

function homeRowStatusRows(states) {
  const concept = {
    ctrl: ["ctrl_off_1", "ctrl_off_2", "ctrl_on_1", "ctrl_on_2"],
    sym: ["sym_off_1", "sym_off_2", "sym_on_1", "sym_on_2"],
    num: ["num_off_1", "num_off_2", "num_on_1", "num_on_2"],
    fn: ["fn_off_1", "fn_off_2", "fn_on_1", "fn_on_2"],
  };
  function parts(name) {
    const [offTop, offBottom, onTop, onBottom] = concept[name];
    return states[name] ? [onTop, onBottom] : [offTop, offBottom];
  }
  const [ctrlTop, ctrlBottom] = parts("ctrl");
  const [symTop, symBottom] = parts("sym");
  const [numTop, numBottom] = parts("num");
  const [fnTop, fnBottom] = parts("fn");
  return [
    ...homePairRows(ctrlTop, ctrlBottom, symTop, symBottom, states.ctrl, states.sym),
    ...homePairRows(numTop, numBottom, fnTop, fnBottom, states.num, states.fn),
  ];
}

function statusScreenRows({ label = "     ", layerArt = "base_layer", states = {} }) {
  const screen = makeScreen();
  writeChars(screen, 0, 0, codesFor("corne_logo"));
  writeChars(screen, 0, 3, codesFor(label.padEnd(5).slice(0, 5)));
  writeChars(screen, 0, 6, codesFor(layerArt));
  const rows = homeRowStatusRows({
    ctrl: false,
    sym: false,
    num: false,
    fn: false,
    ...states,
  });
  rows.forEach((rowCodes, index) => writeChars(screen, 0, 11 + index, rowCodes));
  return screen;
}

function statusScreenPreview(font) {
  const screens = [
    ["Resting", { label: "     ", layerArt: "base_layer" }],
    ["Control held", { label: "     ", layerArt: "anchor_layer", states: { ctrl: true } }],
    ["Symbols held", { label: "     ", layerArt: "anchor_layer", states: { sym: true } }],
    ["Numbers held", { label: "     ", layerArt: "anchor_layer", states: { num: true } }],
    ["Function held", { label: "     ", layerArt: "anchor_layer", states: { fn: true } }],
    ["Media chord", { label: "media", layerArt: "chord_layer", states: { ctrl: true } }],
    ["Styles chord", { label: "style", layerArt: "chord_layer", states: { sym: true } }],
    ["Navigation chord", { label: " nav ", layerArt: "chord_layer", states: { num: true } }],
    ["Snap chord", { label: " snap", layerArt: "chord_layer", states: { fn: true } }],
  ];
  return screens.map(([name, config]) => assetRow(
    name,
    oledScreenSvg(font, statusScreenRows(config), { scale: 5 }),
    "Rendered as the current 5-column rotated OLED status screen: logo, layer tile, and home-row status stack."
  )).join("");
}

const proposedConceptTiles = {
  ctrl: [
    "############",
    "#..........#",
    "#..######..#",
    "#.##....##.#",
    "#.##.......#",
    "#.##.......#",
    "#.##.......#",
    "#.##.......#",
    "#.##.......#",
    "#.##.......#",
    "#.##....##.#",
    "#..######..#",
    "#..........#",
    "#.##.####..#",
    "#..........#",
    "############",
  ],
  sym: [
    "############",
    "#..........#",
    "#..#....#..#",
    "#..#....#..#",
    "#.########.#",
    "#..#....#..#",
    "#..#....#..#",
    "#.########.#",
    "#..#....#..#",
    "#..#....#..#",
    "#..........#",
    "#..##..##..#",
    "#.#..##..#.#",
    "#..##..##..#",
    "#..........#",
    "############",
  ],
  num: [
    "############",
    "#..........#",
    "#..#....#..#",
    "#..#....#..#",
    "#.########.#",
    "#..#....#..#",
    "#..#....#..#",
    "#.########.#",
    "#..#....#..#",
    "#..#....#..#",
    "#..........#",
    "#....##....#",
    "#...###....#",
    "#....##....#",
    "#..........#",
    "############",
  ],
  fn: [
    "############",
    "#..........#",
    "#.########.#",
    "#.##.......#",
    "#.##.......#",
    "#.######...#",
    "#.##.......#",
    "#.##.......#",
    "#..........#",
    "#.##...##..#",
    "#.###..##..#",
    "#.####.##..#",
    "#.##.####..#",
    "#..........#",
    "#..........#",
    "############",
  ],
};

function drawMatrixRects(matrix, originX, originY, scale, fill) {
  const rects = [];
  matrix.forEach((row, y) => {
    Array.from(row).forEach((pixel, x) => {
      if (pixel === "#") {
        rects.push(`<rect x="${originX + x * scale}" y="${originY + y * scale}" width="${scale}" height="${scale}" fill="${fill}"/>`);
      }
    });
  });
  return rects.join("");
}

function invertMatrix(matrix) {
  return matrix.map((row) => Array.from(row).map((pixel) => pixel === "#" ? "." : "#").join(""));
}

function proposedTileSvg(name, on = false, options = {}) {
  const scale = options.scale || 5;
  const matrix = on ? invertMatrix(proposedConceptTiles[name]) : proposedConceptTiles[name];
  const fill = options.fill || "#d7f7ff";
  const width = 12 * scale;
  const height = 16 * scale;
  return `<svg viewBox="0 0 ${width} ${height}" width="${width}" height="${height}" role="img" aria-label="Proposed ${name} tile">${drawMatrixRects(matrix, 0, 0, scale, fill)}</svg>`;
}

function connectorName(leftOn, rightOn, bottom) {
  if (leftOn && rightOn) return bottom ? "on_on_2" : "on_on_1";
  if (leftOn) return bottom ? "on_off_2" : "on_off_1";
  if (rightOn) return bottom ? "off_on_2" : "off_on_1";
  return bottom ? "off_off_2" : "off_off_1";
}

function connectorGlyphRects(font, x, y, scale, leftOn, rightOn, fill) {
  const top = iconArrays[connectorName(leftOn, rightOn, false)].filter((code) => code !== 0)[0];
  const bottom = iconArrays[connectorName(leftOn, rightOn, true)].filter((code) => code !== 0)[0];
  return [
    glyphRects(font, top, x, y, scale, fill),
    glyphRects(font, bottom, x, y + 8 * scale, scale, fill),
  ].join("");
}

function proposedStatusScreenSvg(font, config, options = {}) {
  const scale = options.scale || 5;
  const fill = options.fill || "#d7f7ff";
  const width = 30 * scale;
  const height = 128 * scale;
  const states = { ctrl: false, sym: false, num: false, fn: false, ...(config.states || {}) };
  const screen = statusScreenRows(config);
  const rects = [];

  for (let row = 0; row < 11; row += 1) {
    for (let col = 0; col < 5; col += 1) {
      rects.push(glyphRects(font, screen[row][col], col * 6 * scale, row * 8 * scale, scale, fill));
    }
  }

  rects.push(drawMatrixRects(states.ctrl ? invertMatrix(proposedConceptTiles.ctrl) : proposedConceptTiles.ctrl, 0, 88 * scale, scale, fill));
  rects.push(connectorGlyphRects(font, 12 * scale, 88 * scale, scale, states.ctrl, states.sym, fill));
  rects.push(drawMatrixRects(states.sym ? invertMatrix(proposedConceptTiles.sym) : proposedConceptTiles.sym, 18 * scale, 88 * scale, scale, fill));
  rects.push(drawMatrixRects(states.num ? invertMatrix(proposedConceptTiles.num) : proposedConceptTiles.num, 0, 112 * scale, scale, fill));
  rects.push(connectorGlyphRects(font, 12 * scale, 112 * scale, scale, states.num, states.fn, fill));
  rects.push(drawMatrixRects(states.fn ? invertMatrix(proposedConceptTiles.fn) : proposedConceptTiles.fn, 18 * scale, 112 * scale, scale, fill));

  return `<svg viewBox="0 0 ${width} ${height}" width="${width}" height="${height}" role="img" aria-label="Proposed OLED status screen">${rects.join("")}</svg>`;
}

function proposedConceptPreview(font) {
  const concepts = ["ctrl", "sym", "num", "fn"];
  const screens = [
    ["Resting", { label: "     ", layerArt: "base_layer" }],
    ["Control held", { label: "     ", layerArt: "anchor_layer", states: { ctrl: true } }],
    ["Symbols held", { label: "     ", layerArt: "anchor_layer", states: { sym: true } }],
    ["Numbers held", { label: "     ", layerArt: "anchor_layer", states: { num: true } }],
    ["Function held", { label: "     ", layerArt: "anchor_layer", states: { fn: true } }],
    ["Media chord", { label: "media", layerArt: "chord_layer", states: { ctrl: true } }],
    ["Styles chord", { label: "style", layerArt: "chord_layer", states: { sym: true } }],
    ["Navigation chord", { label: " nav ", layerArt: "chord_layer", states: { num: true } }],
    ["Snap chord", { label: " snap", layerArt: "chord_layer", states: { fn: true } }],
  ];
  return `
    <div class="grid">
      ${concepts.map((name) => compositionRow(
        `${name.toUpperCase()} off/on proposal`,
        `<div class="conceptPair">${proposedTileSvg(name, false, { scale: 5 })}${proposedTileSvg(name, true, { scale: 5 })}</div>`,
        "Preview-only 12x16 concept art. The on state is shown as an inverted tile for scanability."
      )).join("")}
    </div>
    <div class="grid" style="margin-top:14px">
      ${screens.map(([name, config]) => assetRow(
        name,
        proposedStatusScreenSvg(font, config, { scale: 5 }),
        "Preview-only proposed concept tiles in the full OLED status composition."
      )).join("")}
    </div>`;
}

function glyphBytesSvg(bytes, options = {}) {
  const scale = options.scale || 8;
  const fill = options.fill || "#d7f7ff";
  const rects = [];
  for (let col = 0; col < 6; col += 1) {
    const byte = bytes[col] || 0;
    for (let row = 0; row < 8; row += 1) {
      if (byte & (1 << row)) {
        rects.push(`<rect x="${col * scale}" y="${row * scale}" width="${scale}" height="${scale}" fill="${fill}"/>`);
      }
    }
  }
  return `<svg viewBox="0 0 ${6 * scale} ${8 * scale}" width="${6 * scale}" height="${8 * scale}" role="img" aria-label="Transformed OLED glyph">${rects.join("")}</svg>`;
}

function tileBlockSvg(font, codes, cols, options = {}) {
  const scale = options.scale || 5;
  const fill = options.fill || "#d7f7ff";
  const visible = codes.filter((code) => code !== 0);
  const rows = Math.ceil(visible.length / cols);
  const width = cols * 6 * scale;
  const height = rows * 8 * scale;
  const rects = visible
    .map((code, index) => {
      const col = index % cols;
      const row = Math.floor(index / cols);
      return glyphRects(font, code, col * 6 * scale, row * 8 * scale, scale, fill);
    })
    .join("");
  return `<svg viewBox="0 0 ${width} ${height}" width="${width}" height="${height}" role="img" aria-label="OLED tile block">${rects}</svg>`;
}

function composedModifierSvg(font, leftTop, connectorTop, rightTop, leftBottom, connectorBottom, rightBottom) {
  const top = [...iconArrays[leftTop].filter((code) => code !== 0), ...iconArrays[connectorTop].filter((code) => code !== 0), ...iconArrays[rightTop].filter((code) => code !== 0)];
  const bottom = [...iconArrays[leftBottom].filter((code) => code !== 0), ...iconArrays[connectorBottom].filter((code) => code !== 0), ...iconArrays[rightBottom].filter((code) => code !== 0)];
  return tileBlockSvg(font, [...top, ...bottom], top.length, { scale: 6 });
}

function decodeRleFrame(frame) {
  const size = frame[0];
  const bytes = [];
  let i = 1;
  while (i < size) {
    let count = frame[i];
    i += 1;
    if (count & 0x80) {
      count &= 0x7f;
      for (let n = 0; n < count; n += 1) {
        bytes.push(frame[i]);
        i += 1;
      }
    } else {
      const value = frame[i];
      i += 1;
      for (let n = 0; n < count; n += 1) {
        bytes.push(value);
      }
    }
  }
  return bytes;
}

function rawOledSvg(bytes, options = {}) {
  const scale = options.scale || 2;
  const fill = options.fill || "#d7f7ff";
  const width = 128;
  const height = 32;
  const rects = [];
  for (let index = 0; index < bytes.length; index += 1) {
    const x = index % width;
    const page = Math.floor(index / width);
    const byte = bytes[index] || 0;
    for (let bit = 0; bit < 8; bit += 1) {
      if (byte & (1 << bit)) {
        rects.push(`<rect x="${x * scale}" y="${(page * 8 + bit) * scale}" width="${scale}" height="${scale}" fill="${fill}"/>`);
      }
    }
  }
  return `<svg viewBox="0 0 ${width * scale} ${height * scale}" width="${width * scale}" height="${height * scale}" role="img" aria-label="Decoded OLED frame">${rects.join("")}</svg>`;
}

function glyph(font, code) {
  return font.slice(code * 6, code * 6 + 6);
}

function glyphKey(bytes) {
  return bytes.join(",");
}

function bitReverse(byte) {
  let out = 0;
  for (let bit = 0; bit < 8; bit += 1) {
    if (byte & (1 << bit)) {
      out |= 1 << (7 - bit);
    }
  }
  return out;
}

function mirrorGlyphX(bytes) {
  return [...bytes].reverse();
}

function mirrorGlyphY(bytes) {
  return bytes.map(bitReverse);
}

function overlayGlyph(bytes, mask) {
  return bytes.map((byte, index) => byte | (mask[index] || 0));
}

function clearGlyph(bytes, mask) {
  return bytes.map((byte, index) => byte & ~(mask[index] || 0));
}

function shiftGlyphX(bytes, amount) {
  const out = Array(6).fill(0);
  for (let index = 0; index < 6; index += 1) {
    const target = index + amount;
    if (0 <= target && target < 6) {
      out[target] = bytes[index];
    }
  }
  return out;
}

function shiftGlyphY(bytes, amount) {
  return bytes.map((byte) => {
    if (amount > 0) {
      return (byte << amount) & 0xff;
    }
    return byte >> Math.abs(amount);
  });
}

function outlineGlyph(bytes) {
  const horizontal = bytes.map((byte, index) => byte | (bytes[index - 1] || 0) | (bytes[index + 1] || 0));
  return horizontal.map((byte) => byte | ((byte << 1) & 0xff) | (byte >> 1));
}

function glyphDelta(from, to) {
  const added = from.map((byte, index) => (to[index] || 0) & ~byte);
  const removed = from.map((byte, index) => byte & ~(to[index] || 0));
  const xorMask = from.map((byte, index) => byte ^ (to[index] || 0));
  const orDerived = overlayGlyph(from, added);
  return {
    added,
    removed,
    xorMask,
    orDerived,
    removedPixels: countPixels(removed),
    addedPixels: countPixels(added),
    orExact: glyphKey(orDerived) === glyphKey(to),
  };
}

function countPixels(bytes) {
  return bytes.reduce((sum, byte) => {
    let count = 0;
    for (let bit = 0; bit < 8; bit += 1) {
      if (byte & (1 << bit)) {
        count += 1;
      }
    }
    return sum + count;
  }, 0);
}

function uniqueNumbers(arrays) {
  return [...new Set(Object.values(arrays).flat().filter((code) => code !== 0))].sort((a, b) => a - b);
}

function findGlyphPairs(font, usedCodes, transform) {
  const lookup = new Map(usedCodes.map((code) => [glyphKey(glyph(font, code)), code]));
  const pairs = [];
  for (const code of usedCodes) {
    const match = lookup.get(glyphKey(transform(glyph(font, code))));
    if (match !== undefined && code < match) {
      pairs.push([code, match]);
    }
  }
  return pairs;
}

function bytesToPixels(bytes) {
  const width = 128;
  const height = 32;
  const pixels = Array.from({ length: height }, () => Array(width).fill(0));
  for (let index = 0; index < bytes.length; index += 1) {
    const x = index % width;
    const page = Math.floor(index / width);
    const byte = bytes[index] || 0;
    for (let bit = 0; bit < 8; bit += 1) {
      if (byte & (1 << bit)) {
        pixels[page * 8 + bit][x] = 1;
      }
    }
  }
  return pixels;
}

function mirrorPixelsX(pixels) {
  return pixels.map((row) => [...row].reverse());
}

function pixelDiff(a, b) {
  let diff = 0;
  for (let y = 0; y < a.length; y += 1) {
    for (let x = 0; x < a[y].length; x += 1) {
      if (a[y][x] !== b[y][x]) {
        diff += 1;
      }
    }
  }
  return diff;
}

function catMirrorAudit(catFrames) {
  const pairs = [
    ["idle0", "left_idle0"],
    ["idle1", "left_idle1"],
    ["idle2", "left_idle2"],
    ["idle3", "left_idle3"],
    ["paws", "left_paws"],
    ["tap0", "left_tap0"],
    ["tap1", "left_tap1"],
  ];
  return pairs
    .filter(([right, left]) => catFrames[right] && catFrames[left])
    .map(([right, left]) => {
      const rightPixels = bytesToPixels(decodeRleFrame(catFrames[right]));
      const leftPixels = bytesToPixels(decodeRleFrame(catFrames[left]));
      return {
        right,
        left,
        mirrorDiffPixels: pixelDiff(mirrorPixelsX(rightPixels), leftPixels),
        rightRleBytes: catFrames[right][0],
        leftRleBytes: catFrames[left][0],
      };
    });
}

function hex(code) {
  return `0x${code.toString(16).padStart(2, "0")}`;
}

function makeAudit(font, iconArrays, catFrames) {
  const glyphCount = font.length / 6;
  const usedCodes = uniqueNumbers(iconArrays);
  const usedCustom = usedCodes.filter((code) => code >= 0x80);
  const highestUsed = usedCodes.at(-1) || 0;
  const customFontSlots = Math.max(0, glyphCount - 0x80);
  const packedCustomSlots = usedCustom.length;
  const packedFontBytes = (0x80 + packedCustomSlots) * 6;
  const horizontalGlyphPairs = findGlyphPairs(font, usedCustom, mirrorGlyphX);
  const verticalGlyphPairs = findGlyphPairs(font, usedCustom, mirrorGlyphY);
  const catMirrors = catMirrorAudit(catFrames);

  return {
    generatedAt: new Date().toISOString(),
    sources: {
      font: path.relative(root, fontPath).replaceAll("\\", "/"),
      icons: path.relative(root, iconsPath).replaceAll("\\", "/"),
      bongocat: path.relative(root, catPath).replaceAll("\\", "/"),
    },
    font: {
      bytes: font.length,
      glyphCount,
      customSlots: customFontSlots,
      usedGlyphsByIcons: usedCodes.length,
      usedCustomGlyphsByIcons: usedCustom.length,
      highestIconGlyphUsed: hex(highestUsed),
      theoreticalPackedFontBytes: packedFontBytes,
      theoreticalPackingSavingBytes: font.length - packedFontBytes,
    },
    glyphTransforms: {
      horizontalMirrorPairs: horizontalGlyphPairs.map(([a, b]) => [hex(a), hex(b)]),
      verticalMirrorPairs: verticalGlyphPairs.map(([a, b]) => [hex(a), hex(b)]),
      note: "Pairs are exact glyph-byte transforms among custom glyphs referenced by oled-icons.c. Firmware savings require compile measurement because transform code has a byte cost.",
    },
    bongocatTransforms: {
      horizontalMirrorPairs: catMirrors,
      duplicatedLeftRleBytes: catMirrors.reduce((sum, row) => sum + row.leftRleBytes, 0),
      measuredFirmwareSavingBytes: 1260,
      note: "The measured firmware saving includes removed left RLE data minus added transform code and compiler effects.",
    },
  };
}

function auditTable(audit) {
  const rows = [
    ["Font bytes", audit.font.bytes],
    ["Custom glyphs used by oled-icons.c", audit.font.usedCustomGlyphsByIcons],
    ["Highest icon glyph used", audit.font.highestIconGlyphUsed],
    ["Theoretical tight font saving", `${audit.font.theoreticalPackingSavingBytes} bytes`],
    ["Horizontal glyph mirror pairs", audit.glyphTransforms.horizontalMirrorPairs.length],
    ["Vertical glyph mirror pairs", audit.glyphTransforms.verticalMirrorPairs.length],
    ["Mirrored cat firmware saving", `${audit.bongocatTransforms.measuredFirmwareSavingBytes} bytes measured`],
  ];
  return `
    <table>
      <tbody>
        ${rows.map(([label, value]) => `<tr><th>${label}</th><td>${value}</td></tr>`).join("")}
      </tbody>
    </table>`;
}

function pairList(pairs) {
  if (!pairs.length) {
    return "<p class=\"muted\">No exact pairs found.</p>";
  }
  return `<ul>${pairs.map(([a, b]) => `<li><code>${a}</code> -> <code>${b}</code></li>`).join("")}</ul>`;
}

function catAuditList(rows) {
  return `<ul>${rows.map((row) => `<li><code>${row.right}</code> mirrors to <code>${row.left}</code>: ${row.mirrorDiffPixels} differing pixels</li>`).join("")}</ul>`;
}

function transformDemo(font) {
  const samples = [
    {
      name: "GUI off tile",
      code: 0x85,
      mask: [0x00, 0x00, 0x18, 0x18, 0x00, 0x00],
    },
    {
      name: "Connector off/off tile",
      code: 0xc5,
      mask: [0x00, 0x18, 0x3c, 0x3c, 0x18, 0x00],
    },
    {
      name: "Numbers icon fragment",
      code: 0x94,
      mask: [0x00, 0x7e, 0x42, 0x42, 0x7e, 0x00],
    },
  ];
  const transforms = [
    ["Stored", (bytes) => bytes],
    ["Mirror X", mirrorGlyphX],
    ["Mirror Y", mirrorGlyphY],
    ["Shift right", (bytes) => shiftGlyphX(bytes, 1)],
    ["Shift down", (bytes) => shiftGlyphY(bytes, 1)],
    ["Overlay mask", (bytes, sample) => overlayGlyph(bytes, sample.mask)],
    ["Clear mask", (bytes, sample) => clearGlyph(bytes, sample.mask)],
    ["Thicken", outlineGlyph],
  ];

  return samples.map((sample) => {
    const bytes = glyph(font, sample.code);
    return `
      <article class="asset wide">
        <h3>${sample.name} <code>0x${sample.code.toString(16)}</code></h3>
        <div class="transformGrid">
          ${transforms.map(([name, transform]) => `
            <div>
              <div class="preview">${glyphBytesSvg(transform(bytes, sample), { scale: 7 })}</div>
              <p>${name}</p>
            </div>
          `).join("")}
        </div>
      </article>`;
  }).join("");
}

function onOffComparisonDemo(font) {
  const comparisons = [
    ["Function top", 0x85, 0x8d],
    ["Function bottom", 0xa5, 0xad],
    ["Symbols top", 0x87, 0x8f],
    ["Symbols bottom", 0xa7, 0xaf],
    ["Numbers top", 0x89, 0x91],
    ["Numbers bottom", 0xa9, 0xb1],
    ["Control top", 0x8b, 0xcd],
    ["Control bottom", 0xab, 0xcf],
    ["Connector top off/off -> on/off", 0xc5, 0xc7],
    ["Connector top off/off -> off/on", 0xc5, 0xc9],
    ["Connector top off/off -> on/on", 0xc5, 0xcb],
    ["Connector bottom off/off -> on/off", 0xc6, 0xc8],
    ["Connector bottom off/off -> off/on", 0xc6, 0xca],
    ["Connector bottom off/off -> on/on", 0xc6, 0xcc],
  ];

  return comparisons.map(([name, offCode, onCode]) => {
    const off = glyph(font, offCode);
    const on = glyph(font, onCode);
    const delta = glyphDelta(off, on);
    const method = delta.orExact ? "OR mask exact" : "XOR mask needed";
    return `
      <article class="asset wide">
        <h3>${name} <code>0x${offCode.toString(16)}</code> -> <code>0x${onCode.toString(16)}</code></h3>
        <div class="transformGrid compareGrid">
          <div>
            <div class="preview">${glyphBytesSvg(off, { scale: 7 })}</div>
            <p>Stored off</p>
          </div>
          <div>
            <div class="preview">${glyphBytesSvg(on, { scale: 7 })}</div>
            <p>Stored on</p>
          </div>
          <div>
            <div class="preview">${glyphBytesSvg(delta.added, { scale: 7, fill: "#5ee68a" })}</div>
            <p>Added (${delta.addedPixels})</p>
          </div>
          <div>
            <div class="preview">${glyphBytesSvg(delta.removed, { scale: 7, fill: "#ff7a7a" })}</div>
            <p>Removed (${delta.removedPixels})</p>
          </div>
          <div>
            <div class="preview">${glyphBytesSvg(delta.orDerived, { scale: 7 })}</div>
            <p>Off OR added</p>
          </div>
          <div>
            <div class="preview">${glyphBytesSvg(delta.xorMask, { scale: 7, fill: "#ffd45e" })}</div>
            <p>XOR mask</p>
          </div>
        </div>
        <p><strong>${method}.</strong> ${delta.orExact ? "The on state can be derived by overlaying only added pixels." : "The on state changes existing pixels too, so OR-only derivation would not match the stored art."}</p>
      </article>`;
  }).join("");
}

function fontAtlas(font, start, end, title) {
  const scale = 3;
  const cols = 16;
  const tileW = 6 * scale + 18;
  const tileH = 8 * scale + 18;
  const rows = Math.ceil((end - start + 1) / cols);
  const width = cols * tileW;
  const height = rows * tileH;
  const cells = [];
  for (let code = start; code <= end; code += 1) {
    const index = code - start;
    const x = (index % cols) * tileW;
    const y = Math.floor(index / cols) * tileH;
    cells.push(`<text x="${x}" y="${y + 9}" class="tiny">0x${code.toString(16).padStart(2, "0")}</text>`);
    cells.push(glyphRects(font, code, x, y + 14, scale));
  }
  return `
    <section class="card">
      <h2>${title}</h2>
      <div class="oledPanel">
        <svg viewBox="0 0 ${width} ${height}" role="img" aria-label="${title}">
          ${cells.join("")}
        </svg>
      </div>
    </section>`;
}

const fontSource = read(fontPath);
const iconsSource = read(iconsPath);
const catSource = read(catPath);

const font = extractArray(fontSource, "font");
const iconArrays = extractNamedArrays(iconsSource, "static char const");
const catFrames = extractNamedArrays(catSource, "static unsigned char const");
const audit = makeAudit(font, iconArrays, catFrames);

const layerNames = ["base_layer", "anchor_layer", "chord_layer"];
const logoNames = ["corne_logo", "katakana"];
const statusTileNames = [
  "fn_off_1", "fn_on_1", "sym_off_1", "sym_on_1",
  "num_off_1", "num_on_1", "ctrl_off_1", "ctrl_on_1",
  "off_off_1", "on_off_1", "off_on_1", "on_on_1",
];
const catNames = ["idle0", "paws", "tap0"];

function assetRow(name, svg, note) {
  return `
    <article class="asset">
      <h3>${name}</h3>
      <div class="preview">${svg}</div>
      <p>${note}</p>
    </article>`;
}

function compositionRow(name, svg, note) {
  return `
    <article class="asset">
      <h3>${name}</h3>
      <div class="preview composed">${svg}</div>
      <p>${note}</p>
    </article>`;
}

const currentStatusStates = [
  ["Control off + Symbols off", "ctrl_off_1", "off_off_1", "sym_off_1", "ctrl_off_2", "off_off_2", "sym_off_2"],
  ["Control on + Symbols off", "ctrl_on_1", "on_off_1", "sym_off_1", "ctrl_on_2", "on_off_2", "sym_off_2"],
  ["Control off + Symbols on", "ctrl_off_1", "off_on_1", "sym_on_1", "ctrl_off_2", "off_on_2", "sym_on_2"],
  ["Control on + Symbols on", "ctrl_on_1", "on_on_1", "sym_on_1", "ctrl_on_2", "on_on_2", "sym_on_2"],
  ["Numbers off + Function off", "num_off_1", "off_off_1", "fn_off_1", "num_off_2", "off_off_2", "fn_off_2"],
  ["Numbers on + Function off", "num_on_1", "on_off_1", "fn_off_1", "num_on_2", "on_off_2", "fn_off_2"],
  ["Numbers off + Function on", "num_off_1", "off_on_1", "fn_on_1", "num_off_2", "off_on_2", "fn_on_2"],
  ["Numbers on + Function on", "num_on_1", "on_on_1", "fn_on_1", "num_on_2", "on_on_2", "fn_on_2"],
];

const html = `<!doctype html>
<html lang="en">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <title>ChieftainDots OLED Asset Preview</title>
  <style>
    :root {
      --paper: #f7f8fb;
      --panel: #ffffff;
      --ink: #111318;
      --muted: #5b6472;
      --line: #d8dde6;
      --oled: #080a0d;
      --pixel: #d7f7ff;
    }
    * { box-sizing: border-box; }
    body {
      margin: 0;
      background: var(--paper);
      color: var(--ink);
      font: 14px/1.45 system-ui, -apple-system, BlinkMacSystemFont, "Segoe UI", sans-serif;
    }
    main {
      width: min(1180px, calc(100vw - 32px));
      margin: 0 auto;
      padding: 28px 0 42px;
    }
    h1, h2, h3, p { margin: 0; }
    h1 { font-size: 28px; line-height: 1.12; letter-spacing: 0; }
    h2 { font-size: 18px; margin-bottom: 12px; }
    h3 { font-size: 14px; margin-bottom: 8px; }
    .summary {
      color: var(--muted);
      max-width: 840px;
      margin-top: 8px;
    }
    .card {
      background: var(--panel);
      border: 1px solid var(--line);
      border-radius: 8px;
      padding: 16px;
      margin-top: 16px;
    }
    .grid {
      display: grid;
      grid-template-columns: repeat(3, 1fr);
      gap: 14px;
    }
    .asset {
      border: 1px solid var(--line);
      border-radius: 8px;
      padding: 12px;
      background: #fbfcfe;
      min-width: 0;
    }
    .wide {
      grid-column: 1 / -1;
    }
    .asset p {
      color: var(--muted);
      font-size: 12px;
      margin-top: 8px;
    }
    .preview,
    .oledPanel {
      background: var(--oled);
      border-radius: 6px;
      padding: 10px;
      overflow: auto;
    }
    .oledPanel svg {
      display: block;
      min-width: 760px;
      max-width: none;
    }
    .preview svg {
      display: block;
      max-width: 100%;
      height: auto;
    }
    .transformGrid {
      display: grid;
      grid-template-columns: repeat(8, minmax(72px, 1fr));
      gap: 10px;
    }
    .compareGrid {
      grid-template-columns: repeat(6, minmax(72px, 1fr));
    }
    .transformGrid p {
      text-align: center;
    }
    .composed svg {
      margin: 0 auto;
    }
    .conceptPair {
      display: flex;
      gap: 12px;
      align-items: center;
      justify-content: center;
    }
    .explain {
      display: grid;
      grid-template-columns: 1fr 1fr;
      gap: 14px;
      color: var(--muted);
    }
    .explain strong {
      color: var(--ink);
    }
    table {
      width: 100%;
      border-collapse: collapse;
      font-size: 13px;
    }
    th,
    td {
      border-top: 1px solid var(--line);
      padding: 8px 6px;
      text-align: left;
      vertical-align: top;
    }
    th {
      width: 45%;
      color: var(--muted);
      font-weight: 600;
    }
    ul {
      margin: 0;
      padding-left: 18px;
      color: var(--muted);
    }
    li + li {
      margin-top: 3px;
    }
    .muted {
      color: var(--muted);
    }
    .tiny {
      font: 8px Consolas, monospace;
      fill: #7f8a99;
    }
    code {
      font: 12px Consolas, monospace;
      color: #303844;
    }
    @media (max-width: 900px) {
      .grid { grid-template-columns: 1fr; }
      .explain { grid-template-columns: 1fr; }
      .transformGrid { grid-template-columns: repeat(2, 1fr); }
      .compareGrid { grid-template-columns: repeat(2, 1fr); }
    }
  </style>
</head>
<body>
  <main>
    <header>
      <h1>ChieftainDots OLED Asset Preview</h1>
      <p class="summary">
        Static preview generated from the real OLED C source files. It decodes
        <code>oledfont.c</code>, <code>oled-icons.c</code>, and representative
        still frames from <code>oled-bongocat.c</code>. No animation is shown here.
      </p>
    </header>

    <section class="card">
      <h2>Asset Audit Summary</h2>
      <div class="explain">
        <div>
          ${auditTable(audit)}
        </div>
        <p>
          <strong>This page now doubles as an audit tool.</strong>
          It reads the real C arrays, reports transform candidates, and writes
          <code>docs/chieftainDots-oled-assets-audit.json</code> for repeatable
          measurements. The byte estimates here are candidates only; firmware
          savings still require compile measurements.
        </p>
      </div>
    </section>

    <section class="card">
      <h2>Exact Transform Candidates</h2>
      <div class="explain">
        <div>
          <h3>Vertical glyph mirrors</h3>
          ${pairList(audit.glyphTransforms.verticalMirrorPairs)}
        </div>
        <div>
          <h3>Horizontal glyph mirrors</h3>
          ${pairList(audit.glyphTransforms.horizontalMirrorPairs)}
        </div>
      </div>
      <div style="margin-top:14px">
        <h3>Bongocat handedness mirrors</h3>
        ${catAuditList(audit.bongocatTransforms.horizontalMirrorPairs)}
      </div>
    </section>

    <section class="card">
      <h2>What Matrix-Style Transforms Look Like</h2>
      <div class="explain">
        <p>
          <strong>These are generated from real glyph bytes.</strong>
          Each stored icon is a 6 by 8 bitmap. The previews below apply tiny
          matrix-style operations: mirror columns, reverse vertical bits, shift,
          overlay a mask, clear a mask, or thicken strokes.
        </p>
        <p>
          <strong>This is the design test before firmware.</strong>
          If a transformed state looks good here, then we can try the equivalent
          bitwise operation in QMK and compile-measure whether it saves bytes.
        </p>
      </div>
      <div class="grid" style="margin-top:14px">
        ${transformDemo(font)}
      </div>
    </section>

    <section class="card">
      <h2>Stored On/Off Compared With Derived States</h2>
      <div class="explain">
        <p>
          <strong>This compares real off/on glyph pairs.</strong>
          Green pixels are added by the on state; red pixels are removed from
          the off state. If there are no removed pixels, the on state can be
          derived with a cheap OR mask.
        </p>
        <p>
          <strong>XOR masks always recreate the target exactly,</strong>
          but they usually cost a stored mask plus code. OR-only derivation is
          the more interesting byte-saving candidate when the art allows it.
        </p>
      </div>
      <div class="grid" style="margin-top:14px">
        ${onOffComparisonDemo(font)}
      </div>
    </section>

    <section class="card">
      <h2>Status Icons From <code>oled-icons.c</code></h2>
      <div class="grid">
        ${logoNames.map((name) => assetRow(name, glyphSequenceSvg(font, iconArrays[name], { scale: 5 }), "Rendered as QMK would write the custom font characters.")).join("")}
        ${layerNames.map((name) => assetRow(name, tileBlockSvg(font, iconArrays[name], 5, { scale: 4 }), "Shown as a 5 x 3 tile block so the intended icon shape is visible.")).join("")}
      </div>
    </section>

    <section class="card">
      <h2>Current Status Screen Compositions</h2>
      <div class="explain">
        <p>
          <strong>These are the screens to inspect for moved-button artefacts.</strong>
          The renderer writes the same custom font glyphs into the same rotated
          5-column screen structure as <code>render_mod_status()</code>.
        </p>
        <p>
          <strong>R29 order is shown as Control, Symbols, Numbers, Function.</strong>
          Chord states keep the same home-row concept pressed while the middle
          tile changes to the chord artwork and the top label names the surface.
        </p>
      </div>
      <div class="grid" style="margin-top:14px">
        ${statusScreenPreview(font)}
      </div>
    </section>

    <section class="card">
      <h2>Proposed Clean Concept Tiles</h2>
      <div class="explain">
        <p>
          <strong>This is option 2 as a preview only.</strong>
          The four home-row concepts get purpose-built 12 by 16 pixel tiles
          instead of reusing moved modifier fragments. This page has not changed
          <code>oledfont.c</code> or firmware glyph bytes.
        </p>
        <p>
          <strong>The goal is to test readability first.</strong>
          If one of these reads well, the next firmware step would be to pack
          the chosen art into available custom glyph slots and compile-measure
          before flashing.
        </p>
      </div>
      <div style="margin-top:14px">
        ${proposedConceptPreview(font)}
      </div>
    </section>

    <section class="card">
      <h2>Home-Row Status Tiles From <code>oled-icons.c</code></h2>
      <div class="explain">
        <p>
          <strong>These are not single complete images.</strong>
          The renderer composes each home-row status pair from six custom font
          glyphs: left concept top, connector top, right concept top, then left
          concept bottom, connector bottom, right concept bottom.
        </p>
        <p>
          <strong>The connector is also stateful.</strong>
          The code chooses one of four connector pairs: off/off, on/off, off/on,
          or on/on. That is how the visual bridge between the two concept icons
          changes with the active state.
        </p>
      </div>
    </section>

    <section class="card">
      <h2>Composed Home-Row Status States</h2>
      <div class="grid">
        ${currentStatusStates.map(([name, ...parts]) => compositionRow(name, composedModifierSvg(font, ...parts), "Composed exactly like render_home_pair(): top row first, then bottom row.")).join("")}
      </div>
    </section>

    <section class="card">
      <h2>Raw Home-Row Status Building Blocks</h2>
      <div class="grid">
        ${statusTileNames.map((name) => assetRow(name, glyphSequenceSvg(font, iconArrays[name], { scale: 6 }), "Single-row status or connector tile from the current renderer.")).join("")}
      </div>
    </section>

    <section class="card">
      <h2>Representative Static Cat Frames From <code>oled-bongocat.c</code></h2>
      <div class="grid">
        ${catNames.map((name) => assetRow(name, rawOledSvg(decodeRleFrame(catFrames[name]), { scale: 2 }), "Decoded from stored RLE frame data as a still image. Left-side frames are derived by firmware mirroring.")).join("")}
      </div>
    </section>

    ${fontAtlas(font, 0x20, 0x7f, "ASCII Font Atlas")}
    ${fontAtlas(font, 0x80, 0xdf, "Custom Icon Font Atlas")}
  </main>
</body>
</html>`;

fs.writeFileSync(auditPath, `${JSON.stringify(audit, null, 2)}\n`, "utf8");
fs.writeFileSync(outPath, html, "utf8");
console.log(`Wrote ${auditPath}`);
console.log(`Wrote ${outPath}`);
