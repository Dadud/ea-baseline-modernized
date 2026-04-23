# Batch 017: `commando` product-shell seam mapping

## Goal

Model the original EA `Code/Commando/commando.dsp` target conservatively as an **opt-in product-shell seam**, keeping the default Linux bootstrap scaffold green while making the historical client/FDS/online/frontend ownership inside `commando` explicit.

This batch does **not** claim a portable client executable, FDS build, or product parity. It records the first useful configure/probe evidence for the main Renegade product shell.

## Added CMake target

`Code/Commando/CMakeLists.txt` now models the original VC6 project name unchanged:

```text
commando
```

The target is gated behind:

```text
RENEGADE_BUILD_COMMANDO_SEAMS
```

Example configure command used for the probe:

```bash
cmake -S . -B build/cmake-commando-probe \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_AUDIO_SEAMS=ON \
  -DRENEGADE_BUILD_PHYS_SEAMS=ON \
  -DRENEGADE_BUILD_UI_SEAMS=ON \
  -DRENEGADE_BUILD_RENDERER_SEAMS=ON \
  -DRENEGADE_BUILD_COMBAT_SEAMS=ON \
  -DRENEGADE_BUILD_SCRIPT_SEAMS=ON \
  -DRENEGADE_BUILD_COMMANDO_SEAMS=ON
```

## Source-island classification

The original source inventory is grouped into these first-pass logical islands:

```text
COMMANDO_PRODUCT_STARTUP_SOURCES
COMMANDO_CLIENT_FRONTEND_UI_SOURCES
COMMANDO_CLIENT_RUNTIME_ORCHESTRATION_SOURCES
COMMANDO_FDS_MODE_SOURCES
COMMANDO_ONLINE_SERVICE_GLUE_SOURCES
COMMANDO_CONFIG_SHELL_SOURCES
COMMANDO_RENDER_AUDIO_OWNERSHIP_SOURCES
COMMANDO_LOCAL_SUPPORT_SOURCES
```

Interpretation:

- `COMMANDO_PRODUCT_STARTUP_SOURCES` — RC/icon/text artifacts and product-startup miscellany.
- `COMMANDO_CLIENT_FRONTEND_UI_SOURCES` — dialog shells, HUD/menu/UI ownership, EVA encyclopedia, chat/login dialogs, and other frontend presentation glue.
- `COMMANDO_CLIENT_RUNTIME_ORCHESTRATION_SOURCES` — gameplay shell wiring, player/team/session orchestration, game mode wrappers, packet/application-level runtime objects, and shared product-state management.
- `COMMANDO_FDS_MODE_SOURCES` — startup/shutdown/autostart, server/client FPS shells, and explicit dedicated-server mode overlays.
- `COMMANDO_ONLINE_SERVICE_GLUE_SOURCES` — WOL/GameSpy/NAT/chat/quickmatch/buddy/game-channel glue.
- `COMMANDO_CONFIG_SHELL_SOURCES` — registry/config/dev/console/system settings ownership.
- `COMMANDO_RENDER_AUDIO_OWNERSHIP_SOURCES` — main loop/init/video/audio/browser ownership and related bootstrap surfaces.
- `COMMANDO_LOCAL_SUPPORT_SOURCES` — local chunk IDs / product IDs / tiny support headers.

## Non-Windows handling

On non-Windows seam probes, the scaffold keeps the full source inventory visible but defers:

```text
COMMANDO_PRODUCT_STARTUP_SOURCES
COMMANDO_CLIENT_FRONTEND_UI_SOURCES
COMMANDO_FDS_MODE_SOURCES
COMMANDO_ONLINE_SERVICE_GLUE_SOURCES
COMMANDO_RENDER_AUDIO_OWNERSHIP_SOURCES
```

This lets the first probe test whether any meaningful product-shell/runtime subset exists below the already-known WOL/GameSpy/UI/DirectX/Miles/Bink-heavy areas.

## Narrow fixes applied before trusting the blockers

The first configure/build attempt exposed a small set of Linux path-identity issues before the real product-shell blockers were visible. These were corrected as narrow, behavior-preserving fixes:

- `Code/Commando/CMakeLists.txt` include-dir casing corrected:
  - `../combat` -> `../Combat`
  - `../wwaudio` -> `../WWAudio`
  - `../wwmath` -> `../WWMath`
  - `../scontrol` -> `../SControl`
- `Code/Combat/damage.h` path separator/casing corrected:
  - `..\commando\datasafe.h` -> `../Commando/datasafe.h`
- `Code/Combat/playerdata.h` path separator/casing corrected:
  - `..\commando\datasafe.h` -> `../Commando/datasafe.h`
- `Code/Combat/weaponmanager.h` path separator/casing corrected:
  - `..\commando\datasafe.h` -> `../Commando/datasafe.h`

These changes were made only to get past Windows path assumptions and reveal the real subsystem seams underneath.

## Probe result

Targeted probe command:

```bash
cmake --build build/cmake-commando-probe --target commando -j4
```

After the narrow include/path cleanup, the next meaningful blocker was:

### Blocker 1: reverse product-shell coupling through `Combat` into `datasafe.h`

```text
/tmp/openw3d-ea-baseline/Code/Commando/../Combat/../Commando/datasafe.h:143:10: fatal error: windows.h: No such file or directory
```

This is more useful than the earlier raw `..\commando\datasafe.h` path error because it shows the problem is not merely that `Combat` reaches back into `Commando`; it is that the product-shell-owned `datasafe.h` itself still drags in Win32 process/thread primitives.

That makes the current dependency shape effectively:

```text
commando -> Combat -> datasafe.h -> <windows.h>
```

not a clean layering where `Combat` sits below a narrow platform-neutral product boundary.

### Secondary configure-time finding: historical project metadata includes absent MFC support header

Configure also records:

```text
commando: skipping manifest source that is absent from EA release tree: ./afxres.h
```

This is consistent with other manifest-era public release gaps. It is not the main architecture blocker, but it is now documented for the `commando` target.

## Architectural conclusion

`commando` is **not** just a main executable wrapper. The current EA bucket mixes at least these layers:

- client/frontend/UI shell ownership
- product/runtime orchestration and app-level gameplay shell code
- dedicated-server mode overlays
- online/WOL/GameSpy/NAT/service glue
- config/dev/console ownership
- render/audio/browser/bootstrap ownership

Even after deferring the obvious frontend, online, FDS, and render/audio islands, the remaining runtime-oriented subset still collapses into Win32-heavy product-shell ownership through `datasafe.h`.

So the current batch records `commando` as a **mixed products/client + products/dedicated_server + online_services + input/config shell seam**, not a buildable product target.

## What this batch intentionally does not do

- no claim that `commando` builds on Linux
- no claim of client parity
- no claim of FDS parity
- no fake `windows.h`, GameSpy, Miles, DirectX, or WOL stubs
- no physical source moves
- no premature split into final client/FDS executables

## Resulting status

- `commando` is now **modeled**.
- `commando` is **not** part of the default green scaffold.
- `commando` currently has **configure/probe evidence and documented blockers**, not build or parity evidence.
