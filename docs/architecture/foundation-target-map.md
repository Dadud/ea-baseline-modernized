# Foundation target map

This document maps the original EA Visual C++ 6 support projects into the modernization program's future `foundation` bucket.

The current rule is **logical mapping first, physical moves later**. Original project names remain CMake targets until source membership, target boundaries, and platform seams are understood.

## Current scaffold targets

| Original project | CMake target | Current status | Future bucket notes |
| --- | --- | --- | --- |
| `Code/wwdebug/wwdebug.dsp` | `wwdebug` | Builds in the foundation scaffold | Debug/assert/log helpers. Some platform-specific output paths remain as compatibility shims. |
| `Code/WWMath/WWMath.dsp` | `wwmath` | Builds in the foundation scaffold | Math primitives. Manifest still references absent `Vector3i.h`; default scaffold warns/skips missing files. |
| `Code/wwbitpack/wwbitpack.dsp` | `wwbitpack` | Builds in the foundation scaffold | Bit packing/serialization helpers. Contains documented 16-bit Win32 `WCHAR` compatibility seam. |
| `Code/wwsaveload/wwsaveload.dsp` | `wwsaveload` | Builds in the foundation scaffold | Persistence and save/load primitives. |
| `Code/wwtranslatedb/wwtranslatedb.dsp` | `wwtranslatedb` | Builds in the foundation scaffold | Translation database support. Current non-Windows file API bridge is a scaffold shim, not final platform architecture. |
| `Code/wwutil/wwutil.dsp` | `wwutil` | Builds in the foundation scaffold | Small support utilities. Windows stack walking is no-op on non-Windows scaffold builds. |
| `Code/wwlib/wwlib.dsp` | `wwlib` | Builds in the non-Windows foundation scaffold after source-island classification | Mixed historical bucket. Current non-Windows scaffold compiles the portable subset and defers platform/display/DirectDraw/input/registry/version islands. Needs logical sub-boundaries before it can be a clean portable foundation layer. |
| `Code/wwnet/wwnet.dsp` | `wwnet` | Builds in the non-Windows scaffold after socket/session source classification | First networking layer. Current scaffold compiles packet/stat/network-object helpers and defers live WinSock/session sources for a future platform networking boundary. |
| `Code/BinkMovie/BinkMovie.dsp` | `BinkMovie` | Builds in the non-Windows scaffold after media/render source classification | Small media/content seam. Current scaffold compiles subtitle data/parser sources and defers RAD/Bink playback plus renderer subtitle glue. |
| `Code/SControl/SControl.dsp` | `SControl` | Builds in the non-Windows scaffold with a narrow POSIX socket bridge | Small server-control/network seam. Current scaffold compiles the protocol and UDP transport while preserving the WinSock path under `_WIN32`. |
| `Code/WWAudio/WWAudio.dsp` | `WWAudio` | Opt-in scaffold; Miles/backend blocker documented | Audio subsystem seam. Current CMake classifies events, save/load, scene metadata, threading, utilities, and Miles backend sources. Not in the default green scaffold yet. |
| `Code/wwphys/wwphys.dsp` | `wwphys` | Opt-in scaffold; mixed runtime/renderer/platform blocker documented | Physics subsystem seam. Current CMake classifies path/visibility, runtime simulation, math/serialization, scene/render glue, and terrain/render sources. Not in the default green scaffold yet. |
| `Code/wwui/wwui.dsp` | `wwui` | Opt-in scaffold; mixed UI/input/platform/renderer blocker documented | UI/input subsystem seam. Current CMake classifies dialog/control, input/cursor, and IME islands. Not in the default green scaffold yet. |
| `Code/ww3d2/ww3d2.dsp` | `ww3d2` | Opt-in scaffold; mixed renderer/platform/asset-content blocker documented | Core renderer subsystem seam. Current CMake classifies asset/content, render runtime, presentation, texture/material, and DX8 backend islands. Not in the default green scaffold yet. |

## `wwlib` sub-bucket observations

`wwlib` is the most important finding from Batch 003. The original project name suggests a low-level utility library, but its source list is broad:

- foundation-like code: strings, containers, file classes, compression, hashes, INI, CRC, random, raw/text/buffer files
- platform-like code: registry, version checks, timers, threads, mutexes, Windows handle conventions
- diagnostics: mono console, stack/debug-adjacent support
- content/image helpers: TGA, PCX, palette, blitters
- graphics/platform-adjacent code: `ddraw.cpp`, `dsurface.*`, `xsurface.*`
- Windows integration: COM utilities, browser launching

The current scaffold intentionally keeps this as one `wwlib` target because original VC6 membership is the source of truth. The future architecture should split these responsibilities logically before moving files.

## Current non-Windows classification seams

### `wwnet`

Batch 005 adds `wwnet` as the next support dependency layer. The target is
temporarily grouped with the foundation scaffold for build-order purposes, but
architecturally it crosses into the future `platform` networking boundary.

The non-Windows scaffold keeps the original source inventory visible while
deferring live socket/session sources:

```text
BWBalance.cpp, connect.cpp, netutil.cpp, packetmgr.cpp, rhost.cpp, singlepl.cpp
```

`Code/wwnet/socket_compat.h` provides only narrow socket type names needed by
shared packet/object headers. It is not a WinSock implementation.

### `wwlib`

The first non-mechanical `wwlib` blocker was DirectDraw:

```text
Code/wwlib/dsurface.h -> <ddraw.h>
```

That was not solved with a random `ddraw.h` stub. Instead, Batch 004 keeps original `wwlib.dsp` source membership visible in CMake and removes a classified set of platform/display-adjacent sources from the non-Windows bootstrap build. The current deferred set is:

```text
blit.cpp, convert.cpp, cpudetect.cpp, ddraw.cpp, dsurface.cpp, ini.cpp,
jshell.cpp, keyboard.cpp, mixfile.cpp, mono.cpp, msgloop.cpp, mpu.cpp,
mutex.cpp, rawfile.cpp, rcfile.cpp, refcount.cpp, regexpr.cpp, registry.cpp,
strtok_r.cpp, srandom.cpp, verchk.cpp, widestring.cpp, win.cpp, xsurface.cpp
```

This makes `wwlib` compile-checkable as part of the Linux foundation scaffold while documenting that the original bucket needs future logical splits.

Batch 006 makes those logical splits explicit in CMake source variables instead of leaving them as one opaque list plus removals. Current islands are:

```text
WWLIB_FOUNDATION_SOURCES
WWLIB_PLATFORM_SOURCES
WWLIB_DISPLAY_SURFACE_SOURCES
WWLIB_INPUT_MESSAGE_SOURCES
WWLIB_REGISTRY_VERSION_SOURCES
WWLIB_ENCODING_STRING_SOURCES
WWLIB_CONTENT_ARCHIVE_SOURCES
WWLIB_WINDOWS_INTEGRATION_SOURCES
WWLIB_DIAGNOSTIC_CONSOLE_SOURCES
```

`wwnet` is likewise split into:

```text
WWNET_PACKET_OBJECT_SOURCES
WWNET_LIVE_SOCKET_SESSION_SOURCES
WWNET_BANDWIDTH_REMOTE_HOST_SOURCES
WWNET_SOCKET_TYPE_BRIDGE_SOURCES
```

### `BinkMovie`

Batch 007 adds `BinkMovie` as a small media/content seam. It is not treated as a fully portable video backend yet. The target keeps the original VC6 project name and splits the source inventory into:

```text
BINKMOVIE_SUBTITLE_DATA_SOURCES
BINKMOVIE_SUBTITLE_RENDER_SOURCES
BINKMOVIE_RAD_PLAYER_SOURCES
```

On non-Windows, the scaffold compiles the subtitle data/parser island and defers the renderer subtitle manager plus RAD/Bink playback implementation until the renderer/media boundaries are modeled.

### `SControl`

Batch 009 adds `SControl` as a small server-control/network seam. It is split into:

```text
SCONTROL_PROTOCOL_SOURCES
SCONTROL_SOCKET_TRANSPORT_SOURCES
```

Unlike `wwnet`, the small `SControl` UDP transport is compiled in the non-Windows scaffold using a narrow POSIX socket bridge in `servercontrolsocket.h`. This is not a general WinSock replacement; it only maps the concrete socket calls this target already uses and keeps the original WinSock include path under `_WIN32`.

### `WWAudio`

Batch 010 adds `WWAudio` behind the opt-in `RENEGADE_BUILD_AUDIO_SEAMS` switch. Its CMake source inventory is split into:

```text
WWAUDIO_EVENT_SOURCES
WWAUDIO_SAVELOAD_SOURCES
WWAUDIO_SCENE_METADATA_SOURCES
WWAUDIO_THREADING_SOURCES
WWAUDIO_UTILITY_SOURCES
WWAUDIO_MILES_BACKEND_SOURCES
```

The first raw build probe exposed many case-sensitive local include spellings. Those were fixed so the target now reaches the real blocker: proprietary Miles Sound System headers such as `mss.h` / `Mss.H`. The Miles/DirectSound backend is recorded as a future audio backend boundary; no fake Miles headers were added.

### `wwphys`

Batch 011 adds `wwphys` behind the opt-in `RENEGADE_BUILD_PHYS_SEAMS` switch. Its CMake source inventory is split into:

```text
WWPHYS_PATH_VIS_SOURCES
WWPHYS_RUNTIME_SIM_SOURCES
WWPHYS_MATH_SERIALIZATION_SOURCES
WWPHYS_SCENE_RENDER_GLUE_SOURCES
WWPHYS_TERRAIN_RENDER_SOURCES
```

The first raw probe exposed case-sensitive local include spellings, which were fixed so the target could reach a more meaningful blocker. The current documented seam is mixed runtime/renderer/platform leakage through shared headers, with the next meaningful non-Windows failure surfacing via `wwlib/win.h` (`HINSTANCE` / `HWND`) rather than a single isolated physics implementation file.

### `wwui`

Batch 012 adds `wwui` behind the opt-in `RENEGADE_BUILD_UI_SEAMS` switch. Its CMake source inventory is split into:

```text
WWUI_DIALOG_CONTROL_SOURCES
WWUI_INPUT_CURSOR_SOURCES
WWUI_IME_SOURCES
```

The first raw probe exposed local include-case mismatches, which were fixed so the target could reach more meaningful blockers. The current documented seam is mixed UI/input/platform/renderer leakage through dialog/control headers, IME integration, and ww3d2 presentation dependencies. The next meaningful blockers include `Render2D.h`, `<imm.h>`, and Win32 callback/handle expectations surfacing through `dialogbase.h`, `IMEManager.h`, and `wwlib/win.h`.

### `ww3d2`

Batch 013 adds `ww3d2` behind the opt-in `RENEGADE_BUILD_RENDERER_SEAMS` switch. Its CMake source inventory is split into:

```text
WW3D2_ASSET_CONTENT_SOURCES
WW3D2_RENDER_RUNTIME_SOURCES
WW3D2_PRESENTATION_SOURCES
WW3D2_TEXTURE_MATERIAL_SOURCES
WW3D2_DX8_BACKEND_SOURCES
```

The explicit Direct3D 8 backend island is deferred on non-Windows seam probes, but the next meaningful blockers still come from shared Win32-heavy and DX8-heavy interfaces leaking through higher-level renderer code. The probe stops at `wwlib/win.h` (`HINSTANCE`, `HWND`) and `dx8wrapper.h` (`d3d8.h`), which is useful evidence that `ww3d2` is not yet separable into a portable renderer core without further boundary work.

## Near-term recommendation

Before jumping to `Commando`, continue using this target map to classify original projects:

1. refine the deferred `wwlib` islands one area at a time
2. decide whether later scaffolding should model temporary subtargets for platform, file/archive, display/surface, and string-conversion islands
3. refine the `wwnet` live socket/session seam before treating networking as portable
