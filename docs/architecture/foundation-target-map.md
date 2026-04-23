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
| `Code/wwutil/wwutil.dsp` | `wwutil` | Builds as a targeted scaffold target | Small support utilities. Windows stack walking is no-op on non-Windows scaffold builds. |
| `Code/wwlib/wwlib.dsp` | `wwlib` | Configures and partially compiles; stops at DirectDraw surface dependency | Mixed historical bucket. Contains true foundation code plus platform, COM, mono-console, image/surface, registry, version, and display-adjacent code. Needs logical sub-boundaries before it can be a clean portable foundation layer. |

## `wwlib` sub-bucket observations

`wwlib` is the most important finding from Batch 003. The original project name suggests a low-level utility library, but its source list is broad:

- foundation-like code: strings, containers, file classes, compression, hashes, INI, CRC, random, raw/text/buffer files
- platform-like code: registry, version checks, timers, threads, mutexes, Windows handle conventions
- diagnostics: mono console, stack/debug-adjacent support
- content/image helpers: TGA, PCX, palette, blitters
- graphics/platform-adjacent code: `ddraw.cpp`, `dsurface.*`, `xsurface.*`
- Windows integration: COM utilities, browser launching

The current scaffold intentionally keeps this as one `wwlib` target because original VC6 membership is the source of truth. The future architecture should split these responsibilities logically before moving files.

## Current stop seam

The first non-mechanical `wwlib` blocker is DirectDraw:

```text
Code/wwlib/dsurface.h -> <ddraw.h>
```

This should not be solved with a random `ddraw.h` stub. It should be handled when the modernization introduces clear `platform` and `renderer` seams.

## Near-term recommendation

Before jumping to `Commando`, continue using this target map to classify original projects:

1. finish documenting `wwlib` internal islands
2. decide whether to model a temporary Windows-only surface/display subtarget or exclude those sources from non-Windows foundation bootstrap builds
3. then move outward to `wwnet` or shared runtime targets once foundation/platform seams are explicit
