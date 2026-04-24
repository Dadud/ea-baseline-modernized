# AI Handoff Guide — EA-Baseline Modernization

This guide is for a fresh AI agent that needs to resume work in this repository without relying on prior chat history.

It explains:
- the current goal
- the current repository state
- the most important constraints
- what has already been completed
- what documentation and plans must be read first
- what tools/scripts exist to help
- what to do next
- what **not** to do

---

## 1. Primary goal

Continue the **EA-baseline-first** modernization of Renegade/OpenW3D.

The current strategy is:
1. start from the original EA Renegade baseline
2. preserve client / FDS / tools parity expectations
3. document and prove architecture boundaries before moving files
4. use OpenW3D and related forks/PRs as **donor/reference only**
5. replay OpenW3D fork work into the EA baseline in bounded batches
6. only later do broader product assembly and parity work

Do **not** revert to patching the OpenW3D fork first.

---

## 2. Repository and branch state

### Active implementation repo
- Canonical role: **EA baseline modernization truth**
- Recommended symbolic name in this guide: `<EA_BASELINE_REPO_ROOT>`
- Expected branch: `main`
- Expected remote: `origin`
- Latest known documentation handoff commit:
  - `3e60dbb docs: add compact AI bootstrap prompt`
- Latest known implementation seam commit:
  - `75bf5df build: add opt-in SDL3 mutex seam for wwlib`

### Donor/reference repo
- Canonical role: **OpenW3D donor/reference tree**
- Recommended symbolic name in this guide: `<DONOR_OPENW3D_REPO_ROOT>`

Use `<DONOR_OPENW3D_REPO_ROOT>` only as a donor/reference tree.
Use `<EA_BASELINE_REPO_ROOT>` as the modernization truth.

### Path discovery on a new machine
Do **not** assume the repos live under `/tmp` or any fixed directory.

Preferred discovery rules:
1. Start from the current git checkout and use:
   ```bash
   git rev-parse --show-toplevel
   ```
   to identify `<EA_BASELINE_REPO_ROOT>`.
2. If the donor repo is not adjacent or obvious, ask the operator for the OpenW3D donor checkout path or locate it from their workspace layout.
3. If no donor checkout exists yet, clone one where appropriate for that machine and treat its root as `<DONOR_OPENW3D_REPO_ROOT>`.
4. Build directories are also machine-local choices. `build/cmake-scaffold` and `build/cmake-scaffold-sdl3` are conventions, not hard requirements.

---

## 3. Current high-level status

The baseline has already moved beyond pure scaffold generation. It now includes a bounded SDL3/platform replay into `wwlib`.

### Current default green scaffold
These build in the default Linux scaffold:
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

### Current SDL3/platform seam progress in `wwlib`
The following opt-in non-Windows seams now build under `RENEGADE_USE_SDL3=ON`:
- Batch 023 — `thread.cpp`
- Batch 024 — `rawfile.cpp`
- Batch 025 — `rcfile.cpp`
- Batch 026 — `verchk.cpp` (partial version/file metadata seam)
- Batch 027 — `mutex.cpp`

These are real seam implementations, but they do **not** imply full parity.

---

## 4. Current verified build commands

### Default scaffold
Run from `<EA_BASELINE_REPO_ROOT>`:
```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Expected successful end-state currently includes:
```text
[100%] Built target wwnet
```

### SDL3-enabled `wwlib` seam verification
Run from `<EA_BASELINE_REPO_ROOT>`:
```bash
cmake -S . -B build/cmake-scaffold-sdl3 \
  -DRENEGADE_USE_SDL3=ON \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_PLATFORM_LIBS=ON
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
```

Expected successful end-state currently includes:
```text
[100%] Built target wwlib
```

### Doc consistency check
Run from `<EA_BASELINE_REPO_ROOT>`:
```bash
python3 scripts/architecture/check_doc_sync.py
```

---

## 5. Mandatory reading order for a new AI

If you are starting fresh, read these in this order.

### First: strategy / current truth
1. `docs/architecture/ai-handoff-guide.md`  ← this file
2. `docs/architecture/modernization-progress-roadmap.md`
3. `docs/architecture/modernization-strategy-v2.md`
4. `docs/parity/target-status.md`

### Second: architecture contracts and ledgers
5. `docs/architecture/deferred-source-ledger.md`
6. `docs/architecture/platform-boundary-contracts.md`
7. `docs/architecture/artifact-parity.md`
8. `docs/architecture/current-to-future-bucket-map.md`
9. `docs/architecture/9-bucket-architecture-map.md`

### Third: baseline and project graph
10. `docs/architecture/upstream-baseline-selection.md`
11. `docs/architecture/ea-baseline-contract.md`
12. `docs/architecture/original-project-inventory.md`
13. `docs/architecture/project-scaffold-status.md`
14. `docs/architecture/original-project-scaffold-status.md`
15. `build-manifests/vc6-projects.json`

### Fourth: process / scripts / prior planning
16. `docs/build/project-ingestion-and-probe-workflow.md`
17. `.hermes/plans/2026-04-23_093416-modernization-progress-review-and-next-plan.md`
18. `.hermes/plans/2026-04-23_101900-commando-dependency-prep.md`
19. `.hermes/plans/2026-04-23_102700-batch-forecast-roadmap.md`

### Fifth: donor/replay guidance
20. `docs/architecture/w3dhub-port-evaluation.md`
21. `scripts/replay/compare_openw3d_port.py`

If working on product-shell or renderer/audio prep, also read:
- `docs/architecture/renderer-seam-contract.md`
- `docs/architecture/math-types-extraction-spec.md`
- `docs/architecture/stub-contracts.md`
- `docs/architecture/commando-dependency-prep.md`

---

## 6. Important helper scripts

### Manifest / scaffold / probe
- `scripts/projects/parse_vc6_workspace.py`
  - parse original VC6 `.dsw` / `.dsp` into manifest JSON
- `scripts/projects/write_project_inventory.py`
  - write human-readable project inventory
- `scripts/projects/write_cmake_scaffold.py`
  - generate a manifest-derived CMake scaffold for a target
- `scripts/build/probe_targets.py`
  - classify blockers and write structured probe results

### Doc / donor helpers
- `scripts/architecture/check_doc_sync.py`
  - verify target-name/doc consistency
- `scripts/replay/compare_openw3d_port.py`
  - compare EA baseline vs donor paths/groups

Use these before inventing new one-off workflows.

---

## 7. Most important standing constraints

### Architecture / strategy constraints
- Keep **EA baseline first**.
- Keep **original target names** during baseline scaffold work.
- Keep **logical boundary work ahead of physical file moves**.
- Keep **bounded, reviewable batches**.
- Keep **Windows behavior intact** unless a deviation is documented.
- Keep **OpenW3D/fork work as donor/reference**, not truth.

### Things you must not do
- Do **not** jump straight to broad `Commando` product work unless the batch explicitly requires it.
- Do **not** claim parity from compile success.
- Do **not** add fake platform/SDK headers just to make code compile.
- Do **not** hide real seams with broad no-op platform stubs.
- Do **not** physically reorganize directories yet.
- Do **not** replay unrelated OpenW3D fork baggage.
- Do **not** collapse these three statuses into one:
  - modeled
  - in default green scaffold
  - parity-proven

### Particularly important anti-patterns
Avoid fake or overbroad stubs for:
- DirectDraw / Direct3D
- DirectSound / Miles
- WinSock networking
- RAD/Bink
- GameSpy / WOL / browser / COM layers

Those are real boundaries and must stay explicit.

---

## 8. Most important completed work to understand

### Baseline / scaffold foundation
The EA project graph is already captured and scaffolded from original VC6 metadata.
This is not speculative hand-written CMake.

### `wwlib` has already been decomposed logically
`wwlib` is not treated as a simple “foundation” library anymore.
It is documented as a mixed historical bucket containing:
- foundation
- platform
- display/surface
- input/message
- registry/version
- encoding/string
- content/archive
- Windows integration
- diagnostics

Its CMake source inventory is already split into named logical islands.

### Current SDL3 replay donor choice already made
When evaluating SDL3 donor work, the stronger source was the wider fork/PR ecosystem, especially:
- w3dhub PR #105
- w3dhub PR #107

Do not assume the current OpenW3D branch snapshot is always the best donor.

### SDL3 replay order already established
The first bounded SDL3/platform replay into `wwlib` has been:
1. threading
2. raw file I/O
3. resource-file seam
4. version/file metadata seam
5. mutex/synchronization seam

That sequence was deliberate.

---

## 9. Current seam truth in `wwlib`

### Implemented/building with `RENEGADE_USE_SDL3=ON`
- `thread.cpp`
- `rawfile.cpp`
- `rcfile.cpp`
- `verchk.cpp` (partial)
- `mutex.cpp`

### Still deferred or incomplete in the same broader area
- `cpudetect.cpp`
- `mpu.cpp`
- `registry.cpp`
- `win.cpp`
- `mixfile.cpp`
- `ini.cpp`
- `widestring.cpp`
- `refcount.cpp`
- `regexpr.cpp`
- `srandom.cpp`
- `strtok_r.cpp`
- `FastCriticalSectionClass` modernization (header-level design still historical)
- named/global inter-process mutex identity on non-Windows
- full non-Windows PE version-resource extraction in `verchk.cpp`
- generated embedded-resource population for `rcfile.cpp`

For exact status wording, always consult:
- `docs/architecture/deferred-source-ledger.md`

---

## 10. Recommended next work

At handoff time, the most natural next bounded batch is:

## Batch 028 — CPU/platform seam audit
Focus:
- `Code/wwlib/cpudetect.cpp`
- `Code/wwlib/mpu.cpp`

The next AI should determine:
1. whether either file has a safe portable/buildable subset
2. whether either should remain deferred as a CPU-policy seam
3. whether one can be re-enabled under `RENEGADE_USE_SDL3=ON` or another narrow non-Windows path without broad redesign

### Why this is the right next step
The current `wwlib` SDL3/platform slice is now coherent enough that the next sensible question is:
> where does platform capability / CPU policy belong, and can any of it be safely made real?

This is a better next step than jumping sideways into random product glue.

---

## 11. Suggested workflow for the next AI

If you continue from here, do this:

### Step 1: verify the repo is healthy
Run from `<EA_BASELINE_REPO_ROOT>`:
```bash
git status --short --branch
python3 scripts/architecture/check_doc_sync.py
cmake --build build/cmake-scaffold -j4
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
```

### Step 2: inspect current deferred ledger entries
Search `docs/architecture/deferred-source-ledger.md` for:
- `cpudetect.cpp`
- `mpu.cpp`
- any adjacent `wwlib` platform items you may touch

### Step 3: inspect donor/reference state
Check whether `<DONOR_OPENW3D_REPO_ROOT>` or donor PR refs contain useful, bounded prior work.
Do not assume direct replay is appropriate.

### Step 4: define the smallest honest batch
Possible outcomes are all acceptable if well documented:
- real implementation
- partial implementation
- classification-only batch
- explicit defer with reasons

### Step 5: sync docs in the same batch
At minimum update:
- `docs/architecture/deferred-source-ledger.md`
- `docs/architecture/platform-boundary-contracts.md` if the seam reality changes
- `docs/architecture/project-scaffold-status.md`
- `docs/architecture/modernization-progress-roadmap.md`

### Step 6: verify and commit
Run:
```bash
git diff --check
python3 scripts/architecture/check_doc_sync.py
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
cmake --build build/cmake-scaffold -j4
```

Then commit a bounded message naming the seam clearly.

---

## 12. How to report status correctly

When summarizing progress, keep these separate:

### Modeled
The target has a known scaffold / classification / documentation state.

### Default green scaffold
The target is in the default Linux scaffold and currently builds.

### Parity evidence
Artifact/runtime parity is actually proven.

Example of correct reporting:
- `wwlib` is modeled and builds in the default scaffold.
- `wwlib` also has several SDL3-backed opt-in seams.
- `wwlib` does **not** have full parity proof.

---

## 13. If you need the short version

If you are a fresh AI and only read one minute of context, retain this:

- Work in `<EA_BASELINE_REPO_ROOT>`
- `main` is the active branch
- latest handoff commit is `75bf5df`
- EA baseline is truth; `<DONOR_OPENW3D_REPO_ROOT>` is donor/reference only
- default scaffold is green
- SDL3 `wwlib` seams now include thread/rawfile/rcfile/verchk(partial)/mutex
- do not overclaim parity
- do not add fake SDK/platform stubs
- read the roadmap, strategy v2, deferred ledger, platform boundary contracts, parity target status, and plans before changing direction
- best next likely batch is `cpudetect.cpp` / `mpu.cpp` audit

---

## 14. Final reminder to the next AI

This repo is intentionally being modernized with **evidence, ledgers, contracts, and bounded batches**.

Do not optimize for “made it compile somehow.”
Optimize for:
- truthful boundaries
- reproducible progress
- preserved Windows semantics
- documentation that makes later replay and parity work easier

If unsure whether a change is too broad, it probably is.
Choose the smaller batch.