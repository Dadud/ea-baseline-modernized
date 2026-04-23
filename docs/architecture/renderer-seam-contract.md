# Renderer Seam Contract — ww3d2

**Target:** `ww3d2`
**Status:** `deferred — blocked by DirectX 8 SDK header (d3d8.h) + header tangle`
**Required boundary:** `renderer` → `platform`, `engine_runtime`, `engine_asset_content`

---

## What this seam does

`ww3d2` is the rendering subsystem for C&C Renegade. It provides:

1. **Scene management** — 3D world representation, camera, viewport
2. **Geometry rendering** — meshes, primitives, materials, shaders
3. **Texture and surface management** — DirectX surface creation, locking, blitting
4. **Font and 2D overlay rendering** — `Render2D`, `Render2DSentence`
5. **Math types** — `Vector3`, `Matrix4`, `Sphere`, `OBBox`, `AABox` (shared with Combat, wwphys, wwui)

The **central architectural problem** is that game-logic math types (1 portable group) and DirectX renderer code (1 Windows-only group) live in the same physical directory and are #included by the same upstream targets.

---

## Why it is blocked

```
Code/ww3d2/
  Vector3.h      ← portable game-logic type
  Sphere.h       ← portable game-logic type
  Matrix4.h      ← portable game-logic type
  OBBox.h        ← portable game-logic type
  AABox.h        ← portable game-logic type
  dx8wrapper.h   ← DirectX 8 wrapper (Windows only)
  dx8device.h    ← DirectX 8 device (Windows only)
  d3d8.h         ← DirectX 8 SDK header (absent from repo)
```

Any source file that `#include "../ww3d2/Vector3.h"` transitively reaches `dx8wrapper.h` if the include chain is not guarded. This blocks:
- `Combat` (265 sources)
- `wwphys` (193 sources)
- `wwui` (84 sources)

---

## What the renderer must provide

### A. Scene / Device interface

The renderer exposes a device abstraction that the rest of the engine uses without linking to DirectX directly:

```cpp
// Minimal device facade needed by upstream
class WW3DDeviceClass {
public:
    virtual ~WW3DDeviceClass() = default;
    virtual void Clear(DWORD flags, RGBColor color, float z, DWORD stencil) = 0;
    virtual void Present(HWND hwnd) = 0;
    virtual void BeginScene() = 0;
    virtual void EndScene() = 0;
    // ... capability queries, state setters
};
```

The current `dx8wrapper.h` provides this as a set of free functions. A future renderer backend must provide an equivalent interface.

### B. 3D Math types (portable, no renderer dependency)

These types must be movable to a header-only or minimal-dependency location:

```cpp
// File: Code/engine_math/Vector3.h
class Vector3 {
public:
    float X, Y, Z;
    Vector3() = default;
    Vector3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    Vector3 operator+(const Vector3& o) const;
    Vector3 operator-(const Vector3& o) const;
    Vector3 operator*(float s) const;
    float Dot(const Vector3& o) const;
    Vector3 Cross(const Vector3& o) const;
    float Length() const;
    Vector3 Normal() const;
    // ... all game-logic methods
};

// Same for: Matrix4, Sphere, OBBox, AABox, Plane, Quaternion
```

These are game-logic types used by: `Combat`, `wwphys`, `wwui`, `wwsaveload`, `wwbitpack`, `BinkMovie`, `Commando`.

### C. Render2D / 2D sentence interface

Used by `wwui` and `Commando` for HUD text and UI overlays:

```cpp
// Minimal 2D renderer interface
class Render2DClass {
public:
    virtual void Render_String(const char* str, int x, int y, RGBColor color) = 0;
    virtual void Draw_Rect(int x, int y, int w, int h, RGBColor color) = 0;
    virtual void Draw_Line(int x1, int y1, int x2, int y2, RGBColor color) = 0;
};

class Render2DSentenceClass {
public:
    virtual void Create(Render2DClass* renderer, const char* font, int height) = 0;
    virtual void Set_Text(const char* text) = 0;
    virtual void Display(int x, int y, float scale, RGBColor color) = 0;
    virtual void Destroy() = 0;
};
```

### D. Texture / Surface interface

Used by asset loading and the save system:

```cpp
class SurfaceClass {
public:
    virtual ~SurfaceClass() = default;
    virtual int Get_Width() const = 0;
    virtual int Get_Height() const = 0;
    virtual void* Get_Pixel_Data() = 0;
    virtual void Blt(const Rect* destRect, SurfaceClass* src, const Rect* srcRect, DWORD flags) = 0;
};
```

### E. Asset manager interface

Used by `ww3d2` itself and `Commando`:

```cpp
class WW3DAssetManagerClass {
public:
    virtual ~WW3DAssetManagerClass() = default;
    virtual bool Init() = 0;
    virtual void Shutdown() = 0;
    virtual int Get_Texture_Count() const = 0;
    virtual int Find_Texture(const char* name) = 0;
};
```

---

## How to resolve the header tangle

### Option A: Extract math types (recommended)

**Steps:**
1. Create `Code/engine_math/` directory
2. Copy `Vector3.h`, `Sphere.h`, `OBBox.h`, `AABox.h`, `Matrix4.h`, `Plane.h`, `Quaternion.h` into it
3. Audit each file — ensure no `#include "../ww3d2/dx8wrapper.h"` or any DirectX header
4. Add a forwarding header `Code/ww3d2/Vector3.h` that includes `engine_math/Vector3.h` for backward compatibility during transition
5. Update `Combat`, `wwphys`, `wwui` CMakeLists to include `Code/engine_math` as an include directory
6. Update `ww3d2` CMakeLists to NOT include engine_math sources — keep only actual renderer sources

**Risk:** The portability of these math types needs verification — some may reach Win32 types through templates or inline assembly.

### Option B: Split ww3d2 into two static libs

1. `ww3d2_math` — math types only, no DirectX dependency
2. `ww3d2_renderer` — DirectX renderer, depends on `ww3d2_math`

This mirrors the header extraction but at the CMake target level. More work but cleaner long-term.

### Option C: Keep the physical separation we have and defer

Accept that `ww3d2` is one blocker target for non-Windows builds. On Windows with DX8 SDK, the whole thing builds. On Linux, `ww3d2` stays blocked and we document it as such.

This is the current state. It is acceptable for the FDS build but blocks `Commando` product builds on Linux.

---

## Dependency owners

| Type | Owner |
|------|-------|
| Math types | `engine_runtime`, `wwphys`, `wwui`, `wwsaveload` |
| 2D renderer | `wwui`, `Commando` |
| 3D scene | `Combat`, `Commando` |
| Asset manager | `ww3d2`, `Commando` |
| Texture surface | `wwsaveload`, `wwui`, `BinkMovie` |

---

## Parity checklist

- [ ] `ww3d2` CMake target exists and is architecture-complete
- [ ] DX8 SDK path configurable via CMake
- [ ] Portable math types are extracted and verified buildable without DX8
- [ ] `Render2DClass` and `Render2DSentenceClass` interfaces documented
- [ ] `SurfaceClass` interface documented
- [ ] `WW3DAssetManagerClass` interface documented
- [ ] Upstream targets (`Combat`, `wwphys`) build with extracted math types only
- [ ] Linux headless build (no renderer) does not regress
- [ ] DX8 renderer builds on Windows with SDK
- [ ] Behavior parity: same visual output from same asset pipeline confirmed
