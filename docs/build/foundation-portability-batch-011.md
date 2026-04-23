# Foundation Portability Batch 011: wwphys Boundary Mapping

## Purpose

Batch 011 adds a conservative scaffold for the original EA VC6 `wwphys` static-library project, but keeps it behind an opt-in CMake switch because it quickly exposes a deep runtime/renderer/platform seam rather than a small low-risk compile target.

This batch is a mapping and classification batch, not a claim that the physics subsystem is portable.

## Added target scaffold

Original VC6 project:

```text
Code/wwphys/wwphys.dsp
```

CMake target name:

```text
wwphys
```

The target is opt-in:

```bash
cmake -S . -B build/cmake-phys-probe \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_PHYS_SEAMS=ON
```

It is not part of the default scaffold yet, so the existing full scaffold remains green.

## Source islands

`Code/wwphys/CMakeLists.txt` now records these logical source islands:

```text
WWPHYS_PATH_VIS_SOURCES
WWPHYS_RUNTIME_SIM_SOURCES
WWPHYS_MATH_SERIALIZATION_SOURCES
WWPHYS_SCENE_RENDER_GLUE_SOURCES
WWPHYS_TERRAIN_RENDER_SOURCES
```

Interpretation:

- `WWPHYS_PATH_VIS_SOURCES`: pathfinding, visibility, waypoint, and path solve data structures.
- `WWPHYS_RUNTIME_SIM_SOURCES`: core runtime simulation, vehicles, rigid bodies, and object physics.
- `WWPHYS_MATH_SERIALIZATION_SOURCES`: binary math/geometry helpers and supporting grid/floodfill structures.
- `WWPHYS_SCENE_RENDER_GLUE_SOURCES`: scene/render coupling, projectors, decals, save systems, and Umbra-adjacent integration.
- `WWPHYS_TERRAIN_RENDER_SOURCES`: terrain material/patch rendering and vehicle dazzle/render-adjacent effects.

## Non-Windows bootstrap rule

On non-Windows seam probes, the scaffold keeps the full source inventory visible but defers:

```text
WWPHYS_SCENE_RENDER_GLUE_SOURCES
WWPHYS_TERRAIN_RENDER_SOURCES
```

This avoids pretending renderer/terrain integration is already below a portable physics boundary.

## Probe findings

The first raw build probe stopped on many case-sensitive local include spellings in `Code/wwphys/*`. Batch 011 fixes those local include spellings so the target reaches the real architectural seam instead of failing on Linux filesystem differences.

The next meaningful blocker is not DirectX directly; it is mixed Win32 and renderer leakage through shared headers reachable from apparently non-render runtime/path files:

```text
Code/wwlib/win.h:83:8: error: ‘HINSTANCE’ does not name a type
Code/wwlib/win.h:84:8: error: ‘HWND’ does not name a type
```

That shows `wwphys` is not just a simulation bucket. Even the path/runtime side still pulls through renderer and Win32-heavy headers such as:

- `rendobj.h`
- `widgetuser.h`
- `wwstring.h`
- `win.h`

## Interpretation

`wwphys` should currently be treated as a mixed seam between:

- engine runtime physics/simulation
- renderer scene/render-object integration
- platform/window/Win32-heavy support surfaces
- terrain/render implementation details

This batch does **not** add fake Win32 handle types globally just to force `wwphys` farther. The correct next step later is to decide whether to:

- further narrow the `wwphys` compile subset, or
- design cleaner renderer/platform-facing boundaries before pushing compile depth.

## Verification

Default scaffold verification remains:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

`wwphys` probing is intentionally opt-in and currently documents the mixed runtime/renderer/platform blocker rather than hiding it.
