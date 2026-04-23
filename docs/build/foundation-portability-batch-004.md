# Foundation portability batch 004: `wwlib` source-boundary classification

## Purpose

Batch 004 continues the EA-baseline-first build scaffold by treating `Code/wwlib/wwlib.dsp` as descriptive source metadata while recognizing that the historical `wwlib` bucket is not a pure foundation library.

The goal was not to finish platform portability. The goal was to classify the first mixed-source seam and make a non-Windows bootstrap subset compile without adding fake DirectDraw or Win32 SDK stubs.

## What changed

- `Code/wwlib/CMakeLists.txt` now keeps the original manifest-derived `WWLIB_SOURCES` list visible.
- Non-Windows scaffold builds remove a classified set of `wwlib` sources that belong to platform, display, DirectDraw, console/input, registry/version, or legacy platform-optimized islands.
- Narrow compile-bridge fixes were added only where they helped the remaining foundation subset compile:
  - MSVC formatting aliases such as `_vsnprintf`.
  - additional non-MSVC string helper aliases.
  - non-Windows handling for `thread.cpp` stubs already implied by the legacy `_UNIX` branches.
  - local file/string compatibility includes.
  - a narrow `StringClass::Copy_Wide` fallback using C runtime conversion for the scaffold build.

## Deferred `wwlib` source islands on non-Windows

These files remain part of the original `wwlib` source inventory, but are not compiled in the non-Windows bootstrap target yet:

```text
blit.cpp
convert.cpp
cpudetect.cpp
ddraw.cpp
dsurface.cpp
ini.cpp
jshell.cpp
keyboard.cpp
mixfile.cpp
mono.cpp
msgloop.cpp
mpu.cpp
mutex.cpp
rawfile.cpp
rcfile.cpp
refcount.cpp
regexpr.cpp
registry.cpp
strtok_r.cpp
srandom.cpp
verchk.cpp
widestring.cpp
win.cpp
xsurface.cpp
```

This is intentionally a logical classification only. No files were physically moved, and the target is still named `wwlib` to preserve the original VC6 project identity during baseline modernization.

## Why this is not a DirectDraw stub

The prior blocker was:

```text
Code/wwlib/dsurface.h:43:17: fatal error: ddraw.h: No such file or directory
```

That is an architectural boundary signal, not a missing-header problem. Adding a fake `ddraw.h` would make the scaffold less truthful by hiding the fact that historical `wwlib` mixes foundation code with renderer/display platform code.

Batch 004 instead records the boundary in CMake source membership so future work can split these areas deliberately into `foundation`, `platform`, and `renderer` buckets.

## Verification

The full current foundation/support scaffold now configures and builds on Linux:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Observed result:

```text
[100%] Built target wwlib
```

Targets covered by this scaffold:

```text
wwdebug
wwmath
wwbitpack
wwsaveload
wwtranslatedb
wwutil
wwlib
```

## Remaining caveats

- The non-Windows `wwlib` subset is compile-checkable, not semantically portable yet.
- Several compatibility bridges are temporary and should be replaced later by a deliberate platform/string/file/thread architecture.
- `wwlib` still contains multiple historical responsibilities and should eventually be split logically into smaller targets before any physical reorganization.
- Wide-character compatibility remains a known risk because Windows-era `WCHAR` was 16-bit while host `wchar_t` may be 32-bit.
- The deferred file list should be revisited one island at a time rather than reintroduced wholesale.
