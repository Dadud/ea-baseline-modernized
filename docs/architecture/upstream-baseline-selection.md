# Upstream Baseline Selection

## Decision

Use `ea/main` from `https://github.com/electronicarts/CnC_Renegade.git` as the authoritative EA baseline for the modernization program.

Baseline commit:

```text
3e00c3a1b97381bb28be89a35b856375e0629a08 Remove files left over from the previous test build.
```

Local worktree:

```text
machine-local; discover from the active checkout with git rev-parse --show-toplevel
```

Active modernization branch in the maintained workspace:

```text
main
```

## Why this baseline

`ea/main` is the shortest, most archival history available from the original Electronic Arts source release:

```text
74ab8fa Initial commit
58ed459 Initial commit of Command & Conquer Renegade source code.
854866c initial .gitignore.
d0e4fde Adds LICENSE.md and initial README.md.
3e00c3a Remove files left over from the previous test build.
```

Compared with current fork branches:

| Ref | Commit Count | Relationship to EA baseline |
| --- | ---: | --- |
| `ea/main` | 5 | authoritative source-release baseline |
| `omni/main` | 221 | 216 commits ahead of `ea/main` |
| `origin/main` | 299 | 294 commits ahead of `ea/main` |
| `w3dhub/main` | 299 | 294 commits ahead of `ea/main` |
| `origin/develop-slop` | 300+ | fork modernization + Linux exploration work |

All fork branches share `3e00c3a` as the merge-base with `ea/main`, so `ea/main` is the correct root for a parity-first modernization track.

## Selection criteria

The modernization program needs a baseline that is:

1. **Provenance-clean** — directly from the Electronic Arts source release.
2. **Behavior-preserving** — before fork-era cleanup or modernization deltas.
3. **Small-delta** — minimal history so build-system changes can be audited clearly.
4. **Replay-friendly** — later OpenW3D/Omni/W3DHub changes can be categorized and replayed on top.

`ea/main` satisfies these better than `origin/main`, `w3dhub/main`, or `omni/main`.

## Branching model

### Current archived exploration branch

The previous Linux/OpenW3D exploration state is archived locally as:

```text
archive/develop-slop-linux-exploration-2026-04-23
```

Tag:

```text
develop-slop-pre-baseline-pivot
```

That branch preserves:
- Linux build exploration
- modernization planning docs
- known Win32/platform/header-leakage findings
- the `develop-slop` state before pivoting to the EA-baseline-first strategy

### Active baseline branch

The active baseline work happens on:

```text
ea-baseline-modernized
```

This branch should initially contain only:
- documentation
- build/parity scaffolding
- conservative build modernization
- minimal code changes required to compile original products

## What not to do on this branch yet

Do **not** immediately merge:
- `origin/main`
- `w3dhub/main`
- `omni/main`
- `origin/develop-slop`
- OpenAL/SDL3/Linux-port PR branches

Those belong in the later replay phase after the baseline modernization and parity contracts are documented.
