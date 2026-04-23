# Batch 019 — Automation Tooling and Multi-Target Probe

## What this batch does

Batch 019 does not add a new target or change any source code. It adds the tooling infrastructure that makes future batches faster and more systematic, then demonstrates it by running the first structured multi-target probe across all unmapped subsystems.

## Scripts added

### `scripts/projects/write_cmake_scaffold.py`

Generates a conservative CMake scaffold from the parsed VC6 manifest for any target. Usage:

```bash
python3 scripts/projects/write_cmake_scaffold.py WWAudio --root .
python3 scripts/projects/write_cmake_scaffold.py WWAudio --root . --write --force
```

Input: `build-manifests/vc6-projects.json`  
Output: `Code/<Target>/CMakeLists.txt` using `ea_vc6_static_library()` helper

This automates the boring scaffold boilerplate so developers can focus on source-island classification and narrow compatibility fixes.

### `scripts/build/probe_targets.py`

Probes one or more targets using static signal scanning and optional CMake build. Generates a Markdown report. Usage:

```bash
python3 scripts/build/probe_targets.py WWAudio SControl Combat --root . --output probe-report.md
python3 scripts/build/probe_targets.py WWAudio --root . --build
```

The signal patterns used:

| Category | Pattern |
|---|---|
| DirectDraw/Direct3D | `ddraw`, `d3d`, `Direct3D`, `IDirect3D`, `dx8` |
| DirectSound/Miles | `dsound`, `Miles`, `mss`, `AIL_`, `DirectSound` |
| WinSock/network | `winsock`, `SOCKET`, `WSA`, `sendto`, `recvfrom`, `closesocket` |
| RAD/Bink/media SDK | `Bink`, `RAD Game Tools`, `HBINK` |
| GameSpy/WOL/online | `GameSpy`, `WOL`, `WWOnline`, `WOLBrowser` |
| MFC/ATL/COM | `afxwin`, `atlbase`, `IUnknown`, `CoCreate`, `HRESULT` |
| Win32 window/message | `windows.h`, `HWND`, `PeekMessage`, `DispatchMessage`, `ShellExecute` |
| Win32 registry | `RegOpen`, `RegQuery`, `HKEY_`, `GetFileVersionInfo` |
| Threading/sync | `CreateThread`, `CRITICAL_SECTION`, `WaitForSingleObject`, `HANDLE` |
| String/encoding | `WCHAR`, `wcsicmp`, `WideString`, `MultiByteToWideChar` |
| Old MSVC spelling | `stricmp`, `strcmpi`, `_vsnprintf`, `__int64`, `iostream.h`, `new.h` |

## Probe results

The probe was run against all unmapped subsystems:

| Target | Type | Sources | Missing | Top blocker categories | Build status |
|---|---|---:|---:|---|---|
| `WWAudio` | Win32 static lib | 43 | 0 | threading/sync (127), Miles audio (51), Win32 msg loop (2) | not built |
| `SControl` | Win32 static lib | 4 | 0 | WinSock (60), threading/sync (5) | not built |
| `Combat` | Win32 static lib | 265 | 0 | string/encoding (61), old MSVC (54), threading (43), MFC/COM (20) | not built |
| `ww3d2` | Win32 static lib | 233 | 5 | DirectX (133), string/encoding (81), threading (44), old MSVC (35) | not built |
| `wwphys` | Win32 static lib | 193 | 0 | old MSVC (48), threading (28), Miles audio (4), Win32 msg (3) | not built |
| `wwui` | Win32 static lib | 84 | 0 | string/encoding (158), Win32 msg loop (42), threading (11), WinSock (4) | not built |
| `Scripts` | Win32 DLL | 82 | 1 | old MSVC (99), threading (15), Win32 msg (4) | not built |

### Key findings

**WWAudio** is the most threading-heavy target (127 signal hits). All threading is tied to Miles/DirectSound audio backends — not separable without a platform audio boundary.

**wwui** has the highest string/encoding signal (158 hits). This is expected for a UI text system with `WCHAR` throughout.

**ww3d2** has the highest DirectX signal (133 hits) and 5 missing files (shader files, `.INI`, `.h` without `.cpp`). This is the most renderer-blocked target.

**Combat** has 265 sources — the largest gameplay bucket. 54 old-MSVC signal hits and 43 threading signals indicate moderate portability risk, but the core is C++ gameplay logic.

**SControl** has 4 sources but 60 WinSock signal hits — it is purely a socket transport target.

**Scripts** has 1 missing file (`slnode.h`) and 99 old-MSVC signal hits — DLL entry and debug print infrastructure.

## What this enables

With these scripts in place, future batches can:

1. Generate a scaffold for a new target in one command instead of hand-writing CMake
2. Run a structured probe before deciding what to tackle next
3. Track deferred-source debt in a machine-generated table instead of hand-maintained lists
4. Use signal patterns to prioritize where to spend time on narrow compatibility fixes

## Files added

- `scripts/projects/write_cmake_scaffold.py` — CMake scaffold generator
- `scripts/build/probe_targets.py` — target probe and classifier
- `docs/architecture/target-probe-report.md` — probe results for WWAudio, SControl, Combat, ww3d2, wwphys, wwui, Scripts

## Files modified

- `docs/architecture/modernization-progress-roadmap.md` — updated Batch 019 entry
- `docs/architecture/deferred-source-ledger.md` — updated with probe signal data

## Scaffold impact

None — no source code or CMake targets changed. Default scaffold still builds all 10 targets green.
