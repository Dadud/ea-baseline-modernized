# Commando Dependency-Prep Plan

> **For Hermes:** Use the documented EA-baseline-first workflow. Do not attempt a direct `commando` product build from this plan alone.

**Goal:** Prepare the first `commando` ingestion batch by documenting known dependency and product-shell seams exposed by prior subsystem mapping.

**Architecture:** Treat `commando` as the first true product-shell target above the currently mapped subsystem seams. Preserve original VC6 project identity while documenting the needed logical split between client shell, FDS mode, online glue, script/plugin boundary, and backend ownership.

**Tech Stack:** Markdown docs, VC6 manifest/docs already generated, conservative CMake modernization workflow.

---

### Task 1: Collect upstream dependency evidence
- Read the original `commando` dependency list from `docs/architecture/original-project-inventory.md`.
- Read the latest `Combat` and `Scripts` batch docs.
- Confirm the currently documented blockers that matter for `commando` prep.

### Task 2: Write the prep doc
- Create `docs/architecture/commando-dependency-prep.md`.
- Record the original dependency shape.
- Explain why `commando` is a product-shell target rather than a normal subsystem seam.
- Capture the already-known `Combat` and `Scripts` complications.
- Propose first-pass source-island vocabulary for a later `commando` CMake scaffold.

### Task 3: Write the batch report
- Create `docs/build/foundation-portability-batch-016.md`.
- Explain why this batch is documentation-only.
- State clearly that no `commando` target was added yet.
- Record the expected next implementation batch.

### Task 4: Update status docs
- Update `README.md` to link the new prep doc.
- Update `docs/architecture/modernization-progress-roadmap.md` to make Batch 016 the latest completed batch.
- Update `docs/architecture/foundation-target-map.md` with the new near-term recommendation.
- Update `docs/architecture/original-project-scaffold-status.md` to show `commando` has prep docs even though it is not modeled.
- Update `docs/parity/target-status.md` so reporting remains explicit.

### Task 5: Verify and commit
- Run `git diff --check`.
- Ensure no target/build claims changed incorrectly.
- Commit docs only.
- Push to `ea-modernized/main`.
