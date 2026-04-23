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

## Near-term recommendation

Before jumping to `Commando`, continue using this target map to classify original projects:

1. refine the deferred `wwlib` islands one area at a time
2. decide whether later scaffolding should model temporary subtargets for platform, file/archive, display/surface, and string-conversion islands
3. refine the `wwnet` live socket/session seam before treating networking as portable
