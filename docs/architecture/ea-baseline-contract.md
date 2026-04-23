# EA Baseline Contract

## Purpose

This document defines what the `ea-baseline-modernized` branch is allowed to do.

The goal is to modernize the original Electronic Arts Renegade source release enough to build and verify the original product set with modern tooling, while preserving behavior and making future OpenW3D replay work easier.

## Source baseline

Authoritative baseline:

```text
ea/main @ 3e00c3a1b97381bb28be89a35b856375e0629a08
```

Local branch:

```text
ea-baseline-modernized
```

## Product targets in scope

The baseline program should preserve and modernize the original product shape.

### 1. Game client
Original project family:
- `Commando`
- executable family: `Renegade*.exe`

Responsibilities:
- full game client
- frontend/menu flow
- renderer/audio/input/UI integration
- multiplayer client shell

### 2. Free Dedicated Server (FDS)
Original project mode:
- `FREEDEDICATEDSERVER` build configuration / define

Responsibilities:
- headless or mostly-headless server product
- server configuration parsing
- server console/admin behavior
- network gameplay hosting

### 3. Tools and editor utilities
Original tools include, but are not limited to:
- LevelEdit
- W3DView
- WWConfig
- MakeMix
- MixViewer
- RenRem
- wdump
- W3DShellExt
- legacy tool trees under `Code/Tools/*`

Responsibilities:
- preserve original tool availability where dependencies exist
- document unavailable third-party SDK blockers clearly
- eventually expose shared asset/content libraries for tool reuse

## Initial target architecture

The final long-term architecture uses 9 logical buckets:

1. `foundation`
2. `platform`
3. `engine_runtime`
4. `engine_asset_content`
5. `renderer`
6. `audio`
7. `input_and_control`
8. `online_services`
9. `products`

During the EA-baseline phase, only minimal conservative separation should be introduced:

- `foundation`
- `platform`
- `products`
- small, build-required slices of `engine_runtime`
- small, build-required slices of `engine_asset_content`

Do not fully redesign renderer, input, or online systems during the first baseline pass unless absolutely required for buildability.

## Allowed changes on `ea-baseline-modernized`

Allowed:
- documentation
- build scripts
- modern build-system scaffolding
- source inventory scripts
- artifact manifest tooling
- compiler compatibility fixes with minimal semantic impact
- product target separation for client/FDS/tools
- small platform abstraction introduced only where needed to build
- small source list cleanup that preserves original product behavior

Not allowed yet:
- OpenW3D feature merges
- Linux-first redesigns
- renderer backend rewrites
- major gameplay behavior changes
- broad source movement before logical boundaries are proven
- deleting legacy systems just because they are hard to modernize

## Required documentation for every accepted deviation

Every nontrivial deviation from original source behavior must be documented as one of:

1. **Toolchain noise** — caused by modern compiler/linker timestamps, debug info, deterministic build flags, etc.
2. **Unavailable dependency** — caused by missing third-party SDKs such as Bink, Miles, Lightscape, Umbra, GameSpy, SafeDisk, etc.
3. **Compatibility shim** — a build-only or platform-only compatibility layer intended to preserve behavior.
4. **Accepted modernization difference** — an intentional difference with rationale.
5. **Unresolved blocker** — something that still needs investigation.

## Build-system policy

Modern build scaffolding should preserve the original product model first.

Preferred order:

1. inventory original projects/source lists
2. reproduce client/FDS/tools targets
3. document third-party SDK requirements
4. build with modern MSVC where feasible
5. add artifact manifests and parity checks
6. only then improve organization and target boundaries

## Success criteria

The EA-baseline phase is successful when:

- the selected baseline is documented
- product targets are documented
- client/FDS/tools build expectations are documented
- modern build commands exist or blockers are explicitly listed
- artifact parity tooling exists
- all deviations are documented
- future OpenW3D replay can start from a known contract
