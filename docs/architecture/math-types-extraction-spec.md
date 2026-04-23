# Math Types Extraction Spec

**Problem:** `Code/ww3d2/` contains both portable game-logic math types and DirectX 8 renderer code in the same physical directory. Any source file that includes a math header transitively reaches DirectX headers, blocking `Combat`, `wwphys`, and `wwui` from building without the DX8 SDK.

**Goal:** Separate the math types into a portable library with no DirectX dependency, so upstream targets can use Vector3, Sphere, OBBox, etc. without pulling in the renderer.

---

## Scope

Extract from `Code/ww3d2/` into a new `Code/engine_math/` directory:

### Must extract

| File | Description |
|------|-------------|
| `Vector3.h` | 3D vector |
| `Vector3.cpp` | Implementation if any |
| `Matrix4.h` | 4x4 transformation matrix |
| `Matrix4.cpp` | Implementation |
| `Sphere.h` | Bounding sphere |
| `OBBox.h` | Oriented bounding box |
| `AABox.h` | Axis-aligned bounding box |
| `Plane.h` | 3D plane |
| `Quaternion.h` | Quaternion rotation |
| `MathDef.h` | Common math constants (PI, etc.) |

### Likely extractable (needs audit)

| File | Description | Audit needed |
|------|-------------|-------------|
| `Vector4.h` | 4D vector | Check for renderer coupling |
| `Matrix3.h` | 3x3 rotation/scale matrix | Check for renderer coupling |
| `ProjectionMatrix.h` | Projection matrix helpers | Check for DirectX references |
| `Color.h` | RGBA color | Check for DirectX pixel format references |
| `UVAnim.h` | UV animation | Check for DirectX references |

### Do NOT extract (renderer-only)

| File | Reason |
|------|--------|
| `dx8wrapper.h` | DirectX 8 wrapper |
| `dx8device.h` | DirectX 8 device |
| `dx8caps.h` | DirectX 8 capabilities |
| `d3d8.h` | DirectX 8 SDK header (absent) |
| `dx8indexbuffer.h/cpp` | DirectX index buffer |
| `dx8vertexbuffer.h/cpp` | DirectX vertex buffer |
| `dx8swapchain.h/cpp` | DirectX swap chain |
| `dx8polygonrenderer.h/cpp` | DirectX polygon renderer |
| `render2d.h/cpp` | 2D renderer |
| `render2dsentence.h/cpp` | 2D text renderer |
| `assetmgr.h/cpp` | Asset manager (reaches dx8) |
| `ww3d.h/cpp` | Main renderer entry |
| `pointgr.h/cpp` | Point rendering |
| `meshbuild.h/cpp` | Mesh building |

---

## Step-by-step extraction plan

### Phase 1: Create new directory and files

```bash
mkdir -p Code/engine_math
```

Copy the must-extract files. Do NOT modify the originals yet.

### Phase 2: Audit each extracted header

For each extracted header, verify:

1. No `#include` of any file in `Code/ww3d2/` that starts with `dx8`, `d3d`, `render2d`, `assetmgr`, `ww3d`
2. No `#include` of `windows.h`
3. No DirectX types: `LPDIRECT3D*`, `IDirect3D*`, `D3D*`, `LPDIRECTX*`
4. No inline assembly

If any of these are found, either:
- Remove the offending include and stub the dependency
- Move the header back to `ww3d2` and document the coupling

### Phase 3: Add backward-compatibility forwarding headers

Create forwarding headers in `Code/ww3d2/` so existing includes continue to work:

```cpp
// Code/ww3d2/Vector3.h — BACKWARD COMPATIBILITY ONLY
// This file redirects to the portable engine_math location.
// Do not add new code here. Include engine_math/Vector3.h directly.
#ifndef WW3D2_VECTOR3_H
#define WW3D2_VECTOR3_H
#include "engine_math/Vector3.h"
#endif
```

This preserves all existing `#include "../ww3d2/Vector3.h"` chains during the transition.

### Phase 4: Update upstream CMakeLists

Add `Code/engine_math` to include directories for:
- `Code/Combat/CMakeLists.txt`
- `Code/wwphys/CMakeLists.txt`
- `Code/wwui/CMakeLists.txt`

Do NOT add the renderer-specific ww3d2 files to these targets.

### Phase 5: Verify no DirectX reachability

Build each upstream target and verify it does NOT try to link against DirectX:

```bash
cmake --build build/cmake-scaffold --target Combat 2>&1 | grep -i "d3d\|directx\|dx8"
# Should return nothing
```

If DirectX symbols appear, the extraction is incomplete.

### Phase 6: Remove backward-compat forwarding headers (optional, later)

Once all upstream targets have switched to `engine_math/` includes, the forwarding headers in `Code/ww3d2/` can be removed. This is a cleanup step, not a blocker.

---

## Risks

1. **Inline assembly** — some math functions use x86 inline assembly that may not compile on non-x86 platforms. Needs platform preprocessor guards.
2. **Template coupling** — Matrix4 and Vector3 may use shared template utilities that reach back into renderer headers.
3. **Float precision** — the original code uses x87 floating-point semantics. Any change to SSE2 or soft-float must preserve precision for savegame/network compatibility.
4. **Savegame serialization** — Vector3, Sphere, OBBox, AABox are serialized in savegames. Changing their memory layout would break savegame compatibility. The platform layer must preserve exact struct layout.

---

## Verification

### Compile test (no DirectX)
```bash
cmake --build build/cmake-scaffold --target Combat 2>&1 | grep -i "d3d\|directx\|dx8\|ddraw"
# Must return nothing
```

### Struct layout test
```cpp
static_assert(sizeof(Vector3) == 12, "Vector3 must be 12 bytes (3 floats)");
static_assert(sizeof(Sphere) == 16, "Sphere must be 16 bytes (Vector3 + float radius)");
```

### Savegame compatibility test
Load a savegame from the original game. Verify spatial data (positions, bounding volumes) deserializes correctly with the extracted math types.
