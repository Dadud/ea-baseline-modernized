# Foundation Portability Batch 030 — First Leakage-Cut Implementation Slice

## Goal

Land the first small implementation cuts identified by Batch 029.

This batch intentionally stays conservative. It does **not** attempt a broad renderer/runtime extraction. It only makes two bounded changes:

1. reclassify obvious `wwphys` renderer-glue files in CMake
2. cut the tiny `Combat/timemgr.cpp` renderer timing dependency on non-Windows

---

## Changes made

## 1. `wwphys` CMake reclassification

Moved these files out of `WWPHYS_RUNTIME_SIM_SOURCES` and into `WWPHYS_SCENE_RENDER_GLUE_SOURCES`:
- `phys.cpp`
- `phys.h`
- `staticanimphys.cpp`
- `staticanimphys.h`

### Why

Batch 029 showed that these files are not honest runtime-simulation core on the non-Windows portable path:
- `phys.cpp` directly includes DX8 renderer headers
- `staticanimphys.cpp` directly includes `dx8wrapper.h`
- both behave more like scene/render glue than portable simulation core

### What this accomplishes

On non-Windows, these files now defer with the existing scene/render glue island instead of silently pretending to belong to the portable runtime bucket.

This is a **classification honesty** improvement, not a behavior change.

---

## 2. `Combat/timemgr.cpp` timing dependency cut

`timemgr.cpp` previously included `ww3d.h` unconditionally and reached renderer-facing timing hooks directly.

This batch narrows that dependency by:
- including `ww3d.h` only on `_WIN32`
- wrapping movie-capture frame-rate access behind a small local helper
- wrapping render-time sync behind a small local helper
- making the non-Windows path use a no-op for the render sync / movie-capture hook path

### Why this is safe

The movie-capture and renderer sync hooks are renderer-facing concerns. On non-Windows scaffold work, treating them as unavailable is more honest than pulling in the renderer surface unconditionally.

This is a small leakage cut, not a redesign of the time system.

---

## Why this batch matters

Batch 029 identified three classes of next moves:
- reclassify obviously misbucketed files
- take tiny removable leaks first
- leave deeper architecture problems alone until boundaries are clearer

Batch 030 follows that rule exactly.

It deliberately avoids:
- touching `datasafe`
- broad `ww3d2` extraction work
- fake renderer abstractions
- overclaiming runtime portability

---

## Verification

### Doc verification
```bash
python3 scripts/architecture/check_doc_sync.py
```

Result: passed.

### Build verification
```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build/cmake-scaffold -j4
```

Observed successful end-state included:
```text
[100%] Built target wwnet
```

---

## Outcome

Batch 030 is a successful first leakage-cut slice.

### Improvements achieved
- `wwphys` portable/runtime classification is more honest
- one small `Combat` renderer dependency is now narrowed on non-Windows
- the project now has a proven pattern for turning Batch 029 inventory findings into small implementation cuts

### What this does **not** solve
- the larger `ww3d2` header-surface leak through `mesh.h`
- broader renderer/runtime separation in `Combat`
- `datasafe` product-shell coupling
- true renderer backend extraction

---

## Recommended next step

Proceed to the next high-value shared leakage cut:

### Preferred next batch
Attack the `ww3d2` header-surface leak beginning with:
- `mesh.h -> dx8polygonrenderer.h -> dx8wrapper.h -> d3d8.h`

That is the highest-leverage shared upstream leak identified by Batch 029.
