# Command & Conquer Renegade — EA Baseline Modernization

This is the **EA baseline modernization branch** of the original Electronic Arts C&C Renegade source. The goal is to preserve the original source layout, document its architecture, and build a reproducible modern scaffold before replaying OpenW3D fork changes.

## What is this?

This repo starts from the [original EA Renegade source](https://github.com/electronicarts/CnC_Renegade) (`ea/main @ 3e00c3a`). It grows a CMake scaffold incrementally from the original VC6 project metadata. The scaffold is organized around **9 architectural buckets**. No fake stubs. No behavior changes. Platform-specific code is classified and deferred.

> **Status (Batch 022):** 11 of 61 original VC6 projects are compiled in the default scaffold. 6 more are blocked by resolvable issues (missing SDKs, header tangle). The rest are entirely Win32/MFC-coupled.

## Quick build

```bash
# Default scaffold — foundation + engine seams (11 targets)
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON -DRENEGADE_BUILD_ENGINE_SEAMS=ON
cmake --build build/cmake-scaffold -j4
```

## Status matrix

| Category | Count | Details |
|----------|-------|---------|
| ✅ Built | 11 | `wwdebug`, `WWMath`, `wwbitpack`, `wwsaveload`, `wwtranslatedb`, `wwlib`, `wwutil`, `wwnet`, `BinkMovie`, `SControl`, `Scripts` |
| ❌ Blocked (SDK) | 2 | `ww3d2` (d3d8.h missing), `WWAudio` (Mss.h missing) |
| ❌ Blocked (header tangle) | 2 | `Combat` (needs ww3d2 math types), `wwphys` (same) |
| ❌ Blocked (Win32) | 1 | `wwui` (158 WCHAR hits, Win32 msg loop) |
| ❌ Blocked (all seams) | 1 | `Commando` (requires all above) |
| ❌ Deferred | 44 | tools, tests, installers — entirely Win32/MFC/assembly |

See: [`docs/architecture/project-scaffold-status.md`](docs/architecture/project-scaffold-status.md)

## Architecture

The codebase maps to 9 long-term buckets:

```
foundation       → wwdebug, WWMath, wwbitpack, wwsaveload, wwtranslatedb
platform         → wwlib (filtered), wwnet (filtered)
engine_runtime   → Scripts (filtered), Combat (blocked)
engine_asset_content → BinkMovie (filtered)
audio            → WWAudio (blocked — Miles SDK)
renderer         → ww3d2 (blocked — DX8 SDK)
physics          → wwphys (blocked — header tangle)
input_and_control → SControl (built), wwui (blocked)
products         → Commando (blocked), tools (deferred)
```

The **central architectural problem** is that `ww3d2/` contains both portable game-logic math types (Vector3, Sphere, OBBox) and DirectX 8 renderer code in the same physical directory. Until these are separated, any target needing math types (Combat, wwphys) cannot build on non-Windows.

See: [`docs/architecture/9-bucket-architecture-map.md`](docs/architecture/9-bucket-architecture-map.md)

## Key docs

| Doc | What it is |
|-----|-----------|
| [`docs/architecture/9-bucket-architecture-map.md`](docs/architecture/9-bucket-architecture-map.md) | Full architecture map with dependency graph and open questions |
| [`docs/architecture/project-scaffold-status.md`](docs/architecture/project-scaffold-status.md) | All 61 VC6 projects with status, bucket, and blocker notes |
| [`docs/architecture/deferred-source-ledger.md`](docs/architecture/deferred-source-ledger.md) | Every intentionally deferred source file and why |
| [`docs/architecture/modernization-progress-roadmap.md`](docs/architecture/modernization-progress-roadmap.md) | Batch-by-batch progress history |
| [`docs/build/`](docs/build/) | Per-batch portability and scaffold documentation |

## Build options

```bash
# Default: foundation + engine seams (what builds cleanly)
-DRENEGADE_BUILD_FOUNDATION_LIBS=ON -DRENEGADE_BUILD_ENGINE_SEAMS=ON

# Probe blocked targets (requires SDK or header cleanup first)
-DRENEGADE_BUILD_RENDERER_SEAMS=ON    # ww3d2 — needs DirectX 8 SDK
-DRENEGADE_BUILD_AUDIO_SEAMS=ON       # WWAudio — needs Miles Sound System SDK
-DRENEGADE_BUILD_PHYS_SEAMS=ON        # wwphys — blocked by header tangle
-DRENEGADE_BUILD_UI_SEAMS=ON          # wwui — needs Win32/text boundary
-DRENEGADE_BUILD_COMBAT_SEAMS=ON      # Combat — blocked by header tangle

# Products (all seams required)
-DRENEGADE_BUILD_COMMANDO_SEAMS=ON    # Commando — everything
-DRENEGADE_BUILD_TOOLS_SEAMS=ON       # Tools — mostly Win32/MFC

# Strict: fail if a manifest-listed source is absent
-DRENEGADE_STRICT_SOURCE_MANIFEST=ON
```

## EA baseline dependencies (original)

The original EA source expects these SDKs. They are absent from this repo:

```
DirectX SDK 8.0+       \Code\DirectX\
RAD Bink SDK            \Code\BinkMovie\
RAD Miles Sound System  \Code\Miles6\
NvDXTLib SDK            \Code\NvDXTLib\
Lightscape SDK          \Code\Lightscape\
Umbra SDK               \Code\Umbra\
GameSpy SDK             \Code\GameSpy\
GNU Regex               \Code\WWLib\
SafeDisk API            \Code\Launcher\SafeDisk\
Microsoft Cab Library   \Code\Installer\Cab\
RTPatch Library         \Code\Installer\
Java Runtime Headers    \Code\Tools\RenegadeGR\
```

## FDS

The dedicated server is **not** a standalone project. It is an alternate `Commando` build with `FREEDEDICATEDSERVER` defined in `Code/Combat/specialbuilds.h`.

---

## Contributing

This repo will not accept direct contributions. Create a fork if you want to collaborate on changes.
