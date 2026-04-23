# 9-Bucket Architecture Map

## Purpose

This document maps every original EA VC6 project into the 9-bucket long-term architecture. It is the architectural truth for the EA baseline modernization.

**Do not physically reorganize the tree until logical target and include boundaries are proven.**

---

## The 9 Buckets

| # | Bucket | Description |
|---|--------|-------------|
| 1 | `foundation` | Zero-dependency utility libraries. Compiled on all platforms. |
| 2 | `platform` | Platform-abstraction layer: filesystem, threading, networking, registry, string. |
| 3 | `engine_runtime` | Gameplay simulation, scripting, persistence, game logic. |
| 4 | `engine_asset_content` | Asset loading, archive management, video playback, subtitle parsing. |
| 5 | `renderer` | 3D rendering, materials, textures, scene management, DirectX backend. |
| 6 | `audio` | Audio playback, Miles Sound System backend, 2D/3D positioning. |
| 7 | `input_and_control` | Input devices, UI controls, keyboard/mouse/controller. |
| 8 | `online_services` | Networking session, GameSpy, WOL, matchmaking. |
| 9 | `products` | Client, dedicated server, tools, launcher, admin interfaces. |

---

## Bucket Dependency Graph

```
foundation
    ↓
platform ←→ (cross-depends with foundation)
    ↓
engine_runtime  engine_asset_content
    ↓                    ↓
audio  physics  renderer  (cross-depends with platform + engine_runtime)
    ↓         ↓
renderer ← ← ← ← ← ← ← (cycle: audio/physics depend on renderer for scene queries)
    ↓
input_and_control
    ↓
products
    ↑
online_services (cross-depends with platform + products)
```

> **Note**: The renderer cycle (audio + physics → renderer) is the central architectural problem. Resolving it is a prerequisite for clean multi-platform builds.

---

## Project-to-Bucket Mapping

### Bucket 1: `foundation`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `wwdebug` | static lib | `wwdebug` | ✅ Built |
| `WWMath` | static lib | `wwmath` | ✅ Built |
| `wwbitpack` | static lib | `wwbitpack` | ✅ Built |
| `wwsaveload` | static lib | `wwsaveload` | ✅ Built |
| `wwtranslatedb` | static lib | `wwtranslatedb` | ✅ Built |

**Architecture notes:**
- No external dependencies between these libraries.
- `wwsaveload` depends on `wwbitpack`, `wwmath`, `wwlib`.
- `WWMath` contains soft-float and fixed-point math only; no DirectX, no Win32.

---

### Bucket 2: `platform`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `wwlib` | static lib | `wwlib` | ✅ Built (filtered) |
| `wwnet` | static lib | `wwnet` | ✅ Built (filtered) |

**Architecture notes:**
- `wwlib` is a **mixed** bucket — contains foundation utilities, platform services, AND Win32-only display/input code. CMakeLists.txt classifies every source into named islands.
- `wwnet` has a portable packet/stat helper subset. Live WinSock session sources are deferred on non-Windows.
- `wwlib` portable islands: string helpers, SHA, threading scaffold (no-op on Linux), ini/file parsing.
- `wwlib` deferred islands: DirectDraw surfaces, keyboard, message loop, registry, mono console, CPU detection.

---

### Bucket 3: `engine_runtime`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `Scripts` | DLL | `Scripts` | ✅ Built (filtered) |
| `Combat` | static lib | `Combat` | ❌ Blocked |

**Architecture notes:**
- `Scripts` produces `libScripts.so`. DLL entry points and Windows-specific glue are deferred.
- `Combat` (265 sources) is the game simulation core. It is **blocked** because it transitively includes `ww3d2` headers (Vector3, Sphere, OBBox, AABox) that live alongside DirectX code in the same physical directory. This is the core header-tangle problem.
- `Combat` future ownership: shared game simulation → `engine_runtime`, client-presented pieces → `products/client`.

---

### Bucket 4: `engine_asset_content`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `BinkMovie` | static lib | `BinkMovie` | ✅ Built (filtered) |
| `ww3d2` | static lib | `ww3d2` | ❌ Blocked — DX8 SDK |
| `wwsaveload` | static lib | `wwsaveload` | ✅ Built (in foundation) |

**Architecture notes:**
- `BinkMovie`: subtitle data/parser island builds. RAD/Bink playback and subtitle renderer glue are deferred.
- `ww3d2` is the `renderer` bucket, not `engine_asset_content`. It is listed here because it provides math types (Vector3, Matrix4, Sphere, OBBox) used by `Combat`, `wwphys`, and `wwui`.
- This dual ownership (math types + DirectX backend) in a single directory is the **header tangle** that blocks `Combat` and `wwphys`.

---

### Bucket 5: `renderer`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `ww3d2` | static lib | `ww3d2` | ❌ Blocked — DX8 SDK |

**Architecture notes:**
- `ww3d2` requires the DirectX 8 SDK header `d3d8.h`, which is not in the EA source tree.
- The directory contains both:
  1. Game-logic math types (Vector3, Matrix4, Sphere, OBBox, AABox) — portable
  2. DirectX 8 renderer backend (dx8wrapper.h, dx8device.h, etc.) — Windows-only
- **Physical file separation** of (1) from (2) is the prerequisite for building `Combat` and `wwphys` on non-Windows.
- See: `docs/architecture/renderer-seam-classification.md` (stub — to be written)

---

### Bucket 6: `audio`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `WWAudio` | static lib | `WWAudio` | ❌ Blocked — Miles SDK |

**Architecture notes:**
- Requires Miles Sound System SDK header `Mss.h`.
- Even facade handle types (Sound2DHandleClass, Sound3DHandleClass) include `WWAudio.h` which unconditionally includes `Mss.H`.
- No portable C++ subset exists without boundary cleanup.

---

### Bucket 7: `input_and_control`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `SControl` | static lib | `SControl` | ✅ Built |
| `wwui` | static lib | `wwui` | ❌ Blocked — Win32 + WCHAR |

**Architecture notes:**
- `SControl` is script/console control. Mostly portable C++.
- `wwui` (84 sources): 158 WCHAR signal hits, 42 Win32 message loop hits. Includes IME manager, dialog controls, and text rendering. Deeply Win32-coupled.

---

### Bucket 8: `online_services`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| *(no standalone project)* | — | — | — |

**Architecture notes:**
- Online services are embedded in `Commando` and `wwnet`.
- `wwnet` provides packet helpers; session/gameplay online is in `Commando`.
- GameSpy, WOL, NAT, matchmaking are in `Commando` WWOnline directory.
- WOLBrowser is a COM/IDL ActiveX control. Cannot be built on non-Windows.

---

### Bucket 9: `products`

| VC6 Project | Type | CMake Target | Status |
|-------------|------|-------------|--------|
| `Commando` | EXE/DLL | `Commando` | ❌ Blocked — all seams |
| *(28 tool projects)* | various | various | ❌ Mostly deferred |

**Architecture notes:**
- `Commando` is the product shell: client executable, dedicated server build (via `FREEDEDICATEDSERVER`), frontend UI, online glue, config, audio/renderer bootstrap. It depends on every other bucket. Cannot build until all seams are resolved.
- FDS has no standalone `.dsp`; it is an alternate `Commando` build using `FREEDEDICATEDSERVER` from `Code/Combat/specialbuilds.h`.
- All 28 tool projects are Win32/MFC/assembly. Probed tools (BandTest, MakeMix, ChunkView, MixViewer, bin2cpp, W3DView) are entirely platform-coupled.

---

## The Header-Tangle Problem

The central architectural blocker:

```
ww3d2/
  math types (portable):  Vector3.h, Sphere.h, OBBox.h, AABox.h, Matrix4.h, ...
  DirectX code (Win32):   dx8wrapper.h, dx8device.h, d3d8.h, ...

Combat/ (265 sources)
  67 files need Vector3.h, Sphere.h, OBBox.h, AABox.h, ...
  → includes ../ww3d2/*.h
  → hits d3d8.h → BUILD FAILS

wwphys/ (193 sources)
  needs same math types
  → includes ../ww3d2/*.h
  → hits d3d8.h → BUILD FAILS
```

**Solution (future work):**
Extract portable math types into `Code/wwmath2/` (or `Code/engine_math/`), leaving `ww3d2` as pure renderer. This breaks the dependency chain without changing game logic.

---

## Deferred-Item Summary

| Item | Reason | Prerequisite |
|------|--------|-------------|
| `ww3d2` renderer | `d3d8.h` — DirectX 8 SDK missing | Acquire DX8 SDK headers or implement stub |
| `wwphys` physics | Blocked by `ww3d2` header tangle | Extract math types from `ww3d2` |
| `Combat` gameplay | Blocked by `ww3d2` header tangle | Extract math types from `ww3d2` |
| `WWAudio` audio | `Mss.h` — Miles Sound System SDK missing | Acquire Miles SDK or implement stub |
| `wwui` UI | 158 WCHAR hits + Win32 msg loop | Platform IME/text boundary |
| `Commando` product | All above seams unresolved | All above |

---

## Open Questions

1. Should math types be extracted into `wwmath2` (clean separation) or should `ww3d2` be split into `ww3d2_math` + `ww3d2_renderer`?
2. Should Miles Audio be replaced with an open audio backend (OpenAL, miniaudio) for Linux parity?
3. Should DirectX 8 be replaced with a more portable API (Vulkan, Direct3D 9Ex, or a rendering abstraction) for cross-platform rendering?
4. Should the dedicated server (FDS) be decoupled from `Commando` with a standalone CMake target?
5. Should the 28 tool projects be evaluated for open-source reimplementation or replacement?
