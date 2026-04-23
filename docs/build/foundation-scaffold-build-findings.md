# Foundation Scaffold Build Findings

The initial CMake scaffold configures successfully and now carries source membership plus VC6 include paths for the first low-level library set.

## Verified configure command

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
```

This succeeds on Linux with the expected warning about the absent EA release-tree reference:

```text
Code/WWMath/Vector3i.h
```

## First compile check

Command:

```bash
cmake --build build/cmake-scaffold -j4
```

The compile intentionally is not treated as a parity requirement yet. It is being used to classify the next modernization blockers after the baseline target/source graph exists.

Initial blocker classes:

1. **Win32 SDK assumptions**
   - Example: `Code/wwdebug/wwdebug.cpp` includes `<windows.h>`.

2. **Old MSVC standard-library header names**
   - Example: `Code/wwlib/vector.h` included `<new.h>`.
   - Modern C++ uses `<new>`.

3. **Case-insensitive include assumptions from Windows**
   - Example: `Code/wwbitpack/BitPacker.cpp` included `"bitpacker.h"`, while the released file is `BitPacker.h`.

4. **Absent source references in original project files**
   - Example: `Code/WWMath/Vector3i.h` is listed by VC6 metadata but absent from the EA release tree.

## Targeted `wwbitpack` compile check

Command:

```bash
cmake --build build/cmake-scaffold --target wwbitpack -j4
```

A first small portability batch advanced this target past several shallow compatibility blockers:

- include-case mismatch for `BitPacker.h`
- old `<new.h>` references in foundation headers/sources
- unconditional Windows header inclusion through `wwlib/win.h`
- missing non-MSVC `_cdecl` / `__cdecl` definitions
- missing non-MSVC `<tchar.h>` narrow-character compatibility
- MSVC x86 inline assembly in `FastCriticalSectionClass`
- old MSVC function-style cast / loop-scope behavior in `BitPacker.cpp`

The target now reaches a more meaningful portability decision around Windows wide-character semantics. EA/Win32 `WCHAR` is 16-bit, while Linux `wchar_t` is commonly 32-bit. This affects `BitStreamClass`, `WideStringClass`, serialization, and compatibility-sensitive string handling, so it should not be papered over with an ad-hoc typedef.

See `docs/build/foundation-portability-batch-001.md` for the detailed batch notes.

## Modernization rule for these blockers

Do not paper over these by globally renaming files or adding broad platform shims yet.

Handle them in small, documented waves:

1. Capture the blocker class.
2. Prefer compatibility wrappers or narrow conditional includes where behavior is clear.
3. Preserve original source/project membership.
4. Keep product parity and future Windows builds in view.

This keeps the baseline-first track separate from the later OpenW3D replay and physical code-bucket reorganization.
