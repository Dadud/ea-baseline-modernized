# Foundation Portability Batch 001

This batch follows the EA-baseline-first plan: it keeps original project/source membership, does not replay OpenW3D fork changes, does not physically reorganize files, and only applies narrow compatibility fixes needed to advance the manifest-driven foundation scaffold.

## Fix classes covered

### Case-sensitive include cleanup

Windows tolerated mismatched include case. Linux/macOS do not.

Fixed examples:

```text
Code/wwbitpack/BitPacker.cpp: "bitpacker.h" -> "BitPacker.h"
Code/wwbitpack/bitstream.h:   "bitpacker.h" -> "BitPacker.h"
```

### Old MSVC standard-library header names

Preserved MSVC behavior while allowing modern compilers to use standard headers:

```text
<new.h> -> <new> on non-MSVC
```

Touched files:

```text
Code/wwlib/vector.h
Code/wwlib/mempool.h
Code/wwlib/data.cpp
```

### Narrow non-Windows compiler compatibility

Added narrow definitions for MSVC calling convention spellings where headers use them before including Windows headers:

```text
_cdecl
__cdecl
```

Added in:

```text
Code/wwlib/always.h
Code/wwlib/win.h
```

### Narrow non-Windows `TCHAR` bridge

`Code/wwlib/wwstring.h` used `<tchar.h>`, which is an MSVC/Windows compatibility header. For the non-Windows scaffold path, this batch maps the narrow-character `TCHAR` API to `char` and common C library functions.

This is intentionally scoped to the baseline compile scaffold and should be revisited when the platform layer is formalized.

### Legacy inline x86 assembly guard

`Code/wwlib/mutex.h` contained MSVC x86 inline assembly in a header. This batch gates the original assembly behind MSVC x86 checks and provides a minimal compiler-atomic fallback for non-MSVC scaffold compilation.

## CMake compatibility note

The foundation scaffold now applies `-fpermissive` for GNU/Clang C++ builds. This is a compatibility bridge for old MSVC-era source patterns, not a final quality target. Examples include:

- extra qualification on class members
- old template lookup behavior

Long-term cleanup should remove the need for this flag target-by-target.

## Verification run

Command:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold --target wwbitpack -j4
```

Result: progressed beyond the previous blockers:

- include-case mismatch
- `<new.h>` missing on GCC
- unconditional `<windows.h>` through `wwlib/win.h`
- missing `<tchar.h>`
- MSVC x86 inline assembly in `mutex.h`
- old MSVC function-style cast / loop-scope issues in `BitPacker.cpp`
- missing `_cdecl` / `__cdecl`

## Current next blocker

The build now reaches a more meaningful portability decision: Windows `WCHAR` is 16-bit, while Linux `wchar_t` is typically 32-bit. The temporary non-Windows bridge currently maps `WCHAR` to `wchar_t` so standard C wide-string calls compile naturally; that is not the final compatibility answer for serialized/game data.

Examples:

```text
Code/wwbitpack/bitstream.cpp: Get_Wide_Terminated_String reads into WCHAR
Code/wwlib/widestring.h: WideStringClass uses wcslen/wcscmp on WCHAR*
Code/wwlib/wwstring.h: StringClass overloads accept WCHAR*
```

Do not paper this over casually. This should become a small platform/string compatibility layer decision because it affects serialization and asset/network compatibility.
