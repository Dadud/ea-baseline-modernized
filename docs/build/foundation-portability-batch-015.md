# Batch 015: `Scripts` DLL seam mapping

## Goal

Model the original EA `Code/Scripts/Scripts.dsp` target conservatively as an **opt-in script DLL seam**, keeping the default Linux bootstrap scaffold green while making the old script/plugin bucket's responsibilities explicit.

This batch does **not** claim Windows DLL parity or final script/plugin compatibility. It records the first credible ingestion/build evidence for the script runtime core without pretending the whole mission/toolkit DLL body is portable.

## Added CMake target

`Code/Scripts/CMakeLists.txt` now models the original VC6 project name unchanged:

```text
Scripts
```

The target is gated behind:

```text
RENEGADE_BUILD_SCRIPT_SEAMS
```

Example configure command:

```bash
cmake -S . -B build/cmake-scripts-probe \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_SCRIPT_SEAMS=ON
```

## Source-island classification

The original source inventory is grouped into these first-pass logical islands:

```text
SCRIPTS_DLL_ENTRY_SOURCES
SCRIPTS_RUNTIME_CORE_SOURCES
SCRIPTS_MISSION_TOOLKIT_SOURCES
SCRIPTS_LOCAL_SUPPORT_SOURCES
```

Interpretation:

- `SCRIPTS_DLL_ENTRY_SOURCES` — DLL entry/export/debug-print glue.
- `SCRIPTS_RUNTIME_CORE_SOURCES` — script registration, runtime script base classes, commands/events headers, and registrar/factory plumbing.
- `SCRIPTS_MISSION_TOOLKIT_SOURCES` — actual mission/test/toolkit gameplay script content.
- `SCRIPTS_LOCAL_SUPPORT_SOURCES` — local copied support headers/helpers that mirror bits of the game-side API and math/string helpers.

## Non-Windows handling

On non-Windows seam probes, the scaffold keeps the full source inventory visible but defers:

```text
SCRIPTS_DLL_ENTRY_SOURCES
SCRIPTS_MISSION_TOOLKIT_SOURCES
```

That lets the probe focus on whether the script runtime core itself is separable from Windows DLL entrypoints and the large body of mission/toolkit content.

## Narrow fixes applied before trusting the result

The initial probe exposed a set of Linux include-identity and compiler-spelling issues before the runtime core could even be tested. These were corrected as narrow, behavior-preserving fixes:

- `Windows.H` -> `windows.h`
- `slnode.h` -> `ScriptNode.h`
- `scriptregistrar.h` -> `ScriptRegistrar.h`
- `scriptfactory.h` -> `ScriptFactory.h`
- `scriptregistrant.h` -> `ScriptRegistrant.h`
- `dprint.h` -> `DPrint.h`
- added non-MSVC `__cdecl` compatibility in `DPrint.h`
- added non-MSVC `stricmp` mapping in `ScriptRegistrar.cpp` and `scripts.cpp`
- added non-MSVC `_declspec(...)` no-op compatibility in `scriptevents.h` and `scriptcommands.h`

These fixes are scaffolding-level compatibility work only; they do not claim the Windows DLL ABI or export model is finalized.

## Probe result

Targeted probe command:

```bash
cmake --build build/cmake-scripts-probe --target Scripts -j4
```

Current result:

```text
[100%] Built target Scripts
```

On Linux this currently produces a shared library form (`libScripts.so`) for seam-probing purposes. That is **not** a parity claim against the original Windows outputs:

```text
Run/Scripts.dll
Run/ScriptsD.dll
Run/ScriptsP.dll
```

## Architectural conclusion

The key result is that the `Scripts` bucket is now separated into:

- Windows DLL entry/export glue
- reusable script runtime/registration core
- mission/toolkit content body
- local copied helper/support layer

That is useful because it shows the runtime script/plugin core is more separable than `Combat`, while the mission/toolkit content and Windows DLL boundary should still be treated as distinct seams.

## What this batch intentionally does not do

- no claim of Windows DLL parity
- no attempt to load the script DLL into a running game shell
- no mission/toolkit content portability claim
- no physical source moves
- no claim of client/FDS parity

## Resulting status

- `Scripts` is now **modeled**.
- `Scripts` is **not** part of the default green scaffold.
- `Scripts` currently has **portable runtime-core build evidence** for the current phase, with DLL-entry and mission/toolkit content still explicitly outside that claim.
