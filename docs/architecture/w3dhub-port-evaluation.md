# W3DHub / OpenW3D Port Evaluation

**Date:** 2026-04-23
**Purpose:** Evaluate which OpenW3D fork implementations should be adopted into the EA baseline modernization, and how.

---

## What OpenW3D has that EA baseline doesn't

| Component | OpenW3D approach | EA baseline status |
|-----------|-----------------|-------------------|
| File I/O | SDL3 RWops | Win32-only in rawfile.cpp; no cross-platform layer |
| Threading | SDL3 threads + mutexes | Minimal no-op fallbacks in thread.cpp |
| Audio backend | OpenAL2 implementation in `Code/WWAudio/openal/` | Deferred — Miles SDK required |
| Video/audio decode | FFMpeg in `Code/WWAudio/openal/FFMpegBuffer.cpp` | Not available |
| Renderer stubs | DXVK-style D3D9 type wrappers in `Code/dxvk_wrapper/` | Stub-only (no real backend) |
| Text/encoding | ICU integration (optional, `W3D_BUILD_OPTION_ICU`) | TCHAR/WCHAR bridges only |
| Win32 compatibility | CMake `W3D_CLIENT` gating, `W3D_FDS` flag | Same gating approach |

---

## What's already reusable from OpenW3D

### 1. SDL3 platform layer

**Files to consider adopting:**
- `Code/wwlib/rawfile.cpp` — SDL3 RWops for file I/O
- `Code/wwlib/rcfile.cpp` — SDL3 config/path handling
- `Code/wwlib/wwfile.h` — SDL3 file helpers
- `Code/wwutil/ProcessManager.cpp` — SDL3 process management

**What it does:**
```cpp
#if defined(OPENW3D_SDL3)
    SDL_RWops* rw = SDL_RWFromFile(path, mode);
    // ... SDL3 file operations
#else
    // Win32 path
#endif
```

**Decision: ADOPT** — SDL3 is a clean, BSD-licensed cross-platform layer. The EA baseline's minimal fallbacks are insufficient for a real FDS build. SDL3 gives us file I/O, threading, mutexes, and time in one dependency.

**Integration path:**
1. Add `Code/platform/sdl3/` CMake subdirectory
2. Replace `Code/wwlib/rawfile.cpp` Win32 path with SDL3 RWops
3. Add `find_package(SDL3)` to top-level CMake
4. Keep Win32 path as fallback on Windows (no behavior change for Windows builds)

### 2. OpenAL audio backend

**Files to consider adopting:**
- `Code/WWAudio/openal/OpenALAudio.h/cpp` — full audio backend
- `Code/WWAudio/openal/OpenALHandle.h/cpp` — sound handle implementation
- `Code/WWAudio/openal/FFMpegBuffer.h/cpp` — FFmpeg-based audio buffer

**What it does:**
```cpp
class OpenALAudioClass final : public WWAudioClass {
    void Initialize(bool stereo, int bits, int hertz) override;
    void Play_Sample(int handle_id, float volume, float pan) override;
    // ... full Miles-equivalent API
};
```

**Decision: ADOPT WITH CAUTION** — The OpenAL backend is the most significant real implementation in OpenW3D. It gives Linux audio without Miles SDK.

**Key concerns:**
1. **Audio handle types** — OpenAL uses different handle classes than the Miles path. The WWAudio facade (`WWAudioClass`, `SoundHandleClass`) must bridge both.
2. **Miles compatibility** — Miles-specific features (3D positioning, EAX, specific buffer formats) may not all map to OpenAL. Acceptance criteria needed.
3. **FFMpeg** — The `FFMpegBuffer` gives audio/video decode. This is valuable but adds FFmpeg as a build dependency.

**Integration path:**
1. Add `Code/WWAudio/openal/` to `WWAudio` CMakeLists under `W3D_BUILD_OPTION_OPENAL`
2. Add `WWAudio_OpenAL_SOURCES` CMake variable for the OpenAL-specific sources
3. Add `WWAudio_Miles_SOURCES` for Miles-specific sources (Windows only)
4. Add `FFMpegBuffer.cpp/h` under `W3D_BUILD_OPTION_FFMPEG`
5. Define audio backend selection in CMake: `W3D_AUDIO_BACKEND` (OpenAL | Miles | Null)

### 3. DXVK D3D9 type wrappers

**Files to consider adopting:**
- `Code/dxvk_wrapper/d3d9.h` — D3D9 type definitions
- `Code/dxvk_wrapper/d3dx9.h` — D3DX9 type definitions
- `Code/dxvk_wrapper/dxvk_wrapper_compat.h` — compatibility layer
- `Code/dxvk_wrapper/ocidl.h` — COM interface definitions
- `Code/dxvk_wrapper/rpc.h` — RPC definitions

**What it does:**
Provides D3D9 struct/enum/type definitions so code that includes `<d3d9.h>` can compile on Linux without the actual DirectX SDK. It does NOT implement D3D9 — it only provides the types.

```cpp
// dxvk_wrapper_compat.h (non-Windows path)
typedef void *LPGUID;
typedef long HRESULT;
#define STDAPI HRESULT
// ... all D3D9 types as void* or minimal structs
```

**Decision: ADOPT** — This is already being used in OpenW3D's Linux build. It gives us type-level D3D9 compatibility without the SDK.

**Important caveat:** This is NOT a renderer backend. The actual D3D9 function implementations are not here — they are in the DX8/DX9 wrapper. For the EA baseline FDS build, the renderer is gated anyway, so these type wrappers are sufficient for compilation.

### 4. ICU text layer

**What it does:** Unicode normalization and conversion via IBM's ICU library. Enables proper UTF-8/UTF-16 handling across platforms.

**Decision: EVALUATE LATER** — ICU is a large dependency (12MB+ binary). The TCHAR/WCHAR bridges we have are sufficient for the current scaffold. ICU can be evaluated when we get to the wwui/Win32 text boundary work.

---

## What the EA baseline has that OpenW3D doesn't

| Feature | EA baseline | OpenW3D |
|---------|-------------|---------|
| Original source layout preserved | Yes | No (reorganized) |
| Per-batch documentation | Yes | Partial |
| Deferred-source ledger | Yes | No |
| 9-bucket architecture map | Yes | No |
| Source-island CMake classification | Yes | Partial |
| Stub contracts | Yes | No |
| Windows-first scaffold (SDK opt-in) | Yes | Linux-first |

The EA baseline is the better **documentation and architecture** foundation. OpenW3D is the better **practical implementation** foundation. The goal is to combine them.

---

## Recommended adoption strategy

### Phase 1: Platform layer (SDL3)

Adopt SDL3 as the platform layer for the EA baseline.

**Changes:**
1. Add `Code/platform/CMakeLists.txt` with SDL3 detection
2. Move/modify `Code/wwlib/rawfile.cpp` SDL3 path into `Code/platform/`
3. Add `Code/platform/sdl3_filesystem.cpp`, `sdl3_thread.cpp`, `sdl3_time.cpp`
4. Update `Code/wwlib/CMakeLists.txt` to use platform layer
5. Update `Code/wwutil/CMakeLists.txt` similarly

**Why this first:** Threading and file I/O are foundational. The FDS needs these before anything else. SDL3 is well-tested and widely available.

### Phase 2: Renderer type wrappers (DXVK)

Adopt the DXVK wrappers into the EA baseline under `Code/dxvk_wrapper/`.

**Changes:**
1. Copy `Code/dxvk_wrapper/` from OpenW3D to EA baseline
2. Add `Code/dxvk_wrapper/CMakeLists.txt` that includes the directory on all platforms
3. Ensure `dxvk_wrapper_compat.h` is included wherever D3D9 types are needed
4. Update `Code/ww3d2/CMakeLists.txt` to depend on `dxvk_wrapper`

**Why this now:** The type wrappers are needed for any code that references D3D9 types. They're already platform-agnostic. This doesn't change behavior — it just allows compilation.

### Phase 3: OpenAL audio backend

Adopt the OpenAL backend as an optional audio implementation.

**Changes:**
1. Copy `Code/WWAudio/openal/` from OpenW3D to EA baseline
2. Add `WWAudio_OpenAL_SOURCES` CMake variable
3. Add CMake option `W3D_AUDIO_BACKEND` (OpenAL | Miles | Null)
4. Update `Code/WWAudio/CMakeLists.txt` to conditionally include OpenAL sources
5. Add FFMpeg support under `W3D_BUILD_OPTION_FFMPEG`

**Why deferred:** Audio is more complex to integrate. The handle types and buffer management need careful mapping. Target for after the platform layer is stable.

### Phase 4: Replay OpenW3D deltas

After Phases 1-3, replay the OpenW3D changes on top of the EA baseline using the `openw3d-baseline-replay-strategy` skill.

---

## Specific integration notes

### SDL3 + existing wwlib fallbacks

The EA baseline currently has minimal non-Windows fallbacks in:
- `Code/wwlib/thread.cpp` — no-op thread/sleep on Linux
- `Code/wwlib/rawfile.cpp` — deferred on Linux (classified/deferred in CMakeLists)
- `Code/wwlib/systimer.h` — `gettimeofday()` shim for `timeGetTime()`

SDL3 should replace these, not coexist with them. The OpenW3D SDL3 path is cleaner and more complete.

### FFMpeg vs Miles audio

Miles Sound System is a closed proprietary audio library. The OpenW3D OpenAL + FFMpeg approach is open-source and cross-platform.

Recommendation: **Use OpenAL+FFMpeg as the primary audio backend for non-Windows builds. Use Miles for Windows builds when available.**

This preserves Windows audio behavior (Miles was used on the original) while enabling Linux audio without Miles.

### DXVK wrappers vs real DX8/DX9

The DXVK wrappers provide types only, not implementation. For the FDS build, this is fine — FDS doesn't render. For the client build on Linux, a real renderer backend is still needed.

The OpenW3D fork doesn't have a complete D3D9→Vulkan or D3D9→OpenGL translation layer — it has type definitions. A real Linux renderer backend is still future work.

### ICU for text

The ICU dependency is large. Defer until we reach wwui text boundary work. The current TCHAR/WCHAR bridges are adequate for the FDS build.

---

## Open questions

1. **FFMpeg dependency** — Adding FFMpeg adds significant build complexity. Is it needed for FDS? For the original game, Bink handled video. FFMpeg is useful for tools and future asset pipeline work. Consider making it optional.
2. **OpenAL vs miniaudio** — OpenAL is older and less maintained than miniaudio. The OpenW3D fork uses OpenAL. An alternative would be miniaudio (from mdaemon's author) which is lighter and more portable. Evaluate later.
3. **Audio handle type compatibility** — Miles and OpenAL use different internal handle representations. The `SoundHandleClass` facade needs to abstract this cleanly. This is the riskiest integration point.
4. **SDL3 version** — OpenW3D uses SDL3 (not SDL2). SDL3 is still in development. Using it means tracking SDL3 releases. Consider pinning to a specific version.
