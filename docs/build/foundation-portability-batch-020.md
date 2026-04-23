# Batch 020 — Probe WWAudio; classify as MSS audio seam

## What this batch does

Batch 020 attempts to model WWAudio using the automation tooling and documents the outcome: WWAudio is classified as a **Miles Sound System (MSS) audio seam**, not a portable library.

## What was attempted

1. Generated WWAudio CMakeLists.txt using `scripts/projects/write_cmake_scaffold.py`
2. Added `WWAudio` to `RENEGADE_BUILD_AUDIO_SEAMS` top-level CMake
3. Attempted to build — failed with `Mss.H: No such file or directory`

## What was found

Even the "handle facade" types (`Sound2DHandleClass`, `Sound3DHandleClass`, `SoundStreamHandleClass`) include `WWAudio.h`, which unconditionally includes `Mss.H` at the top of the translation unit:

```cpp
// WWAudio.h line 46:
#include "Mss.H"
```

This means there is **no portable C++ subset** within WWAudio that can be compiled without the Miles Sound System SDK headers. The handle wrappers are not pure facades — they are bound to the Miles type system throughout.

This is the same tight third-party SDK coupling pattern seen in:
- `BinkMovie` → RAD/Bink SDK header `Bink.H`
- `ww3d2` → DirectX 8 SDK header `d3d8.h`

## Signal data confirmation

The probe already captured this:

```
WWAudio — DirectSound/Miles/audio backend: 51 hits
  Examples: Code/WWAudio/AudibleSound.cpp, Code/WWAudio/Sound3D.cpp,
  Code/WWAudio/WWAudio.cpp, Code/WWAudio/WWAudio.h, Code/WWAudio/AudibleSound.h
```

The probe correctly predicted this would be a blocker.

## Classification

WWAudio is classified as a **third-party SDK seam** — the entire library is inseparable from the Miles Sound System. The non-Windows scaffold should not attempt to build WWAudio sources until:

1. A platform audio boundary is designed (abstract audio backend interface)
2. A stub or replacement backend (SDL_mixer, OpenAL, miniaudio) is selected
3. The handle wrapper types are refactored to not depend on MSS types in their public headers

## Decision

Do not add a WWAudio CMakeLists.txt. The deferred-source ledger entry for WWAudio remains as-is. The Miles audio seam is documented but not modeled.

## Files added

None — no CMakeLists.txt was committed for WWAudio.

## Files modified

None — scaffold unchanged.

## Scaffold impact

None — default scaffold remains 10/10 targets green (WWAudio not included in default scaffold).
