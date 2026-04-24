# Modernization Progress Review and Roadmap

## Current status

The modernization has successfully pivoted from mixed-history Linux build firefighting to an EA-baseline-first program.

Current baseline worktree:

```text
/tmp/openw3d-ea-baseline
```

Current branch and remote target:

```text
ea-baseline-modernized -> ea-modernized/main
```

Current latest reviewed commit before Batch 007:

```text
43b647c build: classify support source islands
```

Batch 007 then models `BinkMovie` as the next small media/content seam, compiling its subtitle data/parser island while documenting deferred RAD/Bink and renderer glue islands.

## What is now solid

### Baseline and project inventory

- The selected baseline is the original EA source-release lineage at `3e00c3a1b97381bb28be89a35b856375e0629a08`.
- Original VC6 `.dsw` / `.dsp` project metadata is parsed into a machine-readable manifest.
- The manifest currently records 61 projects:
  - 16 static libraries
  - 10 DLLs
  - 10 console applications
  - 25 GUI applications
- `Code/commando.dsw` and `Code/tools.dsw` are the primary original workspaces.
- FDS is documented as a `commando` build mode, not a standalone `.dsp`.

### Build scaffold

The current conservative CMake scaffold builds these original EA targets on Linux:

```text
wwdebug
wwmath
wwbitpack
wwsaveload
wwtranslatedb
wwlib
wwutil
wwnet
BinkMovie
SControl
```

Verified command:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Current successful result:

```text
[100%] Built target SControl
```

### Architecture findings

The main progress is not just that the scaffold builds. The important architectural result is that the old code buckets are being classified without hiding seams.

- `wwlib` is a mixed foundation/platform/display/input/content bucket, not a clean foundation library.
- `wwnet` is a mixed packet/object networking plus live WinSock/session bucket.
- `BinkMovie` is a small media/content bucket with subtitle parsing, subtitle renderer glue, and RAD/Bink playback concerns split apart.
- DirectDraw, WinSock, RAD/Bink, and renderer blockers are being treated as future boundary work, not fake-header problems.
- Logical boundaries are being documented before physical file movement.

## What is still missing

### Product builds

No final product is built yet:

- client
- FDS/dedicated server
- tools
- script DLLs

### Parity proof

No artifact parity has been proven yet. The current scaffold is compile-checkable infrastructure, not parity completion.

### Major subsystems not yet modeled

Important buckets still need scaffold modeling and boundary review:

- tool projects under `Code/Tools`
- `BandTest`
- service/online projects beyond the currently documented seams

See `docs/architecture/original-project-scaffold-status.md` for the full project matrix.

Use `docs/parity/target-status.md` when you need the shorter answer to three separate questions:

- is a target modeled?
- is it part of the default green scaffold?
- does it have any real parity evidence yet?


### Real platform abstraction

Current non-Windows compatibility is intentionally narrow. Still needed:

- filesystem/path boundary
- timer boundary
- threading/synchronization boundary
- sockets/network boundary
- process/window/shell boundary
- registry/config/version boundary
- string/encoding boundary

## Updated plan

### Latest batch completed: Batch 026 — opt-in SDL3 version/file metadata seam for `wwlib`

Batch 026 extends the first SDL3 platform replay into `Code/wwlib/verchk.h` and `Code/wwlib/verchk.cpp`. The seam is intentionally partial: when `RENEGADE_USE_SDL3=ON`, `verchk.cpp` now builds on non-Windows with portable PE image-header timestamp comparison and host-file `FILETIME` synthesis, while preserving the original Win32 path for real PE version-resource extraction.

This batch does **not** claim full parity for Win32 version/resource semantics. `GetVersionInfo()` still returns no version-resource data on non-Windows, and `GetFileCreationTime()` may fall back to file modification time on platforms without birth-time metadata. The purpose of this batch is to make the version/file-metadata seam compile and become explicitly documented, not to overclaim finished parity.

CMake now also re-enables `verchk.cpp` in the UNIX `wwlib` build only when `RENEGADE_USE_SDL3=ON`.

Verified result:

```bash
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
```

with final output including:

```text
[100%] Built target wwlib
```

### Previous batch completed: Batch 025 — opt-in SDL3 resource-file seam for `wwlib`

Batch 025 continues the SDL3 platform replay from the wider OpenW3D fork ecosystem by adapting the `rcfile.*` donor seam into a baseline-friendly form. `Code/wwlib/rcfile.h` and `Code/wwlib/rcfile.cpp` now support an opt-in SDL3-ready non-Windows `ResourceFileClass` path under `RENEGADE_USE_SDL3=ON`, while preserving the original Windows PE-resource path.

This batch intentionally does **not** adopt the full generated embedding pipeline from the donor branch. Instead, it introduces a conservative C++98-friendly static in-memory registry (`std::map<std::string, StaticResourceFileClass>`) so the seam builds cleanly now and can be populated by later batches.

CMake now also re-enables `rcfile.cpp` in the UNIX `wwlib` build only when `RENEGADE_USE_SDL3=ON`.

Verified result:

```bash
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
```

with final output including:

```text
[100%] Built target wwlib
```

This batch still does **not** claim generated embedded-resource population, `embed.py` / `embed.cmake` integration, or product-level resource registration units.

### Previous batch completed: Batch 024 — opt-in SDL3 raw file seam for `wwlib`

Batch 024 continues the SDL3 platform replay from the wider OpenW3D fork ecosystem, again using w3dhub PRs #105 and #107 as the donor reference. This batch implements the filesystem half of the first SDL3 platform seam: `Code/wwlib/wwfile.h`, `Code/wwlib/rawfile.h`, and `Code/wwlib/rawfile.cpp` now support an opt-in SDL3-backed non-Windows raw file path under `RENEGADE_USE_SDL3=ON`, while preserving the existing Windows path and the legacy non-SDL3 UNIX fallback code.

CMake now also re-enables `rawfile.cpp` in the UNIX `wwlib` build only when `RENEGADE_USE_SDL3=ON`.

Verified result:

```bash
cmake -S . -B build/cmake-scaffold-sdl3 \
  -DRENEGADE_USE_SDL3=ON \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_PLATFORM_LIBS=ON
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
```

with final output including:

```text
[100%] Built target wwlib
```

This batch still does **not** claim full SDL3 resource embedding or `rcfile.*` replay. Resource-file handling remains deferred to a follow-on resource batch.

### Previous batch completed: Batch 023 — opt-in SDL3 thread seam for `wwlib`

Batch 023 starts the SDL3 platform replay using the SDL3 work found in the wider OpenW3D fork ecosystem, especially w3dhub PRs #105 and #107. This first bounded batch intentionally adopts only the highest-confidence seam: `Code/wwlib/thread.h` and `Code/wwlib/thread.cpp` now support an opt-in SDL3-backed non-Windows thread path under `RENEGADE_USE_SDL3=ON`, while preserving the original Windows path and the existing `ThreadClass` interface shape.

CMake now exposes:

- `RENEGADE_USE_SDL3=ON` — opt-in SDL3-backed platform seam pieces where implemented
- `PkgConfig::SDL3` wiring for `wwlib`

Verified result:

```bash
cmake -S . -B build/cmake-scaffold-sdl3 \
  -DRENEGADE_USE_SDL3=ON \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_PLATFORM_LIBS=ON
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
```

with final output including:

```text
[100%] Built target wwlib
```

This batch does **not** yet claim full SDL3 filesystem/resource integration. `rawfile.cpp`, `rcfile.*`, and broader SDL3 helper adoption remain separate follow-on work.

### Previous batch completed: Batch 019 — automation tooling and multi-target probe

Batch 019 does not add a new source target. It adds two automation scripts and runs the first structured multi-target probe across all unmapped subsystems.

**Scripts added:**

- `scripts/projects/write_cmake_scaffold.py` — generates CMake scaffolds from the VC6 manifest for any target in one command
- `scripts/build/probe_targets.py` — probes targets using signal-pattern scanning and optional CMake build, outputs Markdown report

**Probe coverage:**

Ran structured probe against: WWAudio, SControl, Combat, ww3d2, wwphys, wwui, Scripts.

Key findings:
- WWAudio is the most threading-heavy target (127 hits), all tied to Miles/DirectSound backends
- wwui has the highest string/encoding signal (158 hits) — expected for a UI text system
- ww3d2 has the highest DirectX signal (133 hits) and 5 missing files (shader/resource files)
- Combat has 265 sources, the largest gameplay bucket, with moderate portability risk
- SControl is purely WinSock transport (60 signal hits, 4 source files)
- Scripts has 1 missing file (`slnode.h`) and 99 old-MSVC spelling hits

These scripts make future batches faster and more systematic by automating scaffold generation and deferred-source tracking.

### Previous batch completed: Batch 021 — probe and model Scripts, BandTest, MakeMix

Batch 021 used the automation tooling to probe three targets simultaneously. Results: Scripts built `libScripts.so` with only warnings — no compatibility patches needed. BandTest is entirely WinSock2/Win32 (84 signal hits) — classified as a WinSock/network transport seam, deferred. MakeMix is entirely Win32 `FindFirstFile`/`WIN32_FIND_DATA` (84 signal hits) — classified as a platform/filesystem seam, deferred. The MakeMix `StdAfx.h` vs `stdafx.h` case mismatch was fixed. A `MixFile.h` symlink was added to `Code/wwlib`. No CMakeLists.txt was committed for BandTest or MakeMix. Scripts remains opt-in under `RENEGADE_BUILD_SCRIPT_SEAMS=ON`.

### Previous batch completed: Batch 020 — probe WWAudio; classify as Miles audio seam

Batch 020 attempted to model WWAudio using the automation tooling. Generated a CMakeLists.txt and attempted to build — failed. The failure confirmed the probe signal data: even the handle facade types (`Sound2DHandleClass`, `Sound3DHandleClass`, `SoundStreamHandleClass`) depend on `WWAudio.h` which unconditionally includes `Mss.H`. There is no portable C++ subset within WWAudio without the Miles Sound System SDK. WWAudio is classified as a third-party SDK seam alongside RAD/Bink and DirectX 8. No CMakeLists.txt was committed. Scaffold remains 10/10 green.

### Previous batch completed: Batch 018 — datasafe Win32 gating + Commando dependency mapping

Batch 018 does two things:

1. **datasafe Win32 gating**: Adds narrow non-Windows compatibility to `datasafe.h` and `datasafe.cpp`. The `safe_int`/`safe_float` encrypted storage types are pure C++ — their thread-safety mutex path (`HANDLE`, `CreateMutex`, `GetCurrentThreadId`, `CloseHandle`) is now gated under `#ifdef _WIN32`. Non-Windows gets `PreferredThread = 0` as a no-op. This unblocks the `Combat` → `datasafe` chain for Linux scaffold compilation without faking the data-safe architecture.

2. **Commando probe and path/case fixes**: Batch-fixed 87 Commando files with Windows backslash include paths (→ forward slashes), 10 files with `WWAudio.H` case issue (→ `WWAudio.h`), and created minimal stubs for genuinely missing EA baseline headers (`gamespybanlist.h`, `WWLib/Notify.h`, `WWLib/Signaler.h`, `WWOnline/RefPtr.h`, `WWOnline/WOLLangCodes.h`, `WWUI/MenuDialog.h`, `WWUI/PopupDialog.h`, `WOLGameInfo.h`, `AudibleSound.h`). Gated `winsock.h` in `wwnet/rhost.h` under `_WIN32`.

The Commando probe revealed that `Commando` is deeply coupled to EA's closed-source WWOnline/WWLib/WWUI framework (event-driven session management, dialog UI, reference-counted smart pointers) and WOLBrowser (COM/IDL ActiveX control with RPC dependencies). This is classified as a **product shell seam** — not buildable on non-Windows without substantial framework reconstruction or behavior-changing stubs.

### Previous batch completed: Batch 016 — `Commando` dependency prep

Batch 016 does not ingest `commando` yet. Instead it adds `docs/architecture/commando-dependency-prep.md` to record the original VC6 dependency shape, the already-known `Combat` and `Scripts` product-shell complications, and the first-pass source-island vocabulary that should be used for a bounded `commando` ingestion batch.

### Previous batch completed: Batch 015 — map `Scripts`

Batch 015 adds an opt-in scaffold for the original `Scripts` DLL target and classifies it into DLL entry, runtime core, mission/toolkit content, and local support islands. It intentionally distinguishes portable runtime-core build evidence from the still-unproven Windows DLL boundary and mission content body.

### Previous batch completed: Batch 014 — map `Combat`

Batch 014 adds an opt-in scaffold for the original `Combat` target and classifies it into gameplay/runtime, client presentation/input, script/save/network, and audio/conversation source islands. It intentionally documents product-shell and audio-backend leakage instead of pulling in `Commando` or faking Miles headers.

### Previous batch completed: Batch 013 — map `ww3d2`

Batch 013 adds an opt-in scaffold for the original `ww3d2` target and classifies it into asset/content, render runtime, presentation, texture/material, and DX8 backend source islands. It intentionally documents mixed renderer/platform/asset-content leakage instead of pushing fake D3D8, D3DX, or Win32 handle shims.

### Previous batch completed: Batch 012 — map `wwui`

Batch 012 adds an opt-in scaffold for the original `wwui` target and classifies it into dialog/control, input/cursor, and IME source islands. It intentionally documents mixed UI/input/platform/renderer leakage instead of pushing fake IME, Win32, or renderer shims.

### Previous batch completed: Batch 011 — map `wwphys`

Batch 011 adds an opt-in scaffold for the original `wwphys` target and classifies it into path/visibility, runtime simulation, math/serialization, scene/render glue, and terrain/render source islands. It intentionally documents mixed runtime/renderer/platform leakage instead of pushing broad Win32 or renderer stubs.

### Previous batch completed: Batch 010 — map `WWAudio`

Batch 010 adds an opt-in scaffold for the original `WWAudio` target and classifies it into event, save/load, scene metadata, threading, utility, and Miles backend source islands. It intentionally documents the Miles/DirectSound backend blocker instead of adding fake Miles headers.

### Previous batch completed: Batch 009 — model `SControl`

Batch 009 adds the original `SControl` target as a small server-control/network seam. CMake now identifies:

- server-control protocol sources;
- UDP socket transport sources.

The non-Windows scaffold compiles both islands using a narrow POSIX socket bridge local to `servercontrolsocket.h`, while preserving the original WinSock path under `_WIN32`.

### Previous batch completed: Batch 008 — automation and target probes

Batch 008 added the scaffold generator/probe scripts and produced the first multi-target probe report:

- `scripts/projects/write_cmake_scaffold.py`
- `scripts/build/probe_targets.py`
- `docs/build/target-probe-report-batch-008.md`

### Previous batch completed: Batch 007 — model `BinkMovie`

Batch 007 adds the original `BinkMovie` target as a small media/content seam. CMake now identifies:

- subtitle data/parser sources;
- subtitle renderer glue sources;
- RAD/Bink playback sources.

On non-Windows, only the subtitle data/parser island is compiled for now. Renderer subtitle glue and RAD/Bink playback remain visible but deferred.

### Previous batch completed: Batch 006 — consolidate and clarify the scaffold

Batch 006 turns the current `wwlib` and `wwnet` source filters into named logical source islands.

For `wwlib`, CMake now identifies:

- foundation-like sources;
- platform sources;
- display/surface sources;
- input/message-loop sources;
- registry/version sources;
- string/encoding sources;
- content/archive sources;
- Windows integration sources;
- diagnostic/mono-console sources.

For `wwnet`, CMake now identifies:

- packet/stat/object helpers;
- live socket/session sources;
- bandwidth/remote-host/session policy sources;
- socket type bridge headers.

Original target names and physical file locations remain unchanged.

### Following batch candidates

After Batch 017, choose one bounded expansion:

1. deeper `commando` source pass
   - refine first-pass source-island membership, especially where config-shell and runtime orchestration still collapse into `datasafe.h` / Win32 product ownership.
2. `BandTest` or another smaller DLL/tool seam
   - continue dynamic-library and dedicated-server-adjacent ingestion patterns without immediately claiming product builds.
3. tools workspace pilot target
   - pick a smaller console/tool target such as `bin2cpp`, `VerStamp`, or `MakeMix` to start proving the tools side of the baseline.

Avoid immediately jumping to:

- full `commando` product build attempts
- full tools workspace
- parity claims for client/FDS/tools/script DLL deployment

Those remain high-coupling buckets and should wait until the product-shell boundaries are described more clearly.

## Longer roadmap

1. Finish scaffold/status clarity for all original projects.
2. Introduce logical platform boundary docs and possibly interface targets.
3. Model small subsystem libraries before product shells.
4. Inventory `Combat` and `Commando` responsibilities before building them.
5. Define FDS/client/product split rules.
6. Build toward client/FDS/tools parity.
7. Only after baseline parity and architecture contracts are credible, replay OpenW3D changes in themed waves.

## Open questions

- What parts of `datasafe.h` are truly product-shell-owned versus runtime-safe state/locking infrastructure?
- How much of `Commando` runtime orchestration can move below the product shell once `datasafe.h` ownership is classified?
- Should the next bounded seam be `BandTest` as a dedicated-server-adjacent DLL/service step or a small tools pilot target?
- What is the minimum useful artifact parity check for the first product build attempt?
