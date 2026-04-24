# Batch 031: ww3d2 Header Surface Reduction

**Date:** 2026-04-24
**Status:** ✅ Completed
**Theme:** Break the shared DX8 header chain from `ww3d2` public headers

## Goal

Reduce the shared DX8 header surface exposed through `ww3d2` public headers. Three independent cuts were made across three files.

---

## Cut 1: mesh.h — Replace dx8polygonrenderer.h with dx8list.h

**Problem:** `mesh.h` included `dx8polygonrenderer.h` (line ~50), pulling in the full chain:
```
dx8polygonrenderer.h → dx8wrapper.h → d3d8.h
```
But `mesh.h` used **zero** symbols from `dx8wrapper.h`. It only needed:
- `DX8PolygonRendererList` — a `MultiListClass<T>` typedef defined in `dx8list.h`

**Fix:** Added `#include "dx8list.h"` to `mesh.h`. A prior partial fix had removed `dx8polygonrenderer.h` but forgot the replacement — this completes that fix.

**Impact:** `mesh.h` no longer pulls in any DX8 SDK types. `mesh.cpp` still includes `dx8polygonrenderer.h` directly and compiles correctly — it legitimately calls DX8 API.

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

## Cut 3: dx8polygonrenderer.h — Pure interface header

**Problem:** `dx8polygonrenderer.h` included `dx8wrapper.h` purely to serve three inline method bodies:
- `Set_Vertex_Index_Range()`
- `Render(int base_vertex_offset)`
- `Render_Sorted(int base_vertex_offset, const SphereClass&)`

These are implementation methods, not interface methods. Having them inline in the header pulled the entire DX8 chain into every file that transitively included `dx8polygonrenderer.h`.

**Fix:**
- Moved all three method bodies from `dx8polygonrenderer.h` to `dx8polygonrenderer.cpp` as non-inline definitions
- Removed `#include "dx8wrapper.h"` from `dx8polygonrenderer.h`
- Added `#include "dx8wrapper.h"` to `dx8polygonrenderer.cpp`

**Impact:** `dx8polygonrenderer.h` is now a pure interface header. `dx8polygonrenderer.cpp` holds the DX8 implementation. This breaks the chain at the source.

**Files changed:**
- `Code/ww3d2/dx8polygonrenderer.h`
- `Code/ww3d2/dx8polygonrenderer.cpp`

---

## What Was NOT Changed (correctly deferred)

- **dynamesh.cpp**: Still includes `dx8wrapper.h` and makes extensive DX8 API calls — correct
- **mesh.cpp**: Still includes `dx8polygonrenderer.h` — correct, calls real DX8 methods
- **dx8wrapper.h, dx8caps.h, dx8fvf.h, dx8renderer.h, dx8texman.h**: ARE the DX8 backend; can't be cut without a full rendering backend rewrite
- **formconv.h**: Small cut available (just 2 public functions), deferred
- **rddesc.h**: Medium cut possible, depends on dx8caps.h first
- **Combat datasafe coupling**: `damage.h`, `playerdata.h`, `weaponmanager.h` use `safe_*` types as struct fields; product-shell data layout coupling, deferred
- **wwphys remaining leaks**: Win32/renderer coupling still present in some files

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
| `mesh.h` | `dx8polygonrenderer.h` → `dx8wrapper.h` → `d3d8.h` | `dx8list.h` only |
| `dynamesh.h` | `dx8wrapper.h` → `d3d8.h` | `vector4.h` only |
| `dx8polygonrenderer.h` | `dx8wrapper.h` (inline impl) | **Pure interface only** — no dx8wrapper.h |

All three are now DX8-SDK-free in their public header interfaces.

---

## Next Batch Candidates

1. **formconv.h** — Just 2 public functions; replace `<d3d8.h>` with forward decls
2. **rddesc.h** — D3DCAPS8/D3DADAPTER_IDENTIFIER8 stored as members; needs dx8caps.h cut first
3. **Combat datasafe coupling** — safe_\* struct field decoupling (high complexity)
4. **wwphys remaining Win32/renderer leaks** — Audit from sub-agent still pending
5. **bmp2d.h / textdraw.h** — both transitively benefit from dynamesh.h cut
