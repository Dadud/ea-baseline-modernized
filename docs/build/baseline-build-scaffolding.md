# Baseline Build Scaffolding Approach

## Purpose

This document defines the first build-modernization step after the original EA project inventory.

The key decision: **build scaffolding should be generated from the original Visual Studio 6 project inventory, not recreated manually from memory.**

## Why not hand-write CMake first?

The EA baseline contains:
- 14 `.dsw` workspaces
- 61 `.dsp` projects
- hundreds of project-specific source lists
- configuration-specific include paths, defines, libraries, and output names
- legacy dependency assumptions

Hand-writing CMake immediately would risk accidentally changing the original product model before parity checks exist.

## Recommended first scaffolding milestone

Create project inventory tooling:

```text
scripts/projects/parse_vc6_workspace.py
scripts/projects/write_project_inventory.py
```

These scripts should parse:
- `.dsw` project membership
- workspace dependency edges
- `.dsp` project type
- configurations
- source files
- include paths
- defines
- link libraries
- output paths

Then they should emit:

```text
build-manifests/vc6-projects.json
build-manifests/vc6-projects.md
```

## Why this helps

The manifest becomes the source of truth for:
- CMake generation
- parity checks
- dependency documentation
- build order decisions
- identifying missing SDKs

## First modern CMake milestone

After inventory scripts exist, create the first conservative CMake scaffold with target names that mirror original projects.

Initial logical target groups:

### Foundation/static libraries first
- `wwmath`
- `wwdebug`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`
- `wwutil`
- `wwlib`

### Runtime/media libraries next
- `wwnet`
- `WWAudio`
- `BinkMovie`
- `ww3d2`
- `wwphys`
- `wwui`
- `Combat`
- `Scripts`
- `SControl`
- `BandTest`

### Products after libraries
- `commando_client`
- `commando_fds`

### Tools after client/FDS shape is understood
Small tools first:
- `bin2cpp`
- `VerStamp`
- `MakeMix`
- `RenRem`
- `wdump`

Then larger GUI/viewer/editor tools.

## FDS modeling

Original FDS is not a standalone `.dsp`. It is a `commando` Release build with:

```cpp
FREEDEDICATEDSERVER
```

defined via `Code/Combat/specialbuilds.h`.

Modern CMake should model it as a separate target:

```text
commando_fds
```

But it should initially share the original `commando` source list as much as possible to preserve behavior.

Long-term, the source list should be split into true client/server/product buckets. That comes after parity tracking.

## Immediate next task

Implement the parser/inventory scripts and generate:

```text
build-manifests/vc6-projects.json
build-manifests/vc6-projects.md
```

Then commit them as a documentation/tooling milestone before writing CMake.
