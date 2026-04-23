# Foundation Portability Batch 012: wwui UI/Input Seam Mapping

## Purpose

Batch 012 adds a conservative scaffold for the original EA VC6 `wwui` static-library project, but keeps it behind an opt-in CMake switch because it quickly exposes a deep UI/input/platform/renderer seam rather than a small low-risk compile target.

This batch is a mapping and classification batch, not a claim that the UI subsystem is portable.

## Added target scaffold

Original VC6 project:

```text
Code/wwui/wwui.dsp
```

CMake target name:

```text
wwui
```

The target is opt-in:

```bash
cmake -S . -B build/cmake-ui-probe \
  -DRENEGADE_BUILD_FOUNDATION_LIBS=ON \
  -DRENEGADE_BUILD_UI_SEAMS=ON
```

It is not part of the default scaffold yet, so the existing full scaffold remains green.

## Source islands

`Code/wwui/CMakeLists.txt` now records these logical source islands:

```text
WWUI_DIALOG_CONTROL_SOURCES
WWUI_INPUT_CURSOR_SOURCES
WWUI_IME_SOURCES
```

Interpretation:

- `WWUI_DIALOG_CONTROL_SOURCES`: core dialog/control, widget, text, list, menu, and style logic.
- `WWUI_INPUT_CURSOR_SOURCES`: mouse manager, screen cursor, and WWUI input bridge.
- `WWUI_IME_SOURCES`: IME manager, IME candidate handling, and IME candidate control UI.

## Non-Windows bootstrap rule

On non-Windows seam probes, the scaffold keeps the full source inventory visible but defers:

```text
WWUI_INPUT_CURSOR_SOURCES
WWUI_IME_SOURCES
```

This avoids pretending Win32 input/message-loop and IME integration are already below a portable UI boundary.

## Probe findings

The first raw build probe stopped on local include-case issues. Batch 012 fixed a small number of local `wwui` include spellings so the target could move past Linux filesystem assumptions and expose more meaningful blockers.

The next meaningful blockers show that even the dialog/control side is not independent from Win32 and renderer/input integration:

```text
ProgressCtrl.h -> Render2D.h
IMECandidate.h -> <imm.h>
dialogbase.h / wwlib/win.h -> HINSTANCE / HWND / CALLBACK
```

This means `wwui` currently mixes at least four concerns:

- dialog/control/widget logic
- Win32 input/message-loop glue
- IME/Unicode/Windows text input integration
- renderer-side text/control presentation via ww3d2 render types

## Interpretation

`wwui` should currently be treated as a mixed seam between:

- input_and_control
- products/client UI shell
- renderer presentation
- platform window/input/IME services

This batch does **not** add fake IME headers, broad Win32 callback macros, or renderer stubs just to force `wwui` farther. The correct next step later is to decide whether to:

- further narrow the compile subset, or
- design clearer UI/input/platform/presentation boundaries before pushing compile depth.

## Verification

Default scaffold verification remains:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

`wwui` probing is intentionally opt-in and currently documents the mixed UI/input/platform/renderer blocker rather than hiding it.
