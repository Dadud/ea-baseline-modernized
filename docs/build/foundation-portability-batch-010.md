# Foundation Portability Batch 010: WWAudio Boundary Mapping

## Purpose

Batch 010 adds a conservative scaffold for the original EA VC6 `WWAudio` static-library project, but keeps it behind an opt-in CMake switch because it exposes a real audio/backend boundary rather than a small low-risk compile target.

This batch is a mapping and classification batch, not a claim that the full audio subsystem is portable.

## Added target scaffold

Original VC6 project:

```text
Code/WWAudio/WWAudio.dsp
```

CMake target name:

```text
WWAudio
```

The target is opt-in:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_AUDIO_SEAMS=ON
```

It is not part of the default scaffold yet, so the existing full scaffold remains green.

## Source islands

`Code/WWAudio/CMakeLists.txt` now records these logical source islands:

```text
WWAUDIO_EVENT_SOURCES
WWAUDIO_SAVELOAD_SOURCES
WWAUDIO_SCENE_METADATA_SOURCES
WWAUDIO_THREADING_SOURCES
WWAUDIO_UTILITY_SOURCES
WWAUDIO_MILES_BACKEND_SOURCES
```

Interpretation:

- `WWAUDIO_EVENT_SOURCES`: callback/event registration around scene audio objects.
- `WWAUDIO_SAVELOAD_SOURCES`: audio scene persistence metadata.
- `WWAUDIO_SCENE_METADATA_SOURCES`: logical scene/listener/sound object metadata.
- `WWAUDIO_THREADING_SOURCES`: delayed-release thread and synchronization helper code.
- `WWAUDIO_UTILITY_SOURCES`: legacy Miles lock utility.
- `WWAUDIO_MILES_BACKEND_SOURCES`: concrete Miles/DirectSound-backed runtime implementation and sound handles.

## Probe result

The first raw build probe found many case-sensitive include mismatches. Batch 010 fixes local `WWAudio` include spelling so the target can reach the true architectural blocker instead of stopping at Windows filesystem assumptions.

The next blockers are the expected audio/backend seams:

```text
Code/WWAudio/AudibleSound.h -> mss.h
Code/WWAudio/WWAudio.h -> Mss.H
```

Those headers belong to the proprietary Miles Sound System SDK. This batch does **not** add fake Miles headers.

The opt-in scaffold also exposes that apparently higher-level scene/event metadata currently includes `WWAudio.h` and `AudibleSound.h`, so even non-backend audio classes are coupled to Miles types. That coupling should be untangled later with an explicit audio backend boundary.

## CMake option

Batch 010 adds:

```cmake
option(RENEGADE_BUILD_AUDIO_SEAMS "Configure experimental audio seam targets" OFF)
```

Default full scaffold builds remain focused on the known green target set. Audio seam work can be configured separately when intentionally probing `WWAudio`.

## Known deferred work

Future audio work should decide how to separate:

- audio scene/content metadata;
- audio events and save/load data;
- delayed-release threading/synchronization;
- Miles/DirectSound concrete backend;
- future portable/null/OpenAL/etc. backend abstractions.

## Verification

Default scaffold verification remains:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

`WWAudio` probe/build is intentionally opt-in and currently documents the Miles/backend blocker rather than hiding it.
