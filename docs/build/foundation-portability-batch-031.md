# Foundation Portability Batch 031 — `mesh.h` Shared Header-Surface Leak Reduction

## Goal

Land a more ambitious but still bounded shared-boundary cut by reducing the DX8 backend bleed caused by the `mesh.h` / `dx8polygonrenderer.h` include cycle.

This batch focuses on the highest-value safe cut identified after Batch 030:
- break the circular include between `mesh.h` and `dx8polygonrenderer.h`
- keep the real DX8 implementation dependency in `.cpp` code where it belongs
- avoid pretending that `dynamesh.h` is the same kind of problem

---

## Problem

Before this batch:
- `mesh.h` included `dx8polygonrenderer.h`
- `dx8polygonrenderer.h` included `mesh.h`

That circular structure meant any consumer of `mesh.h` inherited a DX8-heavy public include surface even when it only needed the mesh interface.

This was a real shared header contamination path and one of the most important leakage points found in Batch 029.

---

## Changes made

## 1. Narrowed `mesh.h`

Removed:
- `#include "dx8polygonrenderer.h"`

Added forward declaration:
- `class DX8PolygonRendererClass;`

### Why this is safe

`mesh.h` only needs `DX8PolygonRendererClass` at header level for:
- friendship
- `DX8PolygonRendererList` member storage through existing list typedefs

Those uses do not require the full renderer class definition in the header.

---

## 2. Narrowed `dx8polygonrenderer.h`

Removed:
- `#include "mesh.h"`
- `#include "meshmdl.h"`

Added forward declaration:
- `class MeshClass;`

### Why this is safe

At header level, `dx8polygonrenderer.h` only uses `MeshClass` by pointer/reference shape:
- constructor parameter
- member pointer
- getter return type

That means a forward declaration is sufficient.

`meshmdl.h` was dead in practice for this header surface and only appeared in a commented-out line.

---

## What stays true after this batch

This is **not** a fake abstraction pass.

The DX8 dependency still exists where it is semantically real:
- `mesh.cpp` still includes `dx8polygonrenderer.h`
- `MeshClass::Render_Material_Pass` still uses DX8-side renderer behavior in implementation code

So this batch removes a **public header leak**, not a real renderer implementation dependency.

---

## What was explicitly deferred

`dynamesh.h` was inspected for the same batch and intentionally deferred.

Why:
- unlike `mesh.h`, `dynamesh.h` contains inline code that directly calls `DX8Wrapper::Convert_Color_Clamp(...)`
- that is a real logic dependency, not just an include-shape problem
- cleaning it up honestly would require a separate helper/abstraction decision

That should be its own batch.

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

### Diff hygiene
```bash
git diff --check
```

Result: passed.

---

## Outcome

Batch 031 successfully reduces one of the highest-value shared DX8 header leaks without overclaiming architectural progress.

### Improvements achieved
- `mesh.h` no longer drags `dx8polygonrenderer.h` into every consumer
- the circular include between `mesh.h` and `dx8polygonrenderer.h` is removed
- the DX8 dependency is pushed back into implementation space instead of public header surface

### What this does not solve
- `dynamesh.h` inline DX8 helper usage
- broader `ww3d2` backend disentangling
- deeper renderer/runtime interface redesign

---

## Recommended next step

Continue the more ambitious batch style, but keep it bounded.

### Preferred next batch
Either:
1. attack the next `ww3d2` header-surface leak adjacent to `mesh.h`, or
2. take `dynamesh.h` as its own honest refactor batch with a DX8-independent color conversion helper decision up front

My recommendation: do `dynamesh.h` only if we are ready to treat it as a small real refactor rather than another include-only cleanup.
