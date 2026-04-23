# Batch 013: `ww3d2` renderer seam mapping

## Goal

Model the original EA `Code/ww3d2/ww3d2.dsp` target conservatively as an **opt-in renderer seam**, keeping the default Linux bootstrap scaffold green while making the original renderer bucket's internal seams explicit.

This batch does **not** claim portable renderer completion or product parity. It records the first renderer boundary findings in the EA-baseline-first modernization track.

## Added CMake target

`Code/ww3d2/CMakeLists.txt` now models the original VC6 static library target name unchanged:

```text
ww3d2
```

The target is gated behind:

```text
RENEGADE_BUILD_RENDERER_SEAMS
```

Example configure command:

```bash
cmake -S . -B build/cmake-renderer-probe \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_RENDERER_SEAMS=ON
```

## Source-island classification

The original source inventory is grouped into these first-pass logical islands:

```text
WW3D2_ASSET_CONTENT_SOURCES
WW3D2_RENDER_RUNTIME_SOURCES
WW3D2_PRESENTATION_SOURCES
WW3D2_TEXTURE_MATERIAL_SOURCES
WW3D2_DX8_BACKEND_SOURCES
```

Interpretation:

- `WW3D2_ASSET_CONTENT_SOURCES` — W3D asset loading, hierarchy/animation managers, chunk/file readers, and format metadata.
- `WW3D2_RENDER_RUNTIME_SOURCES` — scene graph, render objects, lights, projectors, mesh runtime, visibility, and scene traversal.
- `WW3D2_PRESENTATION_SOURCES` — 2D/text/presentation-facing rendering helpers and higher-level visual presentation assets.
- `WW3D2_TEXTURE_MATERIAL_SOURCES` — texture/material/shader/cache/thumbnail handling.
- `WW3D2_DX8_BACKEND_SOURCES` — explicit Direct3D 8 backend implementation and debugger/state wrappers.

## Non-Windows handling

On non-Windows seam probes, the scaffold keeps the full source inventory visible but defers:

```text
WW3D2_DX8_BACKEND_SOURCES
```

That is enough to avoid pretending the explicit DX8 backend is portable, while still testing how deeply Direct3D/Win32 assumptions leak through the rest of the renderer bucket.

## Probe result

Targeted probe command:

```bash
cmake --build build/cmake-renderer-probe --target ww3d2 -j4
```

The first meaningful blockers were **not** treated as fake-header problems.

### Blocker 1: shared Win32 handle leakage through `wwlib`

```text
Code/wwlib/win.h:83:8: error: ‘HINSTANCE’ does not name a type
Code/wwlib/win.h:84:8: error: ‘HWND’ does not name a type
```

This surfaced while compiling `assetstatus.cpp`, showing that even apparently higher-level renderer/content management still reaches shared Win32-heavy platform surfaces.

### Blocker 2: Direct3D headers still leak through higher-level renderer code

```text
Code/ww3d2/dx8wrapper.h:48:10: fatal error: d3d8.h: No such file or directory
```

This surfaced while compiling `assetmgr.cpp`, showing that asset/content management is still coupled to renderer backend interfaces rather than cleanly separated from DX8.

## Architectural conclusion

`ww3d2` is **not** just a backend module. The current EA bucket mixes at least five concerns:

- asset/content loading
- renderer runtime / scene graph
- 2D/text/presentation
- texture/material/shader handling
- explicit Direct3D 8 backend implementation

Even after deferring the obvious DX8 implementation island, the remaining renderer bucket still leaks:

- shared Win32 platform surfaces (`HINSTANCE`, `HWND`)
- DX8 backend headers (`dx8wrapper.h`, `d3d8.h`)

So the current batch records `ww3d2` as a **mixed renderer/platform/asset-content seam**, not a portable renderer core.

## What this batch intentionally does not do

- no fake `d3d8.h` or D3DX headers
- no fake Win32 handle typedef layer to push compile depth artificially
- no physical source moves
- no claim that `ww3d2` builds on Linux
- no claim of client/tool parity

## Resulting status

- `ww3d2` is now **modeled**.
- `ww3d2` is **not** part of the default green scaffold.
- `ww3d2` currently has **configure/probe evidence and documented blockers**, not build or parity evidence.
