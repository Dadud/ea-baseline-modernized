# `Commando` Dependency-Prep Notes

## Goal

Document the specific dependency seams that must be described before attempting to ingest `Code/Commando/commando.dsp` into the modernization scaffold.

This is a **pre-ingestion planning document**, not a build claim. It exists so the first `commando` pass can be honest about product-shell boundaries instead of degenerating into a giant source-list import plus fake SDK/header shims.

## Why `Commando` is different

`commando` is not just another target. It is the original EA product shell that assembles the client executable and, through `FREEDEDICATEDSERVER`, also acts as the baseline dedicated-server product mode.

From `docs/architecture/original-project-inventory.md`, the original VC6 dependency shape is:

```text
Scripts
wwdebug
wwlib
wwmath
wwnet
wwphys
wwutil
WWAudio
wwsaveload
Combat
wwtranslatedb
wwbitpack
ww3d2
wwui
BinkMovie
BandTest
SControl
GameSpy
```

That means `commando` sits above nearly every seam already mapped:

- foundation/support (`wwdebug`, `wwmath`, `wwbitpack`, `wwsaveload`, `wwtranslatedb`, `wwlib`, `wwutil`)
- networking/control (`wwnet`, `SControl`)
- gameplay/runtime (`Combat`)
- audio (`WWAudio`)
- renderer (`ww3d2`)
- UI/input (`wwui`)
- media (`BinkMovie`)
- scripts/plugin DLL (`Scripts`)
- online/service/test/product glue (`BandTest`, `GameSpy`)

So the first `commando` batch should be treated as **product-shell dependency prep**, not as a realistic portable build milestone.

## Product outputs that must stay explicit

Original `commando.dsp` product outputs:

```text
Run/RenegadeD.exe
Run/Renegade.exe
Run/RenegadeP.exe
```

Original dedicated-server relationship:

```text
FREEDEDICATEDSERVER
```

Important implication: the EA baseline does **not** have a separate `renegadeserver.dsp`. The first modern `commando` ingestion needs to preserve the fact that:

- client and FDS originate from the same historical project family
- FDS is a define/configuration variant, not a separate original source tree
- any future target split must still trace back to the original shared source membership

## Known upstream dependency-prep findings from recent seam batches

### 1. `Combat` is still coupled back into the product shell

The `Combat` seam probe reached:

```text
Code/Combat/damage.h:49:25: fatal error: ..\commando\datasafe.h: No such file or directory
```

Additional direct includes exist in:

```text
Code/Combat/damage.h
Code/Combat/playerdata.h
Code/Combat/weaponmanager.h
```

This matters because it proves the current historical layering is not:

```text
Combat -> Commando
```

but instead partially circular in practice:

```text
Commando depends on Combat
Combat headers also reach back into Commando
```

### 2. `Combat` still reaches proprietary audio/backend surfaces

The `Combat` seam probe also reached:

```text
Code/WWAudio/AudibleSound.h:45:10: fatal error: mss.h: No such file or directory
```

This means `commando` ingestion cannot pretend gameplay/runtime is already independent of the Miles-backed `WWAudio` path.

### 3. `Scripts` runtime core is more separable than its product boundary

The `Scripts` seam probe can build a Linux shared-library runtime-core probe, but that still excludes:

```text
SCRIPTS_DLL_ENTRY_SOURCES
SCRIPTS_MISSION_TOOLKIT_SOURCES
```

So `commando` dependency prep must distinguish:

- script runtime registration core
- Windows DLL entry/export boundary
- mission/toolkit content body
- in-game script command/event surface

`commando` almost certainly depends on more than just the runtime core.

### 4. Renderer/UI/audio seams are still intentionally unresolved

Previously mapped seam probes already show that higher product layers are still blocked by:

- `ww3d2` -> `d3d8.h`, `wwlib/win.h`
- `wwui` -> `Render2D.h`, `<imm.h>`, Win32 callback/handle assumptions
- `WWAudio` -> `mss.h`, `Mss.H`
- `BinkMovie` -> deferred renderer subtitle glue and RAD/Bink playback sources

So a first `commando` pass must not imply those backend boundaries are already solved.

## Required dependency-prep split for the first `commando` ingestion

Before adding `Code/Commando/CMakeLists.txt`, treat the project as four overlaid concerns:

### A. Product shell and startup

Examples to expect:

- process startup/shutdown
- main loop ownership
- configuration/bootstrap
- client vs FDS branding/mode switching
- crash/reporting/launcher/update glue

This is the core reason `commando` belongs under `products`, not under `engine_runtime`.

### B. Client presentation and frontend shell

Examples to expect:

- menu/frontend startup
- UI ownership and wwui orchestration
- renderer ownership and video/init paths
- client-facing shell logic

This should remain separate from any future FDS target plan.

### C. Dedicated-server mode overlays

We already know from the baseline docs that `FREEDEDICATEDSERVER` changes behavior in:

- `Code/Combat/*`
- `Code/Commando/*`
- `Code/BandTest/*`
- `Code/Launcher/*`

So the first `commando` ingestion should explicitly inventory:

- files that compile differently under `FREEDEDICATEDSERVER`
- files that are client-only regardless of the define
- files that are server-only in effect even if historically mixed

### D. Online/service/product glue

Because `commando` depends on `BandTest`, `SControl`, and `GameSpy`, a first pass must distinguish:

- low-level runtime networking already in `wwnet`
- dedicated-server control in `SControl`
- legacy service integrations (`GameSpy`, likely WOL-adjacent code elsewhere)
- product-specific online shell behavior in `Commando`

## Recommended first-pass source-island vocabulary for `Commando`

Do not commit to exact source membership yet, but the first ingestion should probably classify toward something like:

```text
COMMANDO_PRODUCT_STARTUP_SOURCES
COMMANDO_CLIENT_FRONTEND_UI_SOURCES
COMMANDO_CLIENT_RUNTIME_ORCHESTRATION_SOURCES
COMMANDO_FDS_MODE_SOURCES
COMMANDO_ONLINE_SERVICE_GLUE_SOURCES
COMMANDO_CONFIG_SHELL_SOURCES
COMMANDO_RENDER_AUDIO_OWNERSHIP_SOURCES
```

This keeps the first `commando` pass aligned with the architectural reality already uncovered in `Combat`, `Scripts`, `wwui`, `ww3d2`, and `WWAudio`.

## What the first `commando` batch should verify

A good first `commando` batch should answer these questions, in order:

1. What source islands exist in the original VC6 project?
2. Which of those are obviously client shell vs FDS mode vs shared orchestration?
3. Which `Combat` and `Scripts` surfaces are pulled directly by `Commando`?
4. Which sources require unresolved third-party SDKs immediately?
5. Which sources are blocked by renderer/UI/audio/platform seams rather than local include-case issues?
6. Can `commando` be modeled as an opt-in seam target without breaking the default scaffold?

## What the first `commando` batch should *not* do

- no claim of client parity
- no claim of FDS parity
- no fake `GameSpy` project/source replacement
- no fake Direct3D, Miles, IME, or Bink headers just to deepen the compile frontier
- no physical file moves
- no premature split into final client/FDS executables before source-island evidence exists

## Immediate follow-up recommendation

The next implementation batch after this prep doc should be:

1. inspect `Code/Commando/commando.dsp`
2. generate a conservative `Code/Commando/CMakeLists.txt`
3. classify first-pass source islands only
4. keep `commando` opt-in
5. run a targeted probe
6. document first blockers honestly

That would make the next `commando` batch an ingestion-and-boundary batch, not a fake product-porting batch.
