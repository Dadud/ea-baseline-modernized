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
```

Verified command:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Current successful result:

```text
[100%] Built target BinkMovie
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

- `WWAudio`
- `SControl`
- `Combat`
- `ww3d2`
- `wwphys`
- `wwui`
- `Scripts`
- `Commando`
- tool projects under `Code/Tools`

See `docs/architecture/original-project-scaffold-status.md` for the full project matrix.

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

### Latest batch completed: Batch 007 — model `BinkMovie`

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

After Batch 007, choose one low-risk expansion:

1. `SControl`
   - tiny static library;
   - useful for server-control/online-service classification;
   - final bucket is ambiguous.
2. `WWAudio`
   - important audio subsystem;
   - good candidate for future backend split;
   - likely to expose platform/audio SDK assumptions.
3. Continue status/inventory cleanup
   - update the 61-project matrix as each target is modeled;
   - refine current source-island docs before adding higher-coupling projects.

Avoid immediately jumping to:

- `Commando`
- `Combat`
- `ww3d2`
- `wwphys`
- `wwui`
- full tools workspace

Those are high-coupling buckets and should wait until more boundary mapping exists.

## Longer roadmap

1. Finish scaffold/status clarity for all original projects.
2. Introduce logical platform boundary docs and possibly interface targets.
3. Model small subsystem libraries before product shells.
4. Inventory `Combat` and `Commando` responsibilities before building them.
5. Define FDS/client/product split rules.
6. Build toward client/FDS/tools parity.
7. Only after baseline parity and architecture contracts are credible, replay OpenW3D changes in themed waves.

## Open questions

- Should `BinkMovie` or `WWAudio` be the next subsystem after scaffold cleanup?
- How much of `wwlib` should remain under `foundation` once platform/content/display pieces are split?
- Should socket abstraction be introduced before or after `Combat` modeling?
- What is the minimum useful artifact parity check for the first product build attempt?
