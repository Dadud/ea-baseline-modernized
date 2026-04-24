# Upstream Convergence Plan — EA Baseline to OpenW3D/w3dhub

## Purpose

This document makes the long-term plan concrete.

The project is **not** just trying to get an old codebase to compile locally. The long-term goal is to:
- modernize the EA baseline in a truthful, reviewable way
- absorb relevant OpenW3D/w3dhub commits and design decisions deliberately
- make future subsystem replacement and modernization realistic
- eventually converge toward `w3dhub/OpenW3D` rather than staying an isolated fork forever

This plan keeps the existing doctrine:
- EA baseline first
- bounded batches
- no fake seams
- no premature physical reorganization
- donor work replayed only when the baseline is ready

---

## One-sentence plan

First make the old engine **understandable and separable**, then make it **portable and modular enough** to absorb OpenW3D/w3dhub work cleanly, then push toward **real upstream convergence**.

---

## Success criteria

The plan succeeds if the repo becomes:

1. **Buildable with modern tooling**
   - modern CMake scaffold
   - current compiler compatibility
   - reproducible verification steps

2. **Architecturally legible**
   - clear boundary docs
   - deferred-source debt tracked honestly
   - subsystem ownership understandable

3. **Merge/replay-friendly**
   - donor commits can be compared and replayed in bounded batches
   - local changes reduce coupling instead of creating custom dead ends
   - important OpenW3D/w3dhub design decisions are documented and intentionally adopted or rejected

4. **Modernization-ready**
   - renderer/backend swap becomes realistic later
   - shader pipeline modernization becomes realistic later
   - physics replacement becomes realistic later
   - product-shell separation becomes realistic later

5. **Eventually upstream-convergent**
   - the gap to `w3dhub/OpenW3D` becomes understandable by category
   - convergence stops being a vague dream and becomes a staged engineering program

---

## Non-goals for now

This plan does **not** mean:
- rewriting the whole engine now
- adding DX12 now
- adding ray tracing now
- replacing physics now
- physically reorganizing the whole tree now
- merging blindly from donor branches
- claiming parity before evidence exists

Those may become future outcomes, but they are not current batch goals.

---

## The concrete milestones

## Milestone 1 — Stable modernization base

### Goal
Keep the repo trustworthy as a working modernization base.

### Required state
- default scaffold stays green
- SDL3 `wwlib` seam path stays green
- docs stay current enough to trust
- compile database generation is part of the normal workflow
- stale planning artifacts are removed or clearly historical

### What counts as done enough
- current workflow/handoff docs are trustworthy
- build and doc verification can be rerun without archaeology
- each batch updates reality and documentation together

---

## Milestone 2 — Boundary-first subsystem clarity

### Goal
Make the major historical tangles understandable enough that future replacements and donor replays are possible.

### Priority subsystem themes
1. `wwlib` platform/content leftovers
2. `ww3d2` renderer-vs-math separation problem
3. `Combat` runtime-vs-product-shell leakage
4. `wwphys` physics-vs-render bridge leakage
5. `wwui` Win32/UI/input/text boundary
6. `WWAudio` audio-backend boundary
7. `Commando` client/FDS/online/config/bootstrap ownership

### What counts as progress
- source islands classified honestly
- blockers named precisely
- future boundary contracts described
- small real seams landed where safe
- honest defers recorded where not safe

### What counts as done enough
- we can explain why each major subsystem is blocked or movable
- we know which parts are reusable runtime code vs platform/product glue
- future backend replacement points are visible instead of buried

---

## Milestone 3 — Mergeability-oriented extraction

### Goal
Choose boundary work that increases future replayability and upstream convergence.

### Key principle
Every meaningful refactor should answer:
> does this make future donor replay, subsystem modernization, or upstream convergence easier?

### Preferred kinds of work
- extracting platform boundaries
- isolating renderer-facing code from runtime code
- separating portable math/content structures from old backend code
- documenting adopted vs rejected donor design decisions
- reducing hard coupling between product shell and reusable subsystems

### Avoid
- local-only convenience hacks
- broad custom abstractions with no donor/upstream story
- stubs that hide the real dependency graph

### What counts as done enough
- donor commits can be evaluated against clean boundary surfaces
- major local changes are explainable as replay-friendly, not bespoke

---

## Milestone 4 — Donor decision absorption

### Goal
Absorb useful OpenW3D/w3dhub work intentionally, not as random cherry-picks.

### Work types
1. **Commit absorption**
   - identify donor commits by category
   - compare against current baseline seam state
   - replay in bounded batches where appropriate

2. **Decision absorption**
   - adopt architecture decisions, seam shapes, or subsystem splits even when code cannot be copied directly
   - document why a donor decision is accepted, adapted, or rejected

### Required method
For each donor replay candidate:
- identify the target subsystem/boundary
- identify whether the baseline is ready
- compare donor assumptions against baseline reality
- land the replay as a bounded batch with docs and verification

### What counts as done enough
- donor work is entering through named queues instead of ad hoc memory
- the repo has a repeatable replay process
- design convergence is happening even before full code convergence

---

## Milestone 5 — Product-shape preparation

### Goal
Prepare for real client/FDS/tools modernization without pretending it is already solved.

### Focus areas
- `Commando` ownership map
- client vs FDS split rules
- scripts/plugin boundary rules
- tool grouping by shared content/runtime dependencies
- renderer/audio/bootstrap ownership

### What counts as done enough
- we can describe how products will assemble from cleaner subsystem boundaries
- future client/FDS work no longer depends on guessing where ownership lives

---

## Milestone 6 — Future-facing modernization readiness

### Goal
Reach a state where major future changes become realistic engineering projects.

### Examples of future work this plan is intended to enable
- renderer backend replacement or layering
- DX12/Vulkan-era rendering work
- shader modernization
- physics engine replacement or deep refit
- cleaner platform abstraction
- broader non-Windows support

### Important note
This milestone is about **making those futures realistic**, not promising them immediately.

---

## Decision rules for choosing the next batch

When picking work, prefer this order:

1. **Keep the base trustworthy**
   - green scaffold
   - green doc sync
   - current docs

2. **Pick the highest-leverage blocker**
   - something that unlocks multiple later paths

3. **Prefer mergeability gains over neat local cleanup**
   - choose work that makes future donor replay or subsystem replacement easier

4. **Prefer truth over motion**
   - an honest defer/classification batch beats a fake implementation

5. **Prefer bounded subsystem slices**
   - do not open five architecture battles at once

---

## Concrete work lanes from here

### Lane A — Current-truth maintenance
Keep docs, roadmap, workflow, and verification current.

### Lane B — Platform seam completion in `wwlib`
Continue only where the seam can be landed honestly and helps future portability/mergeability.

### Lane C — Renderer/runtime untangling
Focus on the `ww3d2` math/backend split and downstream dependency fallout in `Combat` and `wwphys`.

### Lane D — Product-shell ownership mapping
Clarify `Commando`, FDS, scripts, and online/service ownership before product ambitions expand.

### Lane E — Donor replay queue building
Create a structured queue of OpenW3D/w3dhub commits and decisions worth replaying later by subsystem.

---

## Immediate next practical steps

1. keep the doc/build base clean and trustworthy
2. finish any remaining Batch 028 doc-only classification updates if still incomplete
3. choose the next high-leverage blocker based on mergeability value, not novelty
4. begin a structured donor replay queue by subsystem:
   - renderer
   - platform/SDL seams
   - product shell
   - scripts
   - tools

---

## What to ask before any significant change

Before landing a meaningful batch, ask:
- does this reduce hard coupling?
- does this preserve or improve future mergeability?
- does this help future renderer/shader/physics modernization?
- is this a real seam or just a local patch?
- are we changing architecture truth, or just making the docs prettier?

If the answer is weak, it is probably not the right next batch.

---

## Final framing

This project is restoring and restructuring an old engine so it can have a future.

The near-term work is conservative.
The long-term aim is ambitious:
- truthful modernization
- subsystem separability
- donor decision absorption
- eventual convergence toward `w3dhub/OpenW3D`

That is the plan.