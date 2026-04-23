# Foundation Portability Batch 008: Automation and Target Probe Workflow

## Purpose

Batch 008 turns the previous manual target-by-target scaffold workflow into a repeatable ingestion and triage workflow.

This batch does **not** add a new compiled target to the default scaffold. Instead, it adds automation for future target ingestion and generates an evidence-based probe report for the next candidates.

## Added scripts

### `scripts/projects/write_cmake_scaffold.py`

Generates a conservative `CMakeLists.txt` scaffold from `build-manifests/vc6-projects.json` for a named original VC6 project.

Example dry-run:

```bash
python3 scripts/projects/write_cmake_scaffold.py SControl
```

Example write mode:

```bash
python3 scripts/projects/write_cmake_scaffold.py SControl --write
```

Design rules:

- preserve the original VC6 target name
- preserve manifest source membership
- carry manifest include paths and defines
- do not rename targets to future architecture bucket names
- do not classify final architecture automatically
- refuse to overwrite an existing `CMakeLists.txt` unless `--force` is provided

### `scripts/build/probe_targets.py`

Scans target source files for known modernization blocker signals and can optionally invoke CMake target builds for already-modeled targets.

Example static triage:

```bash
python3 scripts/build/probe_targets.py SControl WWAudio Scripts Combat ww3d2 wwphys wwui \
  --output docs/build/target-probe-report-batch-008.md
```

Example build probe for existing targets:

```bash
python3 scripts/build/probe_targets.py BinkMovie wwnet --build
```

Signal categories include:

- DirectDraw/Direct3D
- DirectSound/Miles/audio backend
- WinSock/network transport
- RAD/Bink/media SDK
- GameSpy/WOL/online service
- MFC/ATL/COM
- Win32 window/message loop
- Win32 registry/config/version
- threading/synchronization
- string/encoding/WCHAR
- old MSVC/C runtime spelling

## Generated target probe report

Batch 008 generated:

```text
docs/build/target-probe-report-batch-008.md
```

Targets scanned:

- `SControl`
- `WWAudio`
- `Scripts`
- `Combat`
- `ww3d2`
- `wwphys`
- `wwui`

## Probe findings summary

| Target | Main finding | Recommended next use |
|---|---|---|
| `SControl` | Very small, mostly WinSock/socket control seam. | Good next scaffold target if we want a contained network/platform boundary. |
| `WWAudio` | Audio backend plus heavy threading/synchronization signals. | Good early audio-boundary classification target, but not a simple portability target. |
| `Scripts` | DLL project with one missing manifest source and many old MSVC/CRT spellings. | Useful for scripts/product-plugin parity planning after executable/shared-library helpers exist. |
| `Combat` | Large gameplay/runtime bucket with string, threading, COM-ish, window, and old CRT signals. | Do not jump to build yet; inventory/classify first. |
| `ww3d2` | Renderer-heavy target with Direct3D, missing manifest assets/sources, string, and threading signals. | Renderer boundary mapping target, not early compile target. |
| `wwphys` | Mixed physics/runtime with old CRT, threading, and render/audio-adjacent leakage. | Inventory/classify before build. |
| `wwui` | UI/input target dominated by `WCHAR` and Win32 message/input/IME signals. | Input/UI boundary mapping target. |

## Strategy impact

The probe confirms that the next implementation batch should remain bounded. `SControl` is the smallest next target, but it is still a network/platform seam rather than a pure foundation library. `WWAudio` is valuable but exposes a larger audio/threading boundary.

Recommended next sequence:

1. Add `SControl` scaffold using the generator.
2. Classify it as a server-control/network platform seam.
3. Avoid implementing a fake socket layer.
4. Then use the probe workflow to choose between `WWAudio` boundary classification and broader subsystem inventory.

## Verification

Batch 008 should not change default scaffold target membership. The expected verification remains:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Expected current scaffold targets:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`
- `wwlib`
- `wwutil`
- `wwnet`
- `BinkMovie`
