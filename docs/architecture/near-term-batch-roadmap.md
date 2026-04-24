# Near-Term Batch Roadmap — Next 5 to 10 Batches

## Purpose

This document turns the upstream-convergence plan into a practical near-term execution order.

It answers:
- what should happen next
- why those batches matter
- what each batch is trying to unlock
- how to choose between equally interesting work

This is a **ranking**, not a rigid script. If a batch reveals a more important blocker, the order can change. The main rule is to optimize for:
- truthful boundaries
- future mergeability
- future subsystem replacement readiness
- eventual convergence toward `w3dhub/OpenW3D`

---

## Ranking criteria

Batches are ranked by:

1. **Unlock value**
   - does this open multiple later paths?
2. **Mergeability value**
   - does this make future donor replay/upstream convergence easier?
3. **Boundary clarity**
   - does this expose a real seam instead of hiding it?
4. **Batch boundedness**
   - can it be landed honestly in reviewable slices?
5. **Verification clarity**
   - can success or honest deferment be proven?

---

## Ranked roadmap

## 1. Batch 029 — `ww3d2` / `Combat` / `wwphys` leakage inventory

### Goal
Produce one tightly scoped inventory of the renderer-header bleed that currently blocks runtime and physics work.

### Why first
Two independent ranking passes converged on the same point:
- the core blocker is the `ww3d2` math/backend tangle
- but some of the next useful cuts are even more concrete: `phys.cpp`, `phys3.cpp`, `combat.cpp`, `WeatherMgr.cpp`, `backgroundmgr.cpp` still pull DX8-era headers directly

Before extraction or replay, we want one accurate map of:
- which includes are purely backend
- which are math/runtime-adjacent
- which files are misbucketed today

### Deliverables
- concrete file/include map for the worst offenders
- classification of each offender as:
  - backend-only leakage
  - runtime/math leakage
  - presentation leakage
  - product-shell leakage
- recommendation for the next smallest implementation cuts

### Unlocks
- makes the next implementation batches surgical instead of fuzzy
- grounds the renderer/runtime untangling in actual files

---

## 2. Batch 030 — `ww3d2` math/backend boundary inventory and extraction plan

### Goal
Document and classify the split between reusable math/content-facing types and hard renderer/backend code inside `ww3d2`.

### Why this high
This is the central blocker for:
- `Combat`
- `wwphys`
- future renderer replacement
- future modern graphics work

If we do not untangle this, DX12/Vulkan/shader modernization stays fantasy.

### Deliverables
- source/header inventory for math-vs-backend-vs-runtime leakage
- include dependency map for the worst offenders
- proposed extraction plan for a future `engine_math`-style boundary
- honest statement of what can move later and what must stay put for now

### Unlocks
- renderer modernization readiness
- gameplay/runtime portability
- future backend swap planning

---

## 3. Batch 031 — first leakage-cut implementation slice (`wwphys` + `Combat`)

### Goal
Land the first bounded source-level cuts that remove obviously misplaced DX8 header dependence from runtime-oriented files.

### Likely starting points
- `Code/wwphys/phys.cpp`
- `Code/wwphys/phys3.cpp`
- `Code/Combat/combat.cpp`
- `Code/Combat/WeatherMgr.cpp`
- `Code/Combat/backgroundmgr.cpp`

### Why here
This turns the architecture work into real code motion without overreaching into giant subsystem rewrites.

### Deliverables
- guarded or relocated renderer-header dependencies where they clearly do not belong in runtime-facing code
- matching doc/ledger updates
- smallest meaningful target verification

### Unlocks
- makes `wwphys` and `Combat` less entangled with renderer backend assumptions
- proves the convergence-oriented untangling workflow on real code

---

## 4. Batch 032 — Donor replay queue by subsystem

### Goal
Create a structured, documented queue of OpenW3D/w3dhub commits and design decisions worth replaying later.

### Why this early
Right now donor work exists mostly as remembered context, scattered docs, and ad hoc comparisons. If eventual convergence is a real goal, donor intake needs structure.

### Deliverables
- subsystem-based replay queue doc
- categories such as:
  - platform/SDL seams
  - renderer/backend wrappers
  - audio backend work
  - scripts/runtime
  - product shell
  - tools
- per queue item:
  - donor source
  - affected subsystem
  - baseline readiness
  - replay risk
  - recommendation: adopt / adapt / defer / reject

### Unlocks
- intentional donor absorption
- better mergeability decisions
- less rediscovery later

---

## 5. Batch 033 — renderer-facing boundary contract

### Goal
Write the concrete boundary contract that future renderer work must obey.

### Why here
Once `ww3d2`, `Combat`, and `wwphys` leakage is better mapped, we need an actual contract that future donor replay and future rendering modernization can target.

### Deliverables
- renderer boundary contract doc update or new focused contract
- distinction between:
  - renderer math/content types
  - renderer backend/device layer
  - scene/runtime bridge
  - product/presentation integration

### Unlocks
- future DX12/Vulkan feasibility work
- shader modernization planning
- donor renderer decision absorption

---

## 6. Batch 034 — `Commando` ownership map refresh

### Goal
Turn `Commando` from "giant scary product shell" into a clearer ownership map.

### Why this matters
Real client/FDS modernization and eventual convergence will fail if `Commando` remains a black box.

### Deliverables
- refreshed ownership map for:
  - client shell
  - FDS mode
  - online/service glue
  - config/dev shell
  - renderer/audio/bootstrap ownership
- explicit list of which `Commando` dependencies should move downward versus remain product-level

### Unlocks
- client vs FDS planning
- product-shape prep
- less guesswork around later donor replay

---

## 7. Batch 035 — `wwui` text/input/platform boundary audit

### Goal
Clarify what in `wwui` is:
- reusable UI/control logic
- text/IME/platform glue
- renderer presentation glue
- client-only product UI

### Why it matters
Future client modernization and upstream convergence need a believable story for text/input/UI ownership.

### Deliverables
- `wwui` source-island map
- boundary proposal for text/input/platform separation
- honest defer/classification for IME and Win32-heavy pieces

### Unlocks
- future client UI modernization
- cleaner input boundary
- better product-shell separation

---

## 8. Batch 036 — `WWAudio` backend decision map

### Goal
Decide how to treat audio modernization and donor audio work.

### Why now
Audio is a major future modernization lane, and donor OpenW3D work already contains meaningful backend decisions. We need a decision map before touching implementation.

### Deliverables
- document Miles-bound surfaces vs backend-neutral surfaces
- compare donor OpenAL/FFMpeg direction against baseline needs
- define what a future audio backend boundary must look like
- decide what donor audio work is likely adopt/adapt/defer/reject

### Unlocks
- future audio backend replacement
- better donor replay planning
- less risk of ad hoc audio divergence

---

## 9. Batch 037 — next `wwlib` high-value seam or classification slice

### Goal
Land one bounded `wwlib` slice only if it improves real portability/mergeability.

### Candidate areas
- `registry.cpp`
- `win.cpp`
- `mixfile.cpp`
- `ini.cpp`
- `widestring.cpp`
- `mono.cpp`
- `msgloop.cpp` / input shell classification

### Rule
Do not do this just because it is available. Only do the slice that best supports future subsystem untangling or donor replay.

### Unlocks
- incremental platform seam maturity
- better support for later product work

---

## 10. Batch 038 — scripts/runtime and plugin boundary prep

### Goal
Clarify how `Scripts` should relate to runtime, DLL/plugin ownership, and product assembly.

### Why this matters
Scripts sit near the product/runtime seam and matter for eventual client/FDS convergence.

### Deliverables
- scripts runtime-core vs DLL-entry split refresh
- plugin boundary notes
- future product assembly implications

### Unlocks
- product assembly planning
- less ambiguity around script system modernization

---

## 11. Batch 039 — first donor replay execution batch

### Goal
Land the first explicitly mergeability-driven donor replay batch from the subsystem queue.

### Requirements before doing this
- donor queue exists
- target boundary is understood
- replay choice is justified
- verification path is clear

### Good candidates
- a narrow SDL/platform seam replay
- a small boundary-shaping refactor with strong donor precedent
- a non-invasive helper layer that reduces later divergence

### Unlocks
- proves the convergence workflow is real
- turns strategy into execution

---

## What can jump the queue

A lower-ranked batch can move up if it:
- removes a critical false assumption in current docs
- unblocks multiple higher-ranked items
- has unusually strong donor/upstream convergence value
- reveals the current ranking is based on a wrong model

---

## What should not jump the queue

Avoid prioritizing these just because they are exciting:
- DX12 experiments
- ray tracing experiments
- broad shader rewrites
- physics engine replacement
- giant repo reorganization
- broad `Commando` build forcing

Those are downstream outcomes of this roadmap, not the next batch.

---

## Recommended immediate start

If starting now, do this order:
1. Batch 029 — `ww3d2` / `Combat` / `wwphys` leakage inventory
2. Batch 030 — `ww3d2` math/backend boundary inventory and extraction plan
3. Batch 031 — first leakage-cut implementation slice
4. Batch 032 — donor replay queue by subsystem

That sequence gives the best balance of:
- concrete blocker clarity
- real untangling progress
- convergence planning
- future modernization payoff

---

## Final note

The near-term goal is not to look busy.
It is to make future big changes possible without wrecking mergeability, truthfulness, or the original baseline story.