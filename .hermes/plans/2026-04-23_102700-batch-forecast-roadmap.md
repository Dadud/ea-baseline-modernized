# OpenW3D EA-Baseline Batch Forecast Roadmap

> **For Hermes:** Use subagent-driven-development skill to execute this roadmap wave-by-wave, not as one giant refactor.

**Goal:** Estimate the remaining modernization batches needed to reach (1) a structurally ready baseline for product assembly work and (2) first serious client/FDS/tools parity attempts.

**Architecture:** Continue the EA-baseline-first strategy. Treat future work as grouped waves: status/report cleanup, high-coupling subsystem mapping, product-graph preparation, then product/parity work. Keep logical boundaries and deferred-source tracking ahead of physical reorganization.

**Tech Stack:** Git, CMake, Python manifest/probe tooling, conservative C++98-era scaffold compatibility bridges, Markdown architecture docs.

---

## Current baseline snapshot

As of commit `8d118b5 build: map WWAudio backend seam`:

- Manifested VC6 projects: `61`
- Currently modeled in some form: roughly `11`
- Still not modeled: roughly `50`
- Current default green scaffold targets:
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
- Additional opt-in mapped seam:
  - `WWAudio`

## Batch forecast summary

### Milestone A: baseline structurally ready for product work

Estimated remaining batches: **12–20**

This means:

- major remaining subsystem buckets are at least inventoried/scaffolded/classified
- deferred-source and boundary docs are mature enough to reason about product assembly
- client/FDS/tools dependency graphs are documentable without guessing
- high-coupling targets are understood well enough to start staged product work

### Milestone B: first serious client/FDS/tools parity attempts

Estimated remaining batches: **25–40+**

This means:

- `Commando` and product shells are partially decomposed or at least buildable under a documented modern scaffold
- FDS/client/product rules are explicit
- script/tool/product outputs are classified
- first artifact or smoke-test parity attempts are possible
- accepted deviations and third-party SDK gaps are documented

## Why the number is not 50 more batches

The remaining 50 unmodeled VC6 projects should not be handled as 50 bespoke manual batches.

Many should be grouped into waves:

- renderer test apps wave
- tools CLI wave
- tools GUI wave
- plugin/importer DLL wave
- tests wave
- product-support utilities wave

The batch count is driven by architectural seam complexity, not by raw project count.

## Recommended remaining waves

### Wave 1: status and report cleanup (1–2 batches)

Purpose:

- tighten generated/maintained status docs
- improve probe report summaries
- ensure modeled vs opt-in vs green-scaffold states are consistently reported

Likely outputs:

- cleaner scaffold-status matrix
- better deferred-source rollups
- maybe generated summary docs for modeled vs deferred subsystems

### Wave 2: high-value subsystem mapping (4–6 batches)

Targets:

- `wwui`
- `wwphys`
- `ww3d2`
- `Combat`
- `Scripts`
- one representative tools wave

Goal:

- map source islands and first blockers
- avoid pretending these are low-friction portability targets
- keep product-shell work deferred until boundaries are clearer

### Wave 3: product graph preparation (3–5 batches)

Focus:

- `Commando` inventory and dependency layering
- FDS/client split rules
- script DLL placement in product graph
- tools dependency layering
- candidate CMake option structure for product builds

Goal:

- be able to explain how client, FDS, tools, and scripts will be assembled without hand-waving

### Wave 4: platform boundary consolidation (2–4 batches)

Focus:

- socket/network boundary
- threading/synchronization boundary
- string/encoding boundary
- filesystem/path boundary
- maybe initial renderer/audio backend contract docs

Goal:

- stop ad hoc local bridges from proliferating before product work starts

### Wave 5: first product attempts (5–10+ batches)

Focus:

- staged `Commando` work
- FDS build mode
- first tools wave build attempts
- scripts/DLL handling
- artifact/smoke-test parity beginnings

This is where pace becomes much less predictable.

## Concrete forecast table

| Milestone | Remaining batches | Confidence | Notes |
|---|---:|---|---|
| Better status/report discipline | 1–2 | high | Mostly documentation/tooling cleanup |
| Major subsystem mapping mostly covered | 6–9 | medium | Depends on how ugly `Combat`, `ww3d2`, `wwui`, `wwphys` are |
| Structurally ready for product work | 12–20 | medium | Best planning target |
| First product build attempts | 18–28 | low-medium | Depends heavily on `Commando`/FDS coupling |
| First serious parity attempts | 25–40+ | low-medium | Depends on SDK gaps, product shell complexity, and runtime smoke test design |

## Recommended planning assumption

Use this working assumption:

```text
~8 more batches for major subsystem mapping
~5 more batches for product-graph and boundary preparation
~10–20 more batches after that for product/parity work
```

That yields a practical estimate of:

- **15–20 more batches** before the baseline feels structurally ready
- **25–40+ more batches** before serious parity attempts are likely underway

## Immediate next-batch recommendation

Do not jump to `Commando` next.

Best next batch:

1. generated status/report cleanup, or
2. the smallest next high-value classification target among:
   - `wwui`
   - `wwphys`
   - `ww3d2`
   - `Combat`

My recommendation is one more cleanup/report batch first, then `wwui` or `wwphys` before `Combat` or `Commando`.

## Verification

After any roadmap-only batch, verify the default scaffold still builds:

```bash
cd /tmp/openw3d-ea-baseline
git diff --check
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Expected current default green targets:

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

`WWAudio` remains an opt-in seam probe, not part of the default green scaffold.
