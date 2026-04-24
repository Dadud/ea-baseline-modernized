# Agentic Workflow — EA Baseline Modernization

## Purpose

This document defines the **default human+agent workflow** for working in this repo.

It exists because the project already has good strategy docs, but the day-to-day execution flow is too scattered across handoff notes, roadmap entries, probe scripts, and older plan files.

Use this as the operational layer:
- what to read first
- what to check before changing code
- when to use sub-agents
- how to run bounded batches
- what must be verified before claiming progress

This workflow follows the standing doctrine:
- **EA baseline first**
- **OpenW3D as donor/reference first, eventual merge/replay target later**
- **bounded, reviewable batches**
- **no fake portability claims**
- **do not confuse modeled / scaffold-buildable / parity-proven**

Long-term direction:
- make the baseline understandable and modular enough to absorb relevant OpenW3D/w3dhub commits and decisions cleanly
- preserve future mergeability when choosing seams, boundaries, and refactors
- prefer work that makes future renderer modernization, shader modernization, subsystem replacement, and upstream convergence easier

---

## The default execution loop

For most work in this repo, follow this loop:

1. **Orient**
   - Read the smallest set of docs needed.
   - Confirm current repo/build state before assuming older docs are current.

2. **Classify the task**
   - Is this:
     - doc cleanup
     - target probe/classification
     - bounded code batch
     - donor comparison
     - architecture decision

3. **Run the smallest useful preflight**
   - Doc work: run `check_doc_sync.py`
   - Code work: inspect target CMake/source classification first
   - Build work: reproduce the smallest relevant configure/build

4. **Decide whether to spawn sub-agents**
   - Use them liberally for parallelizable investigation.
   - Keep main-thread ownership of prioritization, synthesis, and final decisions.

5. **Execute a bounded batch**
   - One clear scope
   - One claim
   - One verification path
   - One doc update set

6. **Verify before claiming success**
   - Run the smallest meaningful gate.
   - Prefer direct evidence over reasoning.

7. **Update docs to match reality**
   - If target status, ledger state, or roadmap state changed, document it in the same batch.

---

## Read order by task type

Do **not** always read everything. Read by intent.

### 1. General repo re-orientation
Read in this order:
1. `docs/architecture/ai-handoff-guide.md`
2. `docs/architecture/modernization-progress-roadmap.md`
3. `docs/parity/target-status.md`
4. `docs/architecture/deferred-source-ledger.md`

### 2. Planning / strategy work
Read:
1. `docs/architecture/upstream-convergence-plan.md`
2. `docs/architecture/near-term-batch-roadmap.md`
3. `docs/architecture/modernization-strategy-v2.md`
4. `docs/architecture/ai-handoff-guide.md`
5. `docs/architecture/current-to-future-bucket-map.md`
6. `docs/architecture/9-bucket-architecture-map.md`
7. `docs/architecture/platform-boundary-contracts.md`

### 3. Target probe / scaffold work
Read:
1. `docs/parity/target-status.md`
2. `docs/architecture/deferred-source-ledger.md`
3. `docs/build/project-ingestion-and-probe-workflow.md`
4. `scripts/build/probe_targets.py`
5. `scripts/projects/write_cmake_scaffold.py`

### 4. Donor replay / comparison work
Read:
1. `docs/architecture/ai-handoff-guide.md`
2. `docs/architecture/w3dhub-port-evaluation.md`
3. `scripts/replay/compare_openw3d_port.py`

### 5. Batch-local code work
Read only what directly constrains the batch:
- relevant target `CMakeLists.txt`
- touched source/header files
- the matching ledger rows
- the matching roadmap entry or intended next batch note

---

## Standard preflight checks

### For doc-only batches
Run:
```bash
python3 scripts/architecture/check_doc_sync.py
```

### For default scaffold validation
Run:
```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build/cmake-scaffold -j4
```

### For SDL3 `wwlib` seam validation
Run:
```bash
cmake -S . -B build/cmake-scaffold-sdl3 \
  -DRENEGADE_USE_SDL3=ON \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_PLATFORM_LIBS=ON \
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
```

### For target investigation
Start with:
```bash
python3 scripts/build/probe_targets.py --help
```
Then run the smallest probe that answers the current question.

---

## Sub-agent usage model

Use sub-agents **liberally where appropriate**, but not sloppily.

### Good uses for sub-agents

Spawn sub-agents for:
- call-site analysis
- target/source-island audits
- donor/reference comparison
- doc review and stale-doc detection
- bounded implementation scouting
- bounded code batches in a single subsystem
- CI/build-matrix or verification design

### Keep in the main thread

Main thread owns:
- final prioritization
- architecture decisions
- tradeoff calls
- synthesis across multiple audits
- final verification judgment
- user-facing recommendation

### Guardrails

When spawning sub-agents:
1. **Announce the work** in the main thread.
2. **Give tight scope**: files, question, and exit criteria.
3. **Avoid nested spawning** from sub-agents.
4. **Do not let sub-agents silently define project policy.**
5. **Review before acting** — sub-agent output is input, not truth.

### Default split by task type

| Task | Default owner |
|---|---|
| Strategy / milestone choice | main thread |
| Call-site tracing | sub-agent |
| Deferred-source audit | sub-agent |
| Doc freshness audit | sub-agent |
| Small cross-doc synthesis | main thread |
| One-subsystem code batch | sub-agent + main-thread review |
| Multi-subsystem refactor | main thread first |
| Final commit/push recommendation | main thread |

### Anti-patterns

Avoid:
- spawning multiple agents onto the same file set without explicit partitioning
- letting a sub-agent both investigate and silently merge broad architectural policy
- using sub-agents for trivial one-file edits that are faster to do directly
- treating a sub-agent result as verified without checking the code/docs/build

---

## Bounded batch protocol

Every batch should answer these five questions before work starts:

1. **What is the scope?**
   - exact files, target, or doc set
2. **What is the claim?**
   - example: "classify as honest defer" or "make this seam build under SDL3"
3. **What is out of scope?**
   - explicit non-goals
4. **What is the verification gate?**
   - doc sync, configure, single-target build, scaffold build, etc.
5. **Which docs must move with it?**
   - ledger, roadmap, handoff, target status, boundary docs

A batch is done when:
- the code/docs change exists
- the verification gate passed
- the relevant docs match the new truth

---

## Recommended batch shapes

### 1. Classification-only batch
Use when the honest answer is "not now".

Deliverables:
- ledger update if needed
- roadmap note
- handoff/current-state update if needed
- no fake fallback code

Good example:
- classify `cpudetect.cpp` / `mpu.cpp` as honest defer, not current scaffold blockers

### 2. Real seam batch
Use when a narrow portable path can build honestly.

Deliverables:
- bounded source changes
- CMake gating
- explicit behavior limitations
- build verification
- doc updates

Good examples:
- `thread.cpp`
- `rawfile.cpp`
- `rcfile.cpp`
- `verchk.cpp`
- `mutex.cpp`

### 3. Probe/modeling batch
Use when a target needs structured triage before implementation.

Deliverables:
- target probe result
- classified blockers
- deferred-source entries
- target status update

---

## Verification rules

Do not over-verify, but do not hand-wave.

### Minimum gate by batch type

| Batch type | Minimum gate |
|---|---|
| doc-only | `check_doc_sync.py` |
| `wwlib` seam batch | target build for `wwlib`; scaffold build if claim affects default path |
| target classification | probe output + targeted inspection |
| compile-fix batch | smallest affected target build |
| roadmap/ledger-only decision batch | doc sync + direct doc inspection |

### Reporting rule

Always keep these separate:
- **modeled**
- **builds in default scaffold**
- **has parity evidence**

---

## Current recommended workflow lanes

### Lane A — Keep the current green path healthy
Priority:
- default scaffold stays green
- SDL3 `wwlib` path stays green
- docs stay in sync

### Lane B — Retire the highest-leverage blockers
Prefer work that unlocks real downstream progress over neat-but-low-value cleanup.

Examples:
- renderer/math header tangle
- `Combat` / `wwphys` dependency leakage
- product-shell ownership boundaries

### Lane C — Separate FDS-path priorities from client-path priorities
Not every deferred item matters equally.

Use this distinction explicitly:
- **FDS-relevant now**
- **client-only later**
- **tools-only later**

### Lane D — Use donor work in a way that preserves eventual upstream convergence
Compare or replay donor work only when:
- the boundary is understood
- the baseline target is already classified
- the replay can be landed as a bounded batch
- the result improves future merge/replayability instead of creating a one-off local divergence

---

## Current workflow cleanup recommendations

These are not all mandatory immediately, but they are the highest-value cleanup items discovered during workflow review.

### High value
1. Add this workflow doc to the normal handoff/read path.
2. Update stale path/branch references in current-state docs.
3. Stop treating old `.hermes/plans/` files as current truth if the roadmap has moved past them.
4. Prefer one current-status source over multiple overlapping status docs when possible.

### Medium value
1. Add a short TL;DR index near the top of `ai-handoff-guide.md`.
2. Standardize `-DCMAKE_EXPORT_COMPILE_COMMANDS=ON` in local bootstrap commands.
3. Add a lightweight CI gate later for:
   - doc sync
   - default configure
   - maybe one narrow scaffold smoke build

### Ongoing hygiene
1. If a batch changes reality, update docs in the same batch.
2. If a sub-agent finds a durable policy/decision, record it in project docs or memory.
3. Prefer deleting or superseding stale execution notes instead of letting them silently drift.

---

## Practical default for the next agent

If you wake up cold in this repo, do this first:

```bash
python3 scripts/architecture/check_doc_sync.py
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
cmake --build build/cmake-scaffold -j4
```

Then read:
1. `docs/architecture/ai-handoff-guide.md`
2. `docs/architecture/modernization-progress-roadmap.md`
3. `docs/parity/target-status.md`
4. `docs/architecture/deferred-source-ledger.md`
5. this file

Then decide:
- keep green
- classify next blocker
- land one bounded seam batch
- or do doc cleanup to restore current truth
