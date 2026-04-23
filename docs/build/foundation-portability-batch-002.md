# Foundation Portability Batch 002

This batch advances the conservative EA-baseline CMake scaffold from one compiling target (`wwbitpack`) to the full initial foundation target set compiling on Linux.

## Scope

Targets covered by the initial foundation scaffold:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`

The goal is still compile scaffolding and blocker classification, not behavior redesign. The fixes are intentionally narrow and preserve the original VC6 project/source membership.

## Verified command

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Result:

```text
[100%] Built target wwtranslatedb
```

The configure step still reports the expected non-strict manifest warning for the original but absent `Code/WWMath/Vector3i.h` reference.

## Fix classes in this batch

### Case-sensitive include fixes

Windows-era projects assumed case-insensitive include lookup. Linux requires the released filename casing:

- `Code/wwdebug/wwdebug.cpp`: `Except.h`
- `Code/wwdebug/wwmemlog.cpp`: `FastAllocator.h`
- `Code/wwdebug/wwprofile.cpp`: `FastAllocator.h`
- `Code/wwsaveload/CMakeLists.txt`: `../WWMath`
- `Code/wwtranslatedb/CMakeLists.txt`: `../WWMath`

### Narrow Windows-header gating

The scaffold now avoids unconditional Windows SDK inclusion in foundation sources where a small non-Windows path is enough to compile the original target:

- `wwdebug.cpp`
- `wwmemlog.cpp`
- `wwprofile.cpp`
- `saveload.cpp`
- `translatedb.cpp`
- `systimer.h`

### Legacy MSVC language compatibility

Several VC6-era assumptions needed small GNU/Clang compatibility bridges:

- `DEFINE_AUTO_POOL` uses explicit specialization syntax for non-MSVC compilers.
- `always.h` supplies `__int64`, `_int64`, `stricmp`, `strnicmp`, `strlwr`, and `__forceinline` compatibility outside MSVC.
- Template-dependent base members in `vector.h`/`simplevec.h` are made explicit only where needed.
- Old loop-index leakage assumptions in `translatedb.cpp` and `wwmath.cpp` are localized with modern declarations.

### Minimal non-Windows runtime stand-ins

This batch adds only enough non-Windows behavior to let foundation static libraries compile:

- `wwmemlog.cpp` disables the old Win32 critical-section path and uses the existing allocator-only path on non-Windows.
- `wwprofile.cpp` gates the MSVC inline assembly counter path and uses `TIMEGETTIME()` outside Windows.
- `systimer.h` supplies a small `gettimeofday`-based `timeGetTime()` fallback outside Windows.
- `translatedb.cpp` maps the local `CreateFile`/`CloseHandle` usage to `fopen`/`fclose` for translation import/export helpers.

These are not the final platform abstraction. They are compile-scaffold bridges that preserve Windows code paths and document the areas that need centralized platform design later.

## Remaining warnings / follow-up

The full foundation build still emits legacy warnings, including:

- extra tokens after `#endif` in `chunkio.h`
- extra qualification warnings in old headers
- deprecated string-literal-to-`char *` conversions
- template-dependent lookup warnings in `multilist.h`
- incomplete-type warning in `persistfactory.h`

Those warnings are deliberately not cleaned in this batch. They should be addressed in later warning-focused passes after the initial target graph is stable.
