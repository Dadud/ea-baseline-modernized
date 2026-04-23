# Target Probe Report

Generated from `build-manifests/vc6-projects.json` and source-text signal scanning.

| Target | Type | Sources | Missing | Top blocker categories | Build status |
|---|---|---:|---:|---|---|
| `WWAudio` | Win32 (x86) Static Library | 43 | 0 | threading/synchronization (127), DirectSound/Miles/audio backend (51), Win32 window/message loop (2), old MSVC/C runtime spelling (2) | not built |
| `SControl` | Win32 (x86) Static Library | 4 | 0 | WinSock/network transport (60), threading/synchronization (5) | not built |
| `Combat` | Win32 (x86) Static Library | 265 | 0 | string/encoding/WCHAR (61), old MSVC/C runtime spelling (54), threading/synchronization (43), MFC/ATL/COM (20) | not built |
| `ww3d2` | Win32 (x86) Static Library | 233 | 5 | DirectDraw/Direct3D (133), string/encoding/WCHAR (81), threading/synchronization (44), old MSVC/C runtime spelling (35) | not built |
| `wwphys` | Win32 (x86) Static Library | 193 | 0 | old MSVC/C runtime spelling (48), threading/synchronization (28), DirectSound/Miles/audio backend (4), Win32 window/message loop (3) | not built |
| `wwui` | Win32 (x86) Static Library | 84 | 0 | string/encoding/WCHAR (158), Win32 window/message loop (42), threading/synchronization (11), WinSock/network transport (4) | not built |
| `Scripts` | Win32 (x86) Dynamic-Link Library | 82 | 1 | old MSVC/C runtime spelling (99), threading/synchronization (15), Win32 window/message loop (4) | not built |

## WWAudio

- Project: `Code/WWAudio/WWAudio.dsp`
- Type: Win32 (x86) Static Library
- Sources in manifest: 43
- Signal hits:
  - DirectSound/Miles/audio backend: 51 hit(s); examples: `Code/WWAudio/AudibleSound.cpp`, `Code/WWAudio/Sound3D.cpp`, `Code/WWAudio/WWAudio.cpp`, `Code/WWAudio/WWAudio.h`, `Code/WWAudio/AudibleSound.h`
  - Win32 window/message loop: 2 hit(s); examples: `Code/WWAudio/Threads.h`, `Code/WWAudio/WWAudio.cpp`
  - old MSVC/C runtime spelling: 2 hit(s); examples: `Code/WWAudio/Threads.cpp`, `Code/WWAudio/Threads.h`
  - threading/synchronization: 127 hit(s); examples: `Code/WWAudio/AudibleSound.cpp`, `Code/WWAudio/WWAudio.cpp`, `Code/WWAudio/Threads.cpp`, `Code/WWAudio/Sound3D.cpp`, `Code/WWAudio/soundstreamhandle.cpp`

## SControl

- Project: `Code/SControl/SControl.dsp`
- Type: Win32 (x86) Static Library
- Sources in manifest: 4
- Signal hits:
  - WinSock/network transport: 60 hit(s); examples: `Code/SControl/servercontrolsocket.cpp`, `Code/SControl/servercontrolsocket.h`, `Code/SControl/servercontrol.h`
  - threading/synchronization: 5 hit(s); examples: `Code/SControl/servercontrolsocket.cpp`, `Code/SControl/servercontrol.h`, `Code/SControl/servercontrolsocket.h`

## Combat

- Project: `Code/Combat/Combat.dsp`
- Type: Win32 (x86) Static Library
- Sources in manifest: 265
- Signal hits:
  - MFC/ATL/COM: 20 hit(s); examples: `Code/Combat/directinput.cpp`, `Code/Combat/comcentergameobj.cpp`, `Code/Combat/soldierfactorygameobj.cpp`, `Code/Combat/basecontroller.cpp`, `Code/Combat/building.cpp`
  - Win32 window/message loop: 12 hit(s); examples: `Code/Combat/debug.cpp`, `Code/Combat/directinput.cpp`, `Code/Combat/scripts.cpp`, `Code/Combat/vehicledriver.cpp`
  - old MSVC/C runtime spelling: 54 hit(s); examples: `Code/Combat/damage.cpp`, `Code/Combat/savegame.cpp`, `Code/Combat/soldier.cpp`, `Code/Combat/animcontrol.cpp`, `Code/Combat/input.cpp`
  - string/encoding/WCHAR: 61 hit(s); examples: `Code/Combat/textwindow.h`, `Code/Combat/hud.cpp`, `Code/Combat/objectivesviewer.cpp`, `Code/Combat/textwindow.cpp`, `Code/Combat/objectives.h`
  - threading/synchronization: 43 hit(s); examples: `Code/Combat/debug.cpp`, `Code/Combat/soldier.cpp`, `Code/Combat/scriptcommands.cpp`, `Code/Combat/pilot.cpp`, `Code/Combat/physicalgameobj.cpp`

## ww3d2

- Project: `Code/ww3d2/ww3d2.dsp`
- Type: Win32 (x86) Static Library
- Sources in manifest: 233
- Missing manifest sources:
  - `Code/ww3d2/FramGrab.cpp`
  - `Code/ww3d2/RequiredAssets/Dazzle.INI`
  - `Code/ww3d2/hmdldef.H`
  - `Code/ww3d2/onepassxyznduv.vsh`
  - `Code/ww3d2/snapPts.h`
- Signal hits:
  - DirectDraw/Direct3D: 133 hit(s); examples: `Code/ww3d2/dx8wrapper.cpp`, `Code/ww3d2/dx8wrapper.h`, `Code/ww3d2/pointgr.cpp`, `Code/ww3d2/ww3d.cpp`, `Code/ww3d2/dx8caps.cpp`
  - MFC/ATL/COM: 11 hit(s); examples: `Code/ww3d2/dx8wrapper.cpp`, `Code/ww3d2/dx8indexbuffer.cpp`, `Code/ww3d2/dx8vertexbuffer.cpp`
  - Win32 window/message loop: 27 hit(s); examples: `Code/ww3d2/dx8wrapper.cpp`, `Code/ww3d2/ww3d.cpp`, `Code/ww3d2/dx8wrapper.h`, `Code/ww3d2/ww3d.h`, `Code/ww3d2/agg_def.cpp`
  - old MSVC/C runtime spelling: 35 hit(s); examples: `Code/ww3d2/dx8caps.cpp`, `Code/ww3d2/assetmgr.cpp`, `Code/ww3d2/texturethumbnail.cpp`, `Code/ww3d2/rendobj.cpp`, `Code/ww3d2/animatedsoundmgr.cpp`
  - string/encoding/WCHAR: 81 hit(s); examples: `Code/ww3d2/font3d.h`, `Code/ww3d2/render2dsentence.cpp`, `Code/ww3d2/render2dsentence.h`, `Code/ww3d2/render2d.cpp`, `Code/ww3d2/render2d.h`
  - threading/synchronization: 44 hit(s); examples: `Code/ww3d2/texfcach.cpp`, `Code/ww3d2/texturethumbnail.cpp`, `Code/ww3d2/dx8caps.cpp`, `Code/ww3d2/dx8wrapper.cpp`, `Code/ww3d2/meshbuild.cpp`

## wwphys

- Project: `Code/wwphys/wwphys.dsp`
- Type: Win32 (x86) Static Library
- Sources in manifest: 193
- Signal hits:
  - DirectDraw/Direct3D: 2 hit(s); examples: `Code/wwphys/pscene.h`, `Code/wwphys/renegadeterrainpatch.cpp`
  - DirectSound/Miles/audio backend: 4 hit(s); examples: `Code/wwphys/phunits.h`
  - Win32 window/message loop: 3 hit(s); examples: `Code/wwphys/Path.cpp`, `Code/wwphys/pathsolve.cpp`, `Code/wwphys/vistable.cpp`
  - old MSVC/C runtime spelling: 48 hit(s); examples: `Code/wwphys/trackedvehicle.cpp`, `Code/wwphys/pathmgr.cpp`, `Code/wwphys/pathsolve.cpp`, `Code/wwphys/vehicledazzle.cpp`, `Code/wwphys/Pathfind.cpp`
  - threading/synchronization: 28 hit(s); examples: `Code/wwphys/vistable.cpp`, `Code/wwphys/rbody.cpp`, `Code/wwphys/animcollisionmanager.h`, `Code/wwphys/phys.h`, `Code/wwphys/animcollisionmanager.cpp`

## wwui

- Project: `Code/wwui/wwui.dsp`
- Type: Win32 (x86) Static Library
- Sources in manifest: 84
- Signal hits:
  - Win32 window/message loop: 42 hit(s); examples: `Code/wwui/IMEManager.cpp`, `Code/wwui/IMEManager.h`, `Code/wwui/wwuiinput.cpp`, `Code/wwui/IMECandidate.cpp`, `Code/wwui/wwuiinput.h`
  - WinSock/network transport: 4 hit(s); examples: `Code/wwui/listctrl.cpp`, `Code/wwui/listctrl.h`, `Code/wwui/treectrl.h`
  - old MSVC/C runtime spelling: 2 hit(s); examples: `Code/wwui/treectrl.cpp`, `Code/wwui/treectrl.h`
  - string/encoding/WCHAR: 158 hit(s); examples: `Code/wwui/textmarqueectrl.cpp`, `Code/wwui/stylemgr.cpp`, `Code/wwui/listctrl.h`, `Code/wwui/listctrl.cpp`, `Code/wwui/editctrl.cpp`
  - threading/synchronization: 11 hit(s); examples: `Code/wwui/IMEManager.cpp`, `Code/wwui/stylemgr.cpp`, `Code/wwui/comboboxctrl.cpp`, `Code/wwui/dialogbase.cpp`, `Code/wwui/dialogmgr.cpp`

## Scripts

- Project: `Code/Scripts/Scripts.dsp`
- Type: Win32 (x86) Dynamic-Link Library
- Sources in manifest: 82
- Missing manifest sources:
  - `Code/Scripts/slnode.h`
- Signal hits:
  - Win32 window/message loop: 4 hit(s); examples: `Code/Scripts/DLLmain.cpp`, `Code/Scripts/DPrint.cpp`
  - old MSVC/C runtime spelling: 99 hit(s); examples: `Code/Scripts/Mission01.cpp`, `Code/Scripts/Mission11.cpp`, `Code/Scripts/Mission07.cpp`, `Code/Scripts/DPrint.h`, `Code/Scripts/Mission04.cpp`
  - threading/synchronization: 15 hit(s); examples: `Code/Scripts/Test_Cinematic.cpp`, `Code/Scripts/scriptcommands.h`, `Code/Scripts/scripts.cpp`, `Code/Scripts/DPrint.cpp`, `Code/Scripts/Mission02.cpp`
