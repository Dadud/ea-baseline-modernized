# OpenW3D EA-Baseline Modernization: Progress Review and Updated Plan

## Goal

Step back from the per-batch compile work, summarize where the EA-baseline modernization stands, and define the next ordered work plan without losing the baseline-first strategy.

The current goal is still:

1. modernize the original EA Renegade source baseline first;
2. preserve client/FDS/tools parity expectations;
3. document the old code buckets and future architecture before moving files;
4. only later replay OpenW3D fork changes onto a cleaner, proven baseline.

## Current repository state

- Worktree: `/tmp/openw3d-ea-baseline`
- Branch: `ea-baseline-modernized`
- Remote target: `ea-modernized/main`
- Latest pushed commit reviewed: `ba7d928 build: model wwnet support target`
- Baseline source: EA `CnC_Renegade` commit `3e00c3a1b97381bb28be89a35b856375e0629a08`

## Progress so far

### 1. Strategy and baseline are established

Completed:

- Archived earlier mixed OpenW3D/Linux exploration instead of discarding it.
- Created a clean EA baseline worktree.
- Documented baseline selection, artifact parity expectations, and the current-to-future architecture map.
- Chose a 9-bucket long-term architecture:
  - `foundation`
  - `platform`
  - `engine_runtime`
  - `engine_asset_content`
  - `renderer`
  - `audio`
  - `input_and_control`
  - `online_services`
  - `products`

What this means:

- We are no longer doing random Linux whack-a-mole on a mixed-history branch.
- The EA tree is being turned into a documented, reproducible modernization baseline.
- OpenW3D fork replay remains intentionally deferred.

### 2. Original VC6 project structure is captured

Completed:

- Parsed original `.dsw` / `.dsp` metadata into:
  - `build-manifests/vc6-projects.json`
  - `build-manifests/vc6-projects.md`
- Documented the original project inventory.
- Confirmed the primary product facts:
  - `Code/commando.dsw` is the main game workspace.
  - `Code/tools.dsw` is the main tools workspace.
  - `Code/Commando/commando.dsp` is the main client product project.
  - FDS is not a separate `.dsp`; it is an alternate `commando` build mode via `FREEDEDICATEDSERVER`.

Current manifest shape:

- 61 projects total.
- 16 static libraries.
- 10 DLLs.
- 10 console apps.
- 25 GUI apps.

### 3. Conservative CMake scaffold exists and builds a useful support layer

Current successful Linux scaffold command:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Current built targets:

```text
wwdebug
wwmath
wwbitpack
wwsaveload
wwtranslatedb
wwlib
wwutil
wwnet
```

This is a meaningful milestone: the baseline now has a compile-checkable support/network-adjacent layer under modern CMake while preserving original EA target names.

### 4. Important architectural seams are now documented, not hidden

The best outcome of Batches 003-005 is not just that more code compiles. It is that major historical buckets are now classified honestly.

#### `wwlib`

Finding:

- `wwlib` is not a clean foundation library.
- It contains true utility/foundation code, but also:
  - platform code;
  - registry/version code;
  - Windows shell/browser integration;
  - mono/debug console code;
  - DirectDraw/display/surface code;
  - input/message-loop code;
  - legacy string/file/archive code needing dedicated review.

Current non-Windows scaffold defers these source islands instead of faking DirectDraw or broad Win32 behavior.

#### `wwnet`

Finding:

- `wwnet` is the first real networking seam.
- It mixes packet/stat/network-object helpers with live WinSock/session/socket code.

Current non-Windows scaffold builds the packet/object side and defers live socket/session sources:

```text
BWBalance.cpp
connect.cpp
netutil.cpp
packetmgr.cpp
rhost.cpp
singlepl.cpp
```

A narrow `socket_compat.h` exists only to expose type names used by shared headers; it is not a network implementation.

## What is still not done

### Not done: product parity

We do not yet build any final EA product:

- client `Renegade.exe`
- FDS/server mode
- tools/editors/viewers
- script DLLs

No artifact parity has been proven yet. The current work is a build-graph and architectural-boundary foundation for that later proof.

### Not done: real platform abstraction

Current non-Windows bridges are deliberately narrow and temporary. Still needed:

- file/path API boundary;
- time/timer boundary;
- threading/mutex/event boundary;
- sockets/network boundary;
- process/window/shell integration boundary;
- registry/config/version boundary;
- encoding/string boundary.

### Not done: renderer/audio/input/online/tooling layers

Still untouched or only mapped at planning level:

- `ww3d2` renderer and Direct3D-era backend assumptions;
- `WWAudio` audio subsystem;
- `wwui` and input paths;
- `wwphys` physics/render hybrid;
- WOL/GameSpy/online service code;
- Tools/LevelEdit/W3DView/etc.;
- `Combat` and `Commando` product/runtime split.

### Not done: physical reorganization

No major file moves should happen yet. Logical boundaries through CMake and docs need to stay ahead of physical repo churn.

## Recommended next plan

### Phase A — Consolidate current scaffold quality before expanding too far

Goal: turn the current support-layer scaffold from “works” into “well described and safer to extend.”

1. Add a generated target status summary document or script output showing:
   - all 61 original projects;
   - whether each has a CMake scaffold;
   - future bucket classification;
   - current build status.
2. Split `wwlib` deferred source list into named logical CMake variables, for example:
   - `WWLIB_FOUNDATION_SOURCES`
   - `WWLIB_PLATFORM_SOURCES`
   - `WWLIB_DISPLAY_SURFACE_SOURCES`
   - `WWLIB_INPUT_MESSAGE_SOURCES`
   - `WWLIB_REGISTRY_VERSION_SOURCES`
   - `WWLIB_ENCODING_STRING_SOURCES`
3. Do the same for `wwnet`:
   - packet/object/stat helpers;
   - live socket/session sources;
   - bandwidth/remote-host/session policy sources.
4. Keep target names unchanged for now.
5. Verify the full current scaffold still builds.

Why first:

- It reduces future confusion.
- It turns “filtered source list” into explicit architecture information.
- It prepares for real `platform` extraction without moving files prematurely.

### Phase B — Model one more low-risk support target, not products

Recommended candidates, in order:

1. `BinkMovie`
   - Small static library.
   - Useful because it touches client/content/media boundaries without pulling the whole renderer.
   - Risk: Bink SDK assumptions may appear.
2. `WWAudio`
   - Important subsystem bucket.
   - Likely platform/backend-heavy but smaller than renderer or `Combat`.
   - Good early audio architecture mapping candidate.
3. `SControl`
   - Very small static library.
   - But its final home is ambiguous: server product vs online/service/control layer.

Avoid for the next immediate batch:

- `Combat` — too large and mixed.
- `Commando` — product shell and FDS/client split should wait.
- `ww3d2` — renderer/backend boundary should wait until platform/content support is clearer.
- `wwphys` — likely render/runtime hybrid.
- `wwui` — client UI/input/IME-heavy.
- Tools — too many product/tool-specific dependencies before shared content/runtime boundaries exist.

### Phase C — Build platform boundary intentionally

Once `wwlib`/`wwnet` classification is clearer, introduce a logical platform plan before broad code changes:

1. Document required platform services:
   - filesystem/path;
   - time;
   - threading/sync;
   - sockets;
   - process/window/shell;
   - registry/config/version.
2. Decide whether to model temporary CMake interface targets such as:
   - `renegade_platform_headers`;
   - `renegade_platform_filesystem`;
   - `renegade_platform_network`.
3. Keep the old sources in place while routing new code through logical target boundaries.

### Phase D — Prepare for runtime/product modeling

Before adding `Combat` or `Commando`, create docs for:

1. `Combat` responsibility inventory:
   - shared gameplay;
   - client presentation;
   - input/control;
   - render/audio dependencies;
   - server-safe pieces.
2. `Commando` product-mode inventory:
   - client shell;
   - FDS/server mode;
   - shared app bootstrap;
   - online/WOL/GameSpy/NAT glue;
   - frontend/UI/config/input glue.
3. FDS build-mode rules based on `FREEDEDICATEDSERVER`.

Only then start modeling `Combat`/`Commando` CMake targets.

### Phase E — Later OpenW3D replay

Do not replay fork changes yet.

Before replaying, we need:

- baseline scaffold for major original buckets;
- documented product split plan;
- initial platform boundary plan;
- known third-party SDK gaps;
- at least a rough client/FDS/tools parity strategy.

Replay order later should be themed:

1. build/devex improvements;
2. audio/backend modernization;
3. platform abstraction work;
4. networking/online changes;
5. renderer/backend changes;
6. Linux/headless/server enablement.

## Immediate next batch recommendation

Best next batch: **Batch 006 — classify scaffold status and split current mixed source lists into named logical islands.**

Deliverables:

- update docs with a 61-project scaffold status matrix;
- refactor `Code/wwlib/CMakeLists.txt` source lists into named logical source groups;
- refactor `Code/wwnet/CMakeLists.txt` source lists into named logical source groups;
- keep generated/original target names unchanged;
- verify full scaffold still builds;
- commit and push.

This is less glamorous than adding another target, but it will make the next expansion much cleaner and aligns with the user's requirement that modernization reorganize every code bucket into something easier to understand, not just make the build pass.

## Success criteria for Batch 006

- No physical file moves.
- No new broad platform abstraction yet.
- No product target modeling yet.
- Full scaffold still builds:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

- Documentation answers:
  - what is already modeled;
  - what is not modeled;
  - what bucket each original project probably belongs to;
  - where the biggest seams still are.
