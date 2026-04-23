# Project Ingestion and Probe Workflow

This workflow standardizes how original EA Renegade VC6 projects are added to the baseline CMake scaffold.

## Inputs

- `build-manifests/vc6-projects.json`
- `build-manifests/vc6-projects.md`
- original `.dsp`/`.dsw` files
- current CMake scaffold

## Workflow

1. Inspect the original project in the manifest.
2. Preserve the original target name.
3. Generate or hand-create a descriptive CMake scaffold from manifest data.
4. Keep source inventory visible.
5. Classify sources into logical islands before broad patching.
6. Configure the scaffold.
7. Probe the target build.
8. Classify the first blocker.
9. Decide:
   - narrow compatibility patch
   - source island deferment
   - boundary design required
10. Record deferred sources in `docs/architecture/deferred-source-ledger.md`.
11. Record parity state separately from compile state.
12. Verify and commit.

## Standard commands

```bash
cd /tmp/openw3d-ea-baseline
python3 scripts/projects/parse_vc6_workspace.py --root . --output build-manifests/vc6-projects.json
python3 scripts/projects/write_project_inventory.py build-manifests/vc6-projects.json --output build-manifests/vc6-projects.md
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold --target TARGET_NAME -j4
```

For long errors:

```bash
cmake --build build/cmake-scaffold --target TARGET_NAME -j4 >/tmp/TARGET_NAME-build.log 2>&1
rc=$?
tail -n 120 /tmp/TARGET_NAME-build.log
exit $rc
```

Full verification:

```bash
git diff --check
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

## Patch versus defer policy

### Patch if narrow and behavior-preserving

Examples:

- include case mismatch
- Windows path separator in include
- old MSVC header spelling
- local `_MSC_VER` guard
- centralized spelling bridge for old CRT names
- template lookup fix under modern compilers

### Defer if architectural

Examples:

- DirectDraw/Direct3D
- DirectSound/Miles
- WinSock implementation
- RAD/Bink SDK
- GameSpy/WOL
- MFC/ATL/COM-heavy product code
- renderer/UI/product-shell coupling

## Reporting format

Each target batch should report:

```text
Target:
Original VC6 project:
Future bucket guess:
Source islands:
Compiled islands:
Deferred islands:
First blocker(s):
Patch/defer decisions:
Scaffold verification:
Parity status:
```

## Skill references

Use these skills for future work:

- `openw3d-vc6-target-ingestion`
- `openw3d-source-island-classification`
- `openw3d-deferred-source-ledger`
- `openw3d-build-probe-triage`
- `openw3d-parity-evidence`
