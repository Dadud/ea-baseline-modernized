# AI Bootstrap Prompt — EA-Baseline Modernization

Use this when starting a fresh AI session on this repo.

```text
You are continuing the EA-baseline-first modernization of Renegade/OpenW3D.

Primary repo (truth): <EA_BASELINE_REPO_ROOT>
Donor/reference repo: <DONOR_OPENW3D_REPO_ROOT>
Active branch: main
Latest known documentation handoff commit: 3e60dbb docs: add compact AI bootstrap prompt
Latest known implementation seam commit: 75bf5df build: add opt-in SDL3 mutex seam for wwlib

Path discovery rules:
- Do not assume `/tmp` or any fixed directory layout.
- If you are already somewhere inside the EA baseline checkout, identify the repo root with: git rev-parse --show-toplevel
- Treat that result as <EA_BASELINE_REPO_ROOT>
- If you are not inside a checkout yet, find or clone the EA baseline repo first, then cd into it before running project commands
- If the donor repo path is unknown, ask the operator or locate/clone it and treat its root as <DONOR_OPENW3D_REPO_ROOT>
- Build directories are local choices; `build/cmake-scaffold` and `build/cmake-scaffold-sdl3` are conventions, not mandatory absolute paths

Machine bootstrap dependencies:
- Required minimum: git, python3, cmake, and a working C/C++ compiler toolchain
- Recommended on UNIX-like systems: pkg-config
- Optional for current SDL3 seam verification: SDL3 development package/headers/library
- Optional Windows-era SDKs for broader product work: DirectX 8 SDK, Miles Sound System SDK, RAD Bink SDK, plus other SDKs noted in README.md
- Do not assume optional SDKs exist; detect what is available first and keep the batch bounded if they are absent


Primary strategy:
- Modernize the original EA Renegade baseline first.
- Preserve client/FDS/tools parity expectations.
- Keep logical boundaries and documentation ahead of physical file reorganization.
- Use OpenW3D/forks/PRs as donor/reference only, not truth.
- Continue in bounded, reviewable batches.

First commands to run:
1. git rev-parse --show-toplevel
2. git status --short --branch
3. python3 scripts/architecture/check_doc_sync.py
4. cmake --build build/cmake-scaffold -j4
5. cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4

Mandatory reading order:
1. docs/architecture/ai-handoff-guide.md
2. docs/architecture/modernization-progress-roadmap.md
3. docs/architecture/modernization-strategy-v2.md
4. docs/parity/target-status.md
5. docs/architecture/deferred-source-ledger.md
6. docs/architecture/platform-boundary-contracts.md
7. docs/architecture/project-scaffold-status.md
8. build-manifests/vc6-projects.json
9. docs/build/project-ingestion-and-probe-workflow.md
10. .hermes/plans/2026-04-23_093416-modernization-progress-review-and-next-plan.md
11. .hermes/plans/2026-04-23_101900-commando-dependency-prep.md
12. .hermes/plans/2026-04-23_102700-batch-forecast-roadmap.md
13. docs/architecture/w3dhub-port-evaluation.md

Current default green scaffold targets:
- wwdebug
- wwmath
- wwbitpack
- wwsaveload
- wwtranslatedb
- wwlib
- wwutil
- wwnet
- BinkMovie
- SControl

Current SDL3-backed wwlib seams under RENEGADE_USE_SDL3=ON:
- Batch 023: thread.cpp
- Batch 024: rawfile.cpp
- Batch 025: rcfile.cpp
- Batch 026: verchk.cpp (partial)
- Batch 027: mutex.cpp

Important truth:
- Compile success is NOT parity proof.
- Do not hide real seams with fake SDK/platform headers.
- Do not physically reorganize files yet.
- Do not jump straight into broad Commando product work unless explicitly required.
- Do not replay unrelated OpenW3D fork baggage.
- Keep Windows behavior intact unless a deviation is documented.

Key current open platform items in wwlib:
- cpudetect.cpp
- mpu.cpp
- registry.cpp
- win.cpp
- mixfile.cpp
- ini.cpp
- widestring.cpp
- FastCriticalSectionClass modernization
- full non-Windows PE version-resource extraction in verchk.cpp
- generated embedded-resource population for rcfile.cpp

Best next recommended batch:
- Batch 028: audit cpudetect.cpp / mpu.cpp as the next CPU/platform seam decision.
- Determine whether either file has a safe portable subset, should remain deferred, or can be re-enabled in a bounded way.

Required verification before committing any new batch:
- git diff --check
- python3 scripts/architecture/check_doc_sync.py
- cmake --build build/cmake-scaffold-sdl3 --target wwlib -j4
- cmake --build build/cmake-scaffold -j4

Required docs to sync whenever seam state changes:
- docs/architecture/deferred-source-ledger.md
- docs/architecture/platform-boundary-contracts.md
- docs/architecture/project-scaffold-status.md
- docs/architecture/modernization-progress-roadmap.md

If unsure whether a change is too broad, choose the smaller batch.
```

## Related docs
- Full handoff: `docs/architecture/ai-handoff-guide.md`
- Strategy: `docs/architecture/modernization-strategy-v2.md`
- Roadmap: `docs/architecture/modernization-progress-roadmap.md`
- Deferred ledger: `docs/architecture/deferred-source-ledger.md`
