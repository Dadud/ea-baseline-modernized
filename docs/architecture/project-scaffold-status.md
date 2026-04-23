# Project Scaffold Status — All 61 Original VC6 Projects



> **Last updated:** Batch 022 (2026-04-23)

> **Source:** `build-manifests/vc6-projects.json` + probe results



## Status Key



| Code | Meaning |

|------|---------|

| ✅ Built | Compiles on Linux as-is |

| ✅ Built (filtered) | Portable subset compiles; platform-specific sources classified and deferred |

| ❌ Blocked (SDK) | Missing third-party SDK (DirectX 8, Miles Sound System) |

| ❌ Blocked (header tangle) | Physical header path mixes game-logic types with SDK headers |

| ❌ Blocked (Win32) | Win32 APIs throughout (WCHAR, message loop, registry) |

| ❌ Deferred | Entire target is Win32/MFC/assembly; no portable subset exists |

| 🔶 Not modeled | CMakeLists.txt not yet created |

| ❌ N/A | Non-Windows only (COM/IDL ActiveX, assembly) |



## All Projects



| Project | Type | Bucket | Status | Notes |

|---|---|---|---|---|

| `BandTest` | Win32 (x86) Dynamic-Link Library | online_services | ❌ Deferred — entire target: WinSock2 bandwidth tester | ❌ Deferred — entire target: WinSock2 bandwidth tester |
| `BinkMovie` | Win32 (x86) Static Library | engine_asset_content | ✅ Built (filtered) — subtitle data/parser; RAD/decode deferred | ✅ Built (filtered) — subtitle data/parser; RAD/decode deferred |
| `Combat` | Win32 (x86) Static Library | engine_runtime | ❌ Blocked — 67/265 sources need ww3d2 math types | ❌ Blocked — 67/265 sources need ww3d2 math types |
| `commando` | Win32 (x86) Application | — | 🔶 Not yet probed or modeled | 🔶 Not yet probed or modeled |
| `Installer` | Win32 (x86) Application | products/installer | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `launcher` | Win32 (x86) Application | products/launcher | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `SControl` | Win32 (x86) Static Library | input_and_control | ✅ Built — console/script control | ✅ Built — console/script control |
| `Scripts` | Win32 (x86) Dynamic-Link Library | engine_runtime | ✅ Built (filtered) — libScripts.so; DLL entry deferred | ✅ Built (filtered) — libScripts.so; DLL entry deferred |
| `bandy` | Win32 (x86) Console Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `BitPackTest` | Win32 (x86) Console Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `LocalHost` | Win32 (x86) Console Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `meshtest` | Win32 (x86) Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `PhysTest` | Win32 (x86) Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `SplineTest` | Win32 (x86) Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `collide` | Win32 (x86) Console Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `mathtest` | Win32 (x86) Console Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `movietest` | Win32 (x86) Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `skeleton_gth` | Win32 (x86) Application | tests | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `AMC_imp` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `asf_imp` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `Blender2` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `ChunkView` | Win32 (x86) Application | products/tools | ❌ Deferred — entire target: MFC GUI app | ❌ Deferred — entire target: MFC GUI app |
| `Clipbord` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `CommandoUpdate` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `CopyLocked` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `LevelEdit` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `LightMap` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `MakeMix` | Win32 (x86) Console Application | products/tools | ❌ Deferred — entire target: Win32 FindFirstFile | ❌ Deferred — entire target: Win32 FindFirstFile |
| `MaxFly` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `MixViewer` | Win32 (x86) Application | products/tools | ❌ Deferred — entire target: MFC GUI app | ❌ Deferred — entire target: MFC GUI app |
| `RenRem` | Win32 (x86) Console Application | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `RenegadeGR` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `SimpleGraph` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `SkeletonHack` | Win32 (x86) Console Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `VerStamp` | Win32 (x86) Console Application | products/tools | ❌ Deferred — x86 assembly resource stamper | ❌ Deferred — x86 assembly resource stamper |
| `ViewTrans` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `W3DShellExt` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `W3DUpdate` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `W3DView` | Win32 (x86) Application | products/tools | ❌ Deferred — entire target: MFC + Direct3D viewer | ❌ Deferred — entire target: MFC + Direct3D viewer |
| `WWConfig` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed; 34 DirectX hits in probe | ❌ Deferred — not probed; 34 DirectX hits in probe |
| `WWCtrl` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — entire target: Win32 window controls DLL | ❌ Deferred — entire target: Win32 window controls DLL |
| `bin2cpp` | Win32 (x86) Console Application | products/tools | ❌ Deferred — entire target: Win32 HANDLE file I/O | ❌ Deferred — entire target: Win32 HANDLE file I/O |
| `max2w3d` | Win32 (x86) Dynamic-Link Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `pluglib` | Win32 (x86) Static Library | products/tools | ❌ Deferred — not probed, likely Win32/MFC | ❌ Deferred — not probed, likely Win32/MFC |
| `VidInit` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `wdump` | Win32 (x86) Application | products/tools | ❌ Deferred — not probed | ❌ Deferred — not probed |
| `WWAudio` | Win32 (x86) Static Library | audio | ❌ Blocked — Mss.h Miles Sound System SDK missing | ❌ Blocked — Mss.h Miles Sound System SDK missing |
| `wwmath` | Win32 (x86) Static Library | — | 🔶 Not yet probed or modeled | 🔶 Not yet probed or modeled |
| `skeleton` | Win32 (x86) Application | — | 🔶 Not yet probed or modeled | 🔶 Not yet probed or modeled |
| `skeleton` | Win32 (x86) Application | — | 🔶 Not yet probed or modeled | 🔶 Not yet probed or modeled |
| `skeleton` | Win32 (x86) Application | — | 🔶 Not yet probed or modeled | 🔶 Not yet probed or modeled |
| `ww3d2` | Win32 (x86) Static Library | renderer | ❌ Blocked — d3d8.h DirectX 8 SDK missing | ❌ Blocked — d3d8.h DirectX 8 SDK missing |
| `wwbitpack` | Win32 (x86) Static Library | foundation | ✅ Built — default scaffold | ✅ Built — default scaffold |
| `wwdebug` | Win32 (x86) Static Library | foundation | ✅ Built — default scaffold | ✅ Built — default scaffold |
| `wwlib` | Win32 (x86) Static Library | platform | ✅ Built (filtered) — portable islands only | ✅ Built (filtered) — portable islands only |
| `wwnet` | Win32 (x86) Static Library | platform | ✅ Built (filtered) — packet helpers; WinSock deferred | ✅ Built (filtered) — packet helpers; WinSock deferred |
| `wwphys` | Win32 (x86) Static Library | physics | ❌ Blocked — needs ww3d2 math types (same dir as DX8) | ❌ Blocked — needs ww3d2 math types (same dir as DX8) |
| `wwsaveload` | Win32 (x86) Static Library | foundation | ✅ Built — default scaffold | ✅ Built — default scaffold |
| `wwtranslatedb` | Win32 (x86) Static Library | foundation | ✅ Built — default scaffold | ✅ Built — default scaffold |
| `wwui` | Win32 (x86) Static Library | input_and_control | ❌ Blocked — 158 WCHAR hits, 42 Win32 msg loop hits | ❌ Blocked — 158 WCHAR hits, 42 Win32 msg loop hits |
| `wwutil` | Win32 (x86) Static Library | foundation | ✅ Built — default scaffold | ✅ Built — default scaffold |


## Count by Status



- **✅ Built:** 11

- **❌ Blocked (SDK missing):** 2 (`ww3d2`, `WWAudio`)

- **❌ Blocked (header tangle):** 2 (`wwphys`, `Combat`)

- **❌ Blocked (Win32):** 1 (`wwui`)

- **❌ Blocked (all seams):** 1 (`Commando`)

- **❌ Deferred:** 41

- **🔶 Not yet modeled:** 3

- **Total projects:** 61
