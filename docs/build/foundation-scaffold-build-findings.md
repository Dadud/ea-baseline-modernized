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

Current blocker classes:

1. **Win32 SDK assumptions**
   - Example: `Code/wwdebug/wwdebug.cpp` includes `<windows.h>`.

2. **Old MSVC standard-library header names**
   - Example: `Code/wwlib/vector.h` includes `<new.h>`.
   - Modern C++ uses `<new>`.

3. **Case-insensitive include assumptions from Windows**
   - Example: `Code/wwbitpack/BitPacker.cpp` includes `"bitpacker.h"`, while the released file is `BitPacker.h`.

4. **Absent source references in original project files**
   - Example: `Code/WWMath/Vector3i.h` is listed by VC6 metadata but absent from the EA release tree.

## Modernization rule for these blockers

Do not paper over these by globally renaming files or adding broad platform shims yet.

Handle them in small, documented waves:

1. Capture the blocker class.
2. Prefer compatibility wrappers or narrow conditional includes where behavior is clear.
3. Preserve original source/project membership.
4. Keep product parity and future Windows builds in view.

This keeps the baseline-first track separate from the later OpenW3D replay and physical code-bucket reorganization.
