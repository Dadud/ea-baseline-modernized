# Target Status Summary

This summary distinguishes three different states that were previously easy to blur together:

1. **Modeled** — an original VC6 project has been inventoried and described in the modernization scaffold/docs.
2. **Default green scaffold** — the target is part of the default `RENEGADE_BUILD_FOUNDATION_LIBS=ON` Linux bootstrap build and currently compiles.
3. **Parity evidence** — artifact/runtime parity has been proven. This is still largely **not started**.

## Current snapshot

- Total original VC6 projects in manifest: `61`
- Modeled in some form: `14`
- Not modeled yet: `47`
- Additional prep-only docs now exist for `commando`, but it still does **not** count as modeled until a target/scaffold exists.

### By project type

| Project type | Total | Modeled | Not modeled |
|---|---:|---:|---:|
| Application | 25 | 0 | 25 |
| Console Application | 10 | 0 | 10 |
| Dynamic-Link Library | 10 | 1 | 9 |
| Static Library | 16 | 13 | 3 |

## Default green scaffold targets

These are the targets verified by the default Linux bootstrap scaffold command:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Current default green targets:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`
- `wwlib`
- `wwutil`
- `wwnet`
- `BinkMovie`
- `SControl`

## Modeled but not part of the default green scaffold

These are intentionally documented/mapped but not included in the default green scaffold claim:

| Target | Current state | Why not in default green scaffold |
|---|---|---|
| `WWAudio` | Opt-in seam map exists | Stops at proprietary Miles/DirectSound backend boundary; configured via `RENEGADE_BUILD_AUDIO_SEAMS=ON` |
| `wwphys` | Opt-in seam map exists | Reaches mixed runtime/renderer/platform leakage; configured via `RENEGADE_BUILD_PHYS_SEAMS=ON` |
| `wwui` | Opt-in seam map exists | Reaches mixed UI/input/platform/renderer leakage; configured via `RENEGADE_BUILD_UI_SEAMS=ON` |
| `ww3d2` | Opt-in seam map exists | Reaches mixed renderer/platform/asset-content leakage; configured via `RENEGADE_BUILD_RENDERER_SEAMS=ON` |
| `Combat` | Opt-in seam map exists | Reaches mixed gameplay/runtime, product-shell, and audio-backend leakage; configured via `RENEGADE_BUILD_COMBAT_SEAMS=ON` |
| `Scripts` | Opt-in seam map exists | Runtime core builds as a Linux shared-library probe while DLL entry and mission content remain deferred; configured via `RENEGADE_BUILD_SCRIPT_SEAMS=ON` |

## Current parity evidence state

Parity evidence should use the states from `openw3d-parity-evidence`:

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

### Current practical interpretation

| Scope | Current highest evidence |
|---|---|
| `wwdebug`, `wwmath`, `wwbitpack`, `wwsaveload`, `wwtranslatedb`, `wwlib`, `wwutil`, `wwnet`, `BinkMovie`, `SControl` | portable island builds / compile-checkable scaffold evidence |
| `WWAudio`, `wwphys`, `wwui`, `ww3d2`, `Combat` | sources classified / blocker documented |
| `Scripts` | portable runtime-core builds / DLL parity not proven |
| client / FDS / tools / scripts product integration | not yet parity-proven |

## Important reporting rule

Do **not** collapse these statements into one:

- “modeled”
- “builds in the default scaffold”
- “has parity evidence”

For example:

- `BinkMovie` is modeled and its subtitle-parser island builds, but full movie playback parity is not proven.
- `WWAudio` is modeled as an opt-in seam map, but is not part of the default green scaffold and has no parity proof.
- `SControl` builds in the default scaffold, but that is still compile evidence rather than product or runtime parity.

## Related docs

- `docs/architecture/commando-dependency-prep.md`
- `docs/architecture/original-project-scaffold-status.md`
- `docs/architecture/deferred-source-ledger.md`
- `docs/architecture/modernization-progress-roadmap.md`
- `docs/build/foundation-scaffold-build-findings.md`
