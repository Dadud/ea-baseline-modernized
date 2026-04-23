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

A follow-up narrow wide-character batch added non-Windows `BitStreamClass` overloads that serialize `WCHAR` through `USHORT`, preserving the Win32-era 16-bit bitstream width for this path while avoiding a global string redesign.

Current targeted result:

```bash
cmake --build build/cmake-scaffold --target wwbitpack -j4
```

```text
[100%] Built target wwbitpack
```

See:

- `docs/build/foundation-portability-batch-001.md`
- `docs/build/legacy-wide-character-compatibility.md`

## Full initial foundation build check

The broader initial foundation target set now compiles on Linux:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Current result:

```text
[100%] Built target wwtranslatedb
```

Targets covered:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`

The build still uses `-fpermissive` as a temporary VC6-era bridge and still emits legacy warnings. The result means the first generated foundation target graph is compile-checkable; it does not mean platform compatibility, warning cleanliness, or behavior parity is finished.

See:

- `docs/build/foundation-portability-batch-002.md`

## Batch 003: `wwlib` and `wwutil` scaffold expansion

The scaffold now models two more original EA support projects:

- `wwlib` from `Code/wwlib/wwlib.dsp`
- `wwutil` from `Code/wwutil/wwutil.dsp`

Targeted `wwutil` verification succeeds:

```bash
cmake --build build/cmake-scaffold --target wwutil -j4
```

```text
[100%] Built target wwutil
```

The full foundation build now advances into `wwlib` and stops at a higher-risk DirectDraw/platform seam:

```text
Code/wwlib/dsurface.h:43:17: fatal error: ddraw.h: No such file or directory
```

This is a useful architectural finding: original `wwlib` is not a clean foundation-only bucket. It contains low-level utility code, but also Windows/COM, mono-console, registry/version, image/surface, and DirectDraw-adjacent code. Batch 003 intentionally stops short of stubbing DirectDraw because that belongs in a later `platform` / `renderer` boundary design.

See:

- `docs/build/foundation-portability-batch-003.md`
- `docs/architecture/foundation-target-map.md`

## Batch 004: `wwlib` non-Windows source classification

Batch 004 keeps original `wwlib.dsp` source membership visible but removes platform/display-adjacent sources from the non-Windows bootstrap target instead of masking them with fake SDK stubs.

The full foundation/support scaffold now builds on Linux:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Current result:

```text
[100%] Built target wwlib
```

Targets covered:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`
- `wwutil`
- `wwlib`

The `wwlib` result is compile-scaffold progress, not final portability. The deferred non-Windows source list marks future `platform`, `renderer`, input, registry/version, and string-conversion work that should be split deliberately.

See:

- `docs/build/foundation-portability-batch-004.md`
- `docs/architecture/foundation-target-map.md`

## Batch 005: `wwnet` source-boundary classification

Batch 005 models the original `Code/wwnet/wwnet.dsp` project as the next support dependency target. The first blocker was the expected WinSock boundary through headers such as:

```text
Code/wwnet/fromaddress.h -> <winsock.h>
Code/wwnet/packetmgr.h -> <winsock.h>
Code/wwnet/netutil.h -> <winsock.h>
```

The non-Windows scaffold now defers live socket/session sources while compiling the packet/stat/network-object side of `wwnet`:

```text
BWBalance.cpp, connect.cpp, netutil.cpp, packetmgr.cpp, rhost.cpp, singlepl.cpp
```

Targeted verification succeeds:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold --target wwnet -j4
```

Current result:

```text
[100%] Built target wwnet
```

See:

- `docs/build/foundation-portability-batch-005.md`
- `docs/architecture/foundation-target-map.md`

## Batch 006: source-island clarification

Batch 006 does not add a new target. It refactors the `wwlib` and `wwnet` CMake source inventory into named logical source islands while preserving original target names and file locations.

For `wwlib`, the scaffold now names foundation, platform, display/surface, input/message-loop, registry/version, string/encoding, content/archive, Windows integration, and diagnostic/mono-console islands.

For `wwnet`, the scaffold now names packet/object helpers, live socket/session sources, bandwidth/remote-host/session policy sources, and the narrow socket type bridge.

The expected build result is unchanged:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

```text
[100%] Built target wwnet
```

See:

- `docs/build/foundation-portability-batch-006.md`
- `docs/architecture/foundation-target-map.md`

## Batch 007: `BinkMovie` media/content seam

Batch 007 models the original `Code/BinkMovie/BinkMovie.dsp` target. The CMake inventory now names subtitle data/parser, subtitle renderer, and RAD/Bink playback source islands.

On non-Windows bootstrap builds, the scaffold compiles the subtitle data/parser island and defers the renderer subtitle manager plus RAD/Bink playback sources. This records the media/render seam without adding fake RAD SDK or renderer stubs.

Targeted verification succeeds:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold --target BinkMovie -j4
```

```text
[100%] Built target BinkMovie
```

See:

- `docs/build/foundation-portability-batch-007.md`
- `docs/architecture/foundation-target-map.md`

## Batch 009: `SControl` server-control/network seam

Batch 009 models the original `Code/SControl/SControl.dsp` target. The CMake inventory now names protocol and socket transport source islands:

```text
SCONTROL_PROTOCOL_SOURCES
SCONTROL_SOCKET_TRANSPORT_SOURCES
```

The first blocker was the expected WinSock include:

```text
Code/SControl/servercontrolsocket.h -> <winsock.h>
```

For this small target, the non-Windows scaffold compiles the real UDP transport using a narrow POSIX socket bridge local to `servercontrolsocket.h`. This does not introduce a general WinSock implementation.

Targeted verification succeeds:

```bash
cmake --build build/cmake-scaffold --target SControl -j4
```

```text
[100%] Built target SControl
```

See:

- `docs/build/foundation-portability-batch-009.md`
- `docs/architecture/foundation-target-map.md`

## Modernization rule for these blockers

Do not paper over these by globally renaming files or adding broad platform shims yet.

Handle them in small, documented waves:

1. Capture the blocker class.
2. Prefer compatibility wrappers or narrow conditional includes where behavior is clear.
3. Preserve original source/project membership.
4. Keep product parity and future Windows builds in view.

This keeps the baseline-first track separate from the later OpenW3D replay and physical code-bucket reorganization.
