# Original EA Project Inventory

## Purpose

This document inventories the original Visual Studio 6 workspace/project structure from the EA source baseline. It is the starting point for modern build scaffolding.

Baseline:

```text
ea/main @ 3e00c3a1b97381bb28be89a35b856375e0629a08
branch: ea-baseline-modernized
worktree: /tmp/openw3d-ea-baseline
```

## Workspace files discovered

The EA baseline contains 14 `.dsw` workspace files and 61 `.dsp` project files.

Primary workspaces:

| Workspace | Role |
| --- | --- |
| `Code/commando.dsw` | Main game/client workspace and shared engine libraries |
| `Code/tools.dsw` | Tool/editor/archive/plugin workspace |

Additional standalone/test workspaces:

| Workspace | Role |
| --- | --- |
| `Code/BandTest/BandTest.dsw` | Bandwidth test DLL workspace |
| `Code/Tests/Bandy/bandy.dsw` | Test workspace |
| `Code/Tests/LocalHost/LocalHost.dsw` | Test workspace |
| `Code/Tools/MaxFly/MaxFly.dsw` | Tool workspace |
| `Code/Tools/RenegadeGR/RenegadeGR.dsw` | Tool/plugin workspace |
| `Code/Tools/RenRem/RenRem.dsw` | Remote console tool workspace |
| `Code/Tools/VerStamp/VerStamp.dsw` | Version stamp utility workspace |
| `Code/Tools/W3DView/W3DView.dsw` | Model viewer workspace |
| `Code/Tools/WWConfig/WWConfig.dsw` | Config tool workspace |
| `Code/ww3d2/hueshift/hueshift.dsw` | Renderer sample/test workspace |
| `Code/ww3d2/skeleton/sample.dsw` | Renderer sample/test workspace |
| `Code/ww3d2/sorttest/sorttest.dsw` | Renderer sample/test workspace |

There is also one modern solution from a later/auxiliary tool area:

| Solution | Role |
| --- | --- |
| `Code/Tools/RenegadeSim/RenegadeSim.sln` | RenegadeSim tool/simulation solution |

## `Code/commando.dsw` projects

This is the most important workspace for the baseline program.

Projects listed by the workspace:

| Project | Path | Role |
| --- | --- | --- |
| `commando` | `Code/Commando/commando.dsp` | Main game client executable project |
| `Combat` | `Code/Combat/Combat.dsp` | Main gameplay/runtime static library |
| `ww3d2` | `Code/ww3d2/ww3d2.dsp` | Renderer/static visual engine library |
| `wwphys` | `Code/wwphys/wwphys.dsp` | Physics/scene static library |
| `wwlib` | `Code/wwlib/wwlib.dsp` | Foundation/platform static library |
| `wwmath` | `Code/WWMath/wwmath.dsp` | Math static library |
| `wwnet` | `Code/wwnet/wwnet.dsp` | Networking static library |
| `WWAudio` | `Code/WWAudio/WWAudio.dsp` | Audio static library |
| `wwui` | `Code/wwui/wwui.dsp` | UI static library |
| `wwdebug` | `Code/wwdebug/wwdebug.dsp` | Debug support static library |
| `wwutil` | `Code/wwutil/wwutil.dsp` | Utility static library |
| `wwbitpack` | `Code/wwbitpack/wwbitpack.dsp` | Bit packing static library |
| `wwsaveload` | `Code/wwsaveload/wwsaveload.dsp` | Save/load static library |
| `wwtranslatedb` | `Code/wwtranslatedb/wwtranslatedb.dsp` | Translation DB static library |
| `BinkMovie` | `Code/BinkMovie/BinkMovie.dsp` | Bink movie static library |
| `BandTest` | `Code/BandTest/BandTest.dsp` | Bandwidth test DLL |
| `SControl` | `Code/SControl/SControl.dsp` | Server control static library |
| `Scripts` | `Code/Scripts/Scripts.dsp` | Game scripts DLL |
| `GameSpy` | `Code/GameSpy/GameSpy.dsp` | GameSpy SDK/project reference; dependency source is absent from baseline tree |
| `LevelEdit` | `Code/Tools/LevelEdit/LevelEdit.dsp` | Public editor/tool project |
| `WWConfig` | `Code/Tools/WWConfig/WWConfig.dsp` | Config utility |
| `WWCtrl` | `Code/Tools/WWCtrl/WWCtrl.dsp` | Tool/control DLL |
| `max2w3d` | `Code/Tools/max2w3d/max2w3d.dsp` | 3ds Max export plugin |

### Main `commando` project dependencies

`commando.dsw` declares the following dependencies for the `commando` project:

```text
Scripts
wwdebug
wwlib
wwmath
wwnet
wwphys
wwutil
WWAudio
wwsaveload
Combat
wwtranslatedb
wwbitpack
ww3d2
wwui
BinkMovie
BandTest
SControl
GameSpy
```

This dependency list is important because it defines the original executable assembly shape.

## `commando.dsp` product configurations

`Code/Commando/commando.dsp` is a Win32 application project with three listed configurations:

| Configuration | Output |
| --- | --- |
| `commando - Win32 Debug` | `Run/RenegadeD.exe`, debug PDB/map |
| `commando - Win32 Release` | `Run/Renegade.exe`, release PDB/map |
| `commando - Win32 Profile` | `Run/RenegadeP.exe`, profile PDB/map |

The project links against many original engine libraries plus legacy SDK dependencies, including DirectX, Miles, Bink, GameSpy, Umbra, and other Windows libraries.

## Free Dedicated Server (FDS)

The original README states that the FDS is produced by uncommenting:

```cpp
#define FREEDEDICATEDSERVER
```

in:

```text
Code/Combat/specialbuilds.h
```

Then rebuilding the Release configuration.

The source tree contains extensive `FREEDEDICATEDSERVER` conditionals, especially in:
- `Code/Combat/*`
- `Code/Commando/*`
- `Code/BandTest/*`
- `Code/Launcher/*`

Important examples:
- `Code/Combat/specialbuilds.h` documents the define and mutual exclusion rules.
- `Code/Commando/ConsoleMode.cpp` prints dedicated-server branding under `FREEDEDICATEDSERVER`.
- `Code/Commando/AutoStart.cpp` and `Code/Commando/slavemaster.cpp` switch executable names between `renegade.exe` and `renegadeserver.exe`.
- `Code/BandTest/BandTest.cpp` switches registry paths to `RenegadeFDS`.

### Baseline FDS implication

The EA baseline does **not** have a separate `renegadeserver.dsp` in the main workspace. FDS is an alternate build of the `commando` project via the `FREEDEDICATEDSERVER` define.

A modern build should eventually model FDS as its own explicit product target, but the first baseline pass should document that it originates from the same source set and behavior contract as `commando - Win32 Release` plus `FREEDEDICATEDSERVER`.

## `Code/tools.dsw` projects

The tools workspace contains 27 projects:

| Project | Path | Type / role |
| --- | --- | --- |
| `LevelEdit` | `Code/Tools/LevelEdit/LevelEdit.dsp` | Main public editor application |
| `W3DView` | `Code/Tools/W3DView/W3DView.dsp` | W3D model/asset viewer |
| `WWConfig` | `Code/Tools/WWConfig/WWConfig.dsp` | Configuration utility |
| `MakeMix` | `Code/Tools/MakeMix/MakeMix.dsp` | MIX archive console utility |
| `MixViewer` | `Code/Tools/MixViewer/MixViewer.dsp` | MIX archive viewer |
| `RenRem` | `Code/Tools/RenRem/RenRem.dsp` | Remote admin console utility |
| `wdump` | `Code/Tools/wdump/wdump.dsp` | W3D dump/inspection utility |
| `W3DShellExt` | `Code/Tools/W3DShellExt/W3DShellExt.dsp` | Windows shell extension |
| `max2w3d` | `Code/Tools/max2w3d/max2w3d.dsp` | 3ds Max W3D exporter plugin |
| `pluglib` | `Code/Tools/pluglib/pluglib.dsp` | Tool/plugin support library |
| `WWCtrl` | `Code/Tools/WWCtrl/WWCtrl.dsp` | Tool/control DLL |
| `AMC_imp` | `Code/Tools/AMC_IMP/AMC_IMP.dsp` | Import plugin DLL |
| `asf_imp` | `Code/Tools/ASF_IMP/ASF_IMP.dsp` | Import plugin DLL |
| `Blender2` | `Code/Tools/Blender2/Blender2.dsp` | Plugin DLL |
| `Clipbord` | `Code/Tools/Clipbord/Clipbord.dsp` | Plugin DLL |
| `ViewTrans` | `Code/Tools/ViewTrans/ViewTrans.dsp` | Plugin DLL |
| `ChunkView` | `Code/Tools/ChunkView/ChunkView.dsp` | Chunk viewer app |
| `CommandoUpdate` | `Code/Tools/CommandoUpdate/CommandoUpdate.dsp` | Update utility |
| `CopyLocked` | `Code/Tools/CopyLocked/CopyLocked.dsp` | Utility app |
| `LightMap` | `Code/Tools/LightMap/LightMap.dsp` | Lightmap tool |
| `MaxFly` | `Code/Tools/MaxFly/MaxFly.dsp` | Tool app |
| `SimpleGraph` | `Code/Tools/SimpleGraph/SimpleGraph.dsp` | Utility app |
| `SkeletonHack` | `Code/Tools/SkeletonHack/SkeletonHack.dsp` | Console utility |
| `VerStamp` | `Code/Tools/VerStamp/VerStamp.dsp` | Version stamp console utility |
| `VidInit` | `Code/Tools/vidinit/VidInit.dsp` | Video init utility |
| `W3DUpdate` | `Code/Tools/W3DUpdate/W3DUpdate.dsp` | W3D update utility |
| `bin2cpp` | `Code/Tools/bin2cpp/bin2cpp.dsp` | Binary-to-C++ converter utility |

## Project type summary

From all 61 `.dsp` files discovered:

- main application projects
- console utilities
- static libraries
- DLL/plugin projects
- test applications
- tool applications
- import/export plugins

Representative source counts:

| Project | Sources listed |
| --- | ---: |
| `Code/Commando/commando.dsp` | 492 |
| `Code/Tools/LevelEdit/LevelEdit.dsp` | 543 |
| `Code/Combat/Combat.dsp` | 265 |
| `Code/ww3d2/ww3d2.dsp` | 233 |
| `Code/wwlib/wwlib.dsp` | 206 |
| `Code/wwphys/wwphys.dsp` | 193 |
| `Code/Tools/W3DView/W3DView.dsp` | 227 |
| `Code/Tools/max2w3d/max2w3d.dsp` | 159 |

This confirms the early build-modernization priority should be:

1. source/project inventory tooling
2. common static libraries
3. `commando` client target
4. explicit FDS variant target
5. core tools subset
6. larger editor/plugin tool set

## Missing or external SDK dependencies

The original README and project files reference dependencies that are not expected to build out-of-the-box on a clean modern machine:

- DirectX SDK
- RAD Bink SDK
- RAD Miles Sound System SDK
- NvDXTLib SDK
- Lightscape SDK
- Umbra SDK
- GameSpy SDK
- GNU Regex
- SafeDisk API
- Microsoft Cab Archive Library
- RTPatch Library
- Java Runtime Headers
- 3ds Max SDK/plugin dependencies

These must be captured in the parity/build plan as one of:
- required legacy dependency
- optional unavailable dependency
- modern replacement candidate
- stub/mock candidate for structural builds only

## Modern build scaffolding implications

The first modern build scaffolding should not start with a giant hand-written CMake port.

Recommended sequence:

1. Parse `.dsw`/`.dsp` inventories into machine-readable manifests.
2. Generate documentation tables from those manifests.
3. Generate initial CMake target skeletons preserving original product/project boundaries.
4. Build common static libraries first.
5. Add `commando_client` target matching original `commando - Win32 Release`.
6. Add `commando_fds` target as a variant of the same source list with `FREEDEDICATEDSERVER` defined.
7. Add tools in priority order:
   - small console utilities first (`bin2cpp`, `VerStamp`, `MakeMix`, `RenRem`, `wdump`)
   - viewer/config tools next (`W3DView`, `WWConfig`, `MixViewer`)
   - LevelEdit and plugins later because they have deeper dependencies.

## First scaffolding target recommendation

Create inventory tooling before build files:

```text
scripts/projects/parse_vc6_workspace.py
scripts/projects/write_project_inventory.py
```

The generated manifest should include:
- project name
- `.dsp` path
- project type
- configurations
- source file list
- include paths
- preprocessor defines
- linked libraries
- output path
- declared workspace dependencies

This gives us a reliable source of truth for preserving the original project model while moving toward CMake.
