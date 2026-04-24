# Batch 031: ww3d2 Header Surface Reduction

**Date:** 2026-04-24
**Status:** ✅ Completed
**Theme:** Break the shared DX8 header chain from `ww3d2` public headers

## Goal

Reduce the shared DX8 header surface exposed through `ww3d2` public headers. The audit identified two independent but complementary cuts: one in `mesh.h` and one in `dynamesh.h`.

---

## Cut 1: mesh.h — Replace dx8polygonrenderer.h with dx8list.h

**Problem:** `mesh.h` included `dx8polygonrenderer.h` (line ~50), pulling in the full chain:
```
dx8polygonrenderer.h → dx8wrapper.h → d3d8.h
```
But `mesh.h` used **zero** symbols from `dx8wrapper.h`. It only needed:
- `DX8PolygonRendererList` — a `MultiListClass<T>` typedef defined in `dx8list.h`

**Fix:** Added `#include "dx8list.h"` to `mesh.h`. The prior partial fix (commit `869788b`) removed `dx8polygonrenderer.h` but forgot to add the replacement — this completes that fix.

**Impact:** `mesh.h` no longer pulls in any DX8 SDK types. `mesh.cpp` still includes `dx8polygonrenderer.h` directly and continues to compile correctly — it legitimately calls DX8 API.

**Files changed:**
- `Code/ww3d2/mesh.h`

---

## Cut 2: dynamesh.h — Remove dx8wrapper.h, extract Color_Convert_Clamp

**Problem:** `dynamesh.h` included `dx8wrapper.h` for one function call:
```cpp
DX8Wrapper::Convert_Color_Clamp(Vector4(r,g,b,a))
```
`Convert_Color_Clamp` is a pure math utility: clamp components to [0,1], pack as ABGR. It touches zero DX8 device state or types.

**Fix:** 
1. Added `Color_Convert_Clamp(const Vector4&)` to `WWMath/vector4.h` — a pure C, portable implementation
2. Replaced all three `DX8Wrapper::Convert_Color_Clamp` calls in `dynamesh.h` with `Color_Convert_Clamp`
3. Removed `#include "dx8wrapper.h"` from `dynamesh.h`, replaced with `#include "vector4.h"`

**Files changed:**
- `Code/WWMath/vector4.h` (new helper added)
- `Code/ww3d2/dynamesh.h`

---

## What Was NOT Changed (correctly)

- **dynamesh.cpp**: Still includes `dx8wrapper.h` and makes extensive DX8 API calls — this is correct and expected
- **mesh.cpp**: Still includes `dx8polygonrenderer.h` — correct, it calls real DX8 methods
- Other `ww3d2` files: The audit found more files with `dx8wrapper.h` in their include chains, but those are deferred to future batches

---

## Verification

- Default scaffold configure: ✅
- Default scaffold build: ✅ (`[100%] Built target wwnet`)
- `check_doc_sync.py`: ✅
- `git diff --check`: ✅ (no whitespace errors)

---

## Impact on Include Graph

| Header | Before | After |
|--------|--------|-------|
| `mesh.h` | `dx8polygonrenderer.h` → `dx8wrapper.h` → `d3d8.h` | `dx8list.h` only (pure typedef/containers) |
| `dynamesh.h` | `dx8wrapper.h` → `d3d8.h` | `vector4.h` only (math utilities) |

Both are now DX8-SDK-free in their public header interfaces.

---

## Next Batch Candidates

1. **meshmdl.h** — Has `dx8list.h` dependency, used widely across ww3d2; worth auditing if it's a good next cut
2. **dx8polygonrenderer.h itself** — the inline Render/Render_Sorted methods use DX8Wrapper inline; can those be moved to the .cpp?
3. **bmp2d.h / textdraw.h** — both include `dynamesh.h` and now benefit from the dynamesh.h cut automatically
4. **Combat deep datasafe coupling** — `damage.h`, `playerdata.h`, `weaponmanager.h` use `safe_*` types as struct fields; requires product-shell data layout review
