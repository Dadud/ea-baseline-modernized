# OpenW3D Modernization Strategy v2

## Purpose

This document updates the EA-baseline modernization strategy after the first scaffold batches proved that the original project graph is useful, but manual target-by-target work will not scale to the larger mixed subsystems.

The goal does **not** change: modernize from the original EA Renegade baseline first, preserve client/FDS/tools parity, then replay OpenW3D changes onto a cleaner and better-documented architecture.

The workflow does change: future work should become more manifest-driven, probe-driven, and ledger-backed so scaffold build success does not hide unresolved architecture seams.

## Current state

The current Linux bootstrap scaffold builds these original EA targets:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`
- `wwlib`
- `wwutil`
- `wwnet`
- `BinkMovie`

The scaffold has already shown that original EA project names are not final architecture boundaries:

- `wwlib` mixes foundation, platform, display/surface, input/message, registry/version, encoding/string, content/archive, Windows integration, and diagnostics concerns.
- `wwnet` mixes packet/object helpers with live WinSock/session concerns.
- `BinkMovie` mixes subtitle data parsing, subtitle rendering glue, and RAD/Bink playback integration.

## Strategy decision

Keep the EA-baseline-first strategy.

Do not pivot back to patching the OpenW3D fork first, do not replay fork changes yet, and do not physically reorganize files before boundaries are proven.

Rework the execution model from manual target accretion to a repeatable pipeline:

```text
VC6 manifest -> generated CMake skeleton -> source-island classification -> build probe -> deferred-source ledger -> parity evidence
```

## Two-track program model

### Track A: preservation and parity

Purpose: make the original EA baseline reproducibly buildable and behavior-checkable.

Deliverables:

- VC6 project manifest
- CMake target graph
- product matrix for client, FDS, tools, and script/plugin outputs
- artifact parity documentation
- build/probe logs
- smoke tests and fixture-based checks where possible

### Track B: architecture and bucket cleanup

Purpose: make every historical bucket easier to understand and eventually reorganize.

Deliverables:

- source-island classifications
- deferred-source ledger
- future bucket map
- platform/renderer/audio/input/network/media/online boundary contracts
- later physical reorganization plan

## Standard pipeline for future targets

1. Inspect the target in `build-manifests/vc6-projects.json`.
2. Generate or update a manifest-derived CMake scaffold that preserves the original target name.
3. Classify sources into logical islands before broad compile patching.
4. Probe the target build and classify the first blocker.
5. Apply only narrow compatibility fixes when they preserve behavior and do not hide a real seam.
6. Defer sources that require real platform, renderer, audio, media, online, or product-shell boundaries.
7. Record all deferred sources in the ledger.
8. Record parity status separately from scaffold build status.
9. Verify with configure, target build, and full scaffold build when applicable.
10. Commit a bounded batch with documentation.

## New reusable skills/workflows

The modernization should use these project-specific workflows:

- `openw3d-vc6-target-ingestion`
- `openw3d-source-island-classification`
- `openw3d-deferred-source-ledger`
- `openw3d-build-probe-triage`
- `openw3d-parity-evidence`

These workflows exist to prevent repeated rediscovery of the same process across the remaining original EA targets.

## Deferred-source ledger policy

A successful scaffold build must not imply all original source files compile.

Whenever a source file is intentionally excluded or deferred, record:

- target
- source file
- source island
- blocker category
- reason deferred
- required future boundary or dependency
- whether it matters for client, FDS, or tools
- status

Recommended repo location:

```text
docs/architecture/deferred-source-ledger.md
```

## Parity evidence policy

Report scaffold progress and parity progress separately.

Useful states:

```text
not inventoried
manifest parsed
CMake target exists
sources classified
portable island builds
full original source target builds
artifact produced
artifact compared
behavior smoke-tested
accepted deviations documented
parity complete for current phase
```

For example, `BinkMovie` currently has portable subtitle data/parser scaffold coverage, but not full RAD/Bink playback parity.

## Near-term plan

### Batch 008: automation and ledger batch

Completed in Batch 008:

- created `docs/architecture/deferred-source-ledger.md`
- added an initial ledger with known `wwlib`, `wwnet`, and `BinkMovie` deferred sources
- added `docs/build/project-ingestion-and-probe-workflow.md`
- added `scripts/projects/write_cmake_scaffold.py`
- added `scripts/build/probe_targets.py`
- generated `docs/build/target-probe-report-batch-008.md`
- updated `README.md` with modernization branch status and current scaffold commands

### Batch 009: targeted scaffold from triage

Use the new workflow to inspect, and where appropriate build, candidates such as:

- `SControl`
- `WWAudio`
- `Scripts`
- `Combat`
- `ww3d2`
- `wwphys`
- `wwui`
- representative tools

Output should be a ranked next-target recommendation based on blocker categories and product value.

### Batch 010+: grouped implementation waves

After triage, model groups of related targets or source islands rather than one-off manual additions, while still keeping commits bounded and reviewable.

## Guardrails

Continue to avoid:

- fake DirectDraw/DirectSound/WinSock/Bink/Miles/GameSpy/WOL stubs that hide real boundaries
- jumping straight to `Commando`
- replaying OpenW3D fork deltas before baseline contracts and scaffold status are stronger
- physical file moves before logical boundaries are proven
- broad platform abstraction before recurring seams are inventoried and minimal contracts are designed

Continue to prefer:

- original target names during baseline scaffold work
- manifest-derived source membership
- logical source-island classification
- small compatibility fixes only when they are low-risk
- explicit documentation of deferred work
- full scaffold verification after each batch
