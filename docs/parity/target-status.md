# Target Status Summary

This summary distinguishes three different states that were previously easy to blur together:

1. **Modeled** — an original VC6 project has been inventoried and described in the modernization scaffold/docs.
2. **Default green scaffold** — the target is part of the default `RENEGADE_BUILD_FOUNDATION_LIBS=ON` Linux bootstrap build and currently compiles.
3. **Parity evidence** — artifact/runtime parity has been proven. This is still largely **not started**.

## Current snapshot

- Total original VC6 projects in manifest: `61`
- Modeled in some form: `11`
- Not modeled yet: `50`

### By project type

| Project type | Total | Modeled | Not modeled |
|---|---:|---:|---:|
| Application | 25 | 0 | 25 |
| Console Application | 10 | 0 | 10 |
| Dynamic-Link Library | 10 | 0 | 10 |
| Static Library | 16 | 11 | 5 |

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
| `WWAudio` | sources classified / backend blocker documented |
| client / FDS / tools / scripts | not yet parity-proven |

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

- `docs/architecture/original-project-scaffold-status.md`
- `docs/architecture/deferred-source-ledger.md`
- `docs/architecture/modernization-progress-roadmap.md`
- `docs/build/foundation-scaffold-build-findings.md`
