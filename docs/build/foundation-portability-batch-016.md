# Batch 016: `Commando` dependency-prep documentation

## Goal

Prepare for the first `Code/Commando/commando.dsp` ingestion batch by documenting the already-known dependency and product-shell seams exposed by earlier subsystem seam probes.

This batch does **not** add a `commando` CMake target yet. It makes the next product-shell batch safer and more honest.

## Why this batch exists

The previous subsystem batches established that `commando` is now the next obvious target, but also that it sits above nearly every unresolved architectural seam already documented in the baseline scaffold:

- `Combat` still reaches `..\\commando\\datasafe.h`
- `Combat` still reaches Miles-backed `WWAudio` surfaces
- `Scripts` runtime core is separable, but DLL-entry and mission/toolkit body are still outside the current seam claim
- `ww3d2`, `wwui`, `WWAudio`, and `BinkMovie` still expose unresolved renderer/UI/audio/media backend boundaries

Without a prep pass, a direct `commando` ingestion would encourage exactly the wrong behavior: drag every unresolved seam into one giant target and then start papering over blockers with ad-hoc stubs.

## Main output

Added:

```text
docs/architecture/commando-dependency-prep.md
```

That document records:

- the original VC6 dependency shape of `commando`
- why `commando` must be treated as a product-shell target
- how `Combat` and `Scripts` currently block or complicate product-shell ingestion
- why client shell, FDS mode, online glue, config shell, and renderer/audio ownership must be split logically before any physical reorg
- recommended first-pass source-island vocabulary for a future `commando` scaffold

## Key conclusions captured

### `Commando` is above almost every remaining seam

The original dependency list includes:

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

That makes `commando` the first true product-shell ingestion target, not just another subsystem seam.

### `Combat` is not yet below the product shell

The earlier seam probe already showed:

```text
Code/Combat/damage.h:49:25: fatal error: ..\commando\datasafe.h: No such file or directory
```

So the first `commando` batch must explicitly account for reverse product-shell reach-back from `Combat`.

### `Scripts` runtime-core evidence is useful but incomplete

`Scripts` can build as a Linux shared-library probe for the runtime core, but the Windows DLL entry layer and the mission/toolkit content body are still outside the current claim. `commando` prep therefore has to distinguish runtime-script plumbing from the full script product surface.

### FDS remains a `commando` mode, not a separate original target

This prep batch reiterates that:

```text
FREEDEDICATEDSERVER
```

is a historical product mode layered onto `commando`, not a separate EA project. That matters because a later client/FDS split should still trace back to shared original source membership.

## Documentation updates in this batch

This batch updates status/roadmap docs so the project state is explicit:

- `README.md`
- `docs/architecture/modernization-progress-roadmap.md`
- `docs/architecture/foundation-target-map.md`
- `docs/architecture/original-project-scaffold-status.md`
- `docs/parity/target-status.md`

## What this batch intentionally does not do

- no new CMake target
- no `commando` build attempt
- no parity claim for client or FDS
- no fake SDK/header stubs
- no physical moves

## Resulting status

- `commando` is still **not modeled** as a build target.
- `commando` now has **dependency-prep documentation** describing how the first ingestion batch should be bounded.
- The default scaffold remains the same green set as before this batch.
