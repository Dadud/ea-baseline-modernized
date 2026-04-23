# Command & Conquer Renegade — EA Baseline Modernization

This is the **EA baseline modernization branch** of the original Electronic Arts C&C Renegade source. The goal is to preserve the original source layout, document its architecture, and build a reproducible modern scaffold before replaying OpenW3D fork changes.

## What is this?

This repo starts from the [original EA Renegade source](https://github.com/electronicarts/CnC_Renegade) (`ea/main @ 3e00c3a`). It grows a CMake scaffold incrementally from the original VC6 project metadata. The scaffold is organized around **9 architectural buckets**. Platform-specific sources are classified and deferred — not faked.

## Platform behavior

**On Windows:** the full original source for each target compiles natively. Win32 APIs, DirectX, Miles Sound System, and MFC are all native to the platform.

**On Linux/macOS (UNIX):** platform-specific sources are filtered so the scaffold can build with GCC/Clang without fake stubs. The defaults are conservative.

This means the same CMake files work on both platforms — the filtering is automatic.

## Quick build

### Linux / macOS

```bash
# Default scaffold — foundation + engine seams
cmake -S . -B build/cmake-scaffold
cmake --build build/cmake-scaffold -j4

# Full scaffold including Scripts runtime core
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_SCRIPT_SEAMS=ON
cmake --build build/cmake-scaffold -j4
```

### Windows (Visual Studio)

```bat
# Configure with Visual Studio generator
cmake -S . -B build\cmake-scaffold -G "Visual Studio 17 2022"
cmake --build build\cmake-scaffold --config Release

# Or from Visual Studio, open build\cmake-scaffold\CnC_Renegade_EA_Baseline.sln
```

On Windows, the default enables foundation, platform, and engine seams. Enable additional buckets explicitly:

```bat
cmake -S . -B build\cmake-scaffold -DRENEGADE_BUILD_RENDERER_SEAMS=ON -DRENEGADE_DX8_INCLUDE_DIR="C:\path\to\dx8-sdk\include"
```

## SDK setup (Windows)

Some targets need third-party SDK headers. On Windows these are optional — targets that can't find their SDK will defer gracefully:

```bat
# DirectX 8 SDK — needed for ww3d2 (renderer)
cmake ... -DRENEGADE_BUILD_RENDERER_SEAMS=ON -DRENEGADE_USE_DX8_SDK=ON -DRENEGADE_DX8_INCLUDE_DIR="C:\DX8SDK\Include"

# Miles Sound System — needed for WWAudio
cmake ... -DRENEGADE_BUILD_AUDIO_SEAMS=ON -DRENEGADE_USE_MILES_SDK=ON -DRENEGADE_MILES_INCLUDE_DIR="C:\Miles\Include"

# RAD Bink — needed for full BinkMovie
cmake ... -DRENEGADE_USE_BINK_SDK=ON -DRENEGADE_BINK_INCLUDE_DIR="C:\Bink\Include"
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

**On Windows with SDKs:** most blocked targets become buildable. The remaining blockers are architectural seams (math-type extraction, Win32 text boundary) that need deliberate design.

See: [`docs/architecture/project-scaffold-status.md`](docs/architecture/project-scaffold-status.md)

## Architecture

The codebase maps to 9 long-term buckets:

```
foundation       → wwdebug, WWMath, wwbitpack, wwsaveload, wwtranslatedb
platform         → wwlib (filtered on UNIX), wwnet (filtered on UNIX)
engine_runtime   → Scripts (filtered on UNIX), Combat (needs ww3d2)
engine_asset_content → BinkMovie (filtered on UNIX)
audio            → WWAudio (needs Miles SDK)
renderer         → ww3d2 (needs DirectX 8 SDK)
physics          → wwphys (needs ww3d2 math types)
input_and_control → SControl, wwui (needs Win32 text boundary)
products         → Commando (needs all above), tools (deferred)
```

**The central architectural problem:** `ww3d2/` contains both portable game-logic math types (Vector3, Sphere, OBBox) and DirectX 8 renderer code in the same physical directory. Until these are separated, any target needing math types (Combat, wwphys) cannot build without either the DX8 SDK or the math types being extracted.

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
# Defaults change based on platform (WIN32 vs UNIX).
# All options can be explicitly set to ON or OFF regardless of platform.

# Foundation (default: ON everywhere)
-DRENEGADE_BUILD_FOUNDATION_LIBS=ON

# Platform layer (default: ON on Windows, ON on Linux)
-DRENEGADE_BUILD_PLATFORM_LIBS=ON

# Engine seams — BinkMovie subtitle data + SControl (default: ON everywhere)
-DRENEGADE_BUILD_ENGINE_SEAMS=ON

# Scripts DLL (default: ON on Windows, OFF on Linux)
-DRENEGADE_BUILD_SCRIPT_SEAMS=ON

# Renderer, physics, audio, UI, Combat, Commando
# Default OFF everywhere — enable explicitly on Windows with SDKs
-DRENEGADE_BUILD_RENDERER_SEAMS=ON    # needs DirectX 8 SDK
-DRENEGADE_BUILD_PHYS_SEAMS=ON        # needs ww3d2 math extraction
-DRENEGADE_BUILD_AUDIO_SEAMS=ON       # needs Miles Sound System SDK
-DRENEGADE_BUILD_UI_SEAMS=ON          # needs Win32 text boundary work
-DRENEGADE_BUILD_COMBAT_SEAMS=ON      # needs ww3d2 math extraction
-DRENEGADE_BUILD_COMMANDO_SEAMS=ON    # everything

# Strict source manifest (default: OFF)
-DRENEGADE_STRICT_SOURCE_MANIFEST=ON   # fail if a manifest source is absent
```

## EA baseline dependencies (original)

The original EA source expects these SDKs. They are absent from this repo:

```
DirectX SDK 8.0+       \Code\DirectX\
RAD Bink SDK            \Code\BinkMovie\
RAD Miles Sound System  \Code\Miles\
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

On Windows: place SDK headers in the expected paths or use `-DRENEGADE_*_INCLUDE_DIR` to point CMake at them.

On Linux/macOS: these SDKs are not available — those targets are deferred.

## FDS

The dedicated server is **not** a standalone project. It is an alternate `Commando` build with `FREEDEDICATEDSERVER` defined in `Code/Combat/specialbuilds.h`.

---

## Contributing

This repo will not accept direct contributions. Create a fork if you want to collaborate on changes.
