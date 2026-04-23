# Foundation portability batch 003: model `wwlib` and `wwutil`

## Purpose

This batch expands the manifest-driven CMake scaffold one layer upward from the first low-risk foundation libraries into the original EA `wwlib` and `wwutil` projects.

The intent is still descriptive and conservative:

- preserve original VC6 project names as CMake targets
- source membership comes from `build-manifests/vc6-projects.json`
- avoid future bucket target names until original boundaries are understood
- apply only narrow compile-scaffold compatibility fixes
- stop when blockers become architectural rather than mechanical

## New targets

Added original-project targets:

- `wwlib` from `Code/wwlib/wwlib.dsp`
- `wwutil` from `Code/wwutil/wwutil.dsp`

`wwlib` is large and mixed for a foundation target: it contains basic containers, files, strings, compression, hashes, image helpers, debugging/mono console helpers, registry/version helpers, and some Windows/COM/DirectDraw-adjacent utility code.

`wwutil` is small and mostly support code:

- `mathutil.cpp`
- `miscutil.cpp`
- `stackdump.cpp`

## Verification performed

Configure continues to succeed with the foundation scaffold enabled:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
```

Targeted `wwutil` build succeeds:

```bash
cmake --build build/cmake-scaffold --target wwutil -j4
```

Observed result:

```text
[100%] Built target wwutil
```

Full foundation build now advances through the previous target set and into `wwlib`, but stops at a higher-risk graphics/platform seam:

```text
Code/wwlib/dsurface.h:43:17: fatal error: ddraw.h: No such file or directory
```

This is intentionally not papered over in this batch. DirectDraw surface/display code inside `wwlib` is a real legacy graphics/platform coupling and should be handled by a later platform/renderer boundary decision, not by a blind Linux stub.

## Narrow fixes applied

The fixes in this batch are low-risk scaffold compatibility only:

1. Case-sensitive include/path fixes:
   - `TARGA.CPP` includes `TARGA.H` instead of lowercase `targa.h`.
   - Windows `sys\\stat.h` spelling was normalized to `sys/stat.h` with a non-Windows `unistd.h` fallback.

2. Windows-only launch/browser code:
   - `LaunchWeb.cpp` keeps the original Win32 implementation under `_WIN32`.
   - Non-Windows scaffold builds return `false` rather than inventing product behavior.

3. Windows multimedia header wrapper:
   - `mmsys.h` now includes `<mmsystem.h>` only on `_WIN32`.
   - MSVC warning pragmas are guarded behind `_MSC_VER`.

4. Windows stack walking:
   - `stackdump.cpp` keeps the original `IMAGEHLP.DLL` path on `_WIN32`.
   - Non-Windows scaffold builds provide a logging no-op for `cStackDump::Print_Call_Stack()`.

5. COM utilities:
   - `WWCOMUtil.h/cpp` keep COM declarations and implementations under `_WIN32`.
   - This avoids forcing `oaidl.h` into non-Windows foundation builds.

6. Small libc / Win32 utility bridges:
   - non-MSVC `strupr` / `_strupr` added alongside existing string helpers in `always.h`
   - `miscutil.cpp` uses `access()` / `unlink()` on non-Windows for read-only checks and removal
   - PE timestamp extraction remains Win32-only; non-Windows scaffold builds use timestamp `0` for the ID string bridge

7. Legacy bool compatibility:
   - `bool.h` no longer tries to redefine C++ `bool`, `true`, and `false` on non-MSVC compilers.

8. Mono console handle storage:
   - `mono.h` uses a local `MONO_HANDLE` alias so non-Windows builds do not require a global Win32 `HANDLE` typedef.

## Current stop point

The next blocker is not just a case typo or old MSVC language issue. It is a DirectDraw dependency from `wwlib` surface/display helpers:

```text
Code/wwlib/dsurface.h -> <ddraw.h>
```

That belongs in the future `platform` / `renderer` boundary discussion. For now, this batch records the seam and leaves the original Windows path intact.

## Implication for the modernization plan

`wwutil` is now a compile-checkable support target.

`wwlib` is modeled, partially compiles, and has revealed that the historical `wwlib` bucket is not purely foundation. It contains at least one graphics/platform-adjacent island that must be separated logically before the project can become a clean portable foundation layer.
