# Deferred Source Ledger

This ledger tracks original EA Renegade source files or source islands that are intentionally deferred from the current portable bootstrap scaffold.

A scaffold target can build while still having deferred source debt. This table records that debt explicitly so future platform, renderer, audio, media, online, and product-shell boundaries can be designed deliberately instead of hidden by fake compatibility stubs.

## Status values

- `deferred`: intentionally not compiled in the current portable scaffold
- `boundary-designed`: a future boundary has been documented but not implemented
- `portable-implementation-started`: work has begun
- `portable-implementation-builds`: replacement or port builds
- `parity-checked`: behavior/artifact parity has been checked
- `not-needed-for-target`: confirmed unnecessary for a product/target
- `removed-with-justification`: removed intentionally with documented reason

## Known deferred sources and islands

| Target | Source / island | Island | Blocker | Reason deferred | Needed for client | Needed for FDS | Needed for tools | Required boundary | Status |
|---|---|---|---|---|---:|---:|---:|---|---|
| `wwlib` | `Code/wwlib/blit.cpp` | display/surface | DirectDraw/Direct3D | Legacy display/surface code is outside the portable foundation scaffold. | yes | no | maybe | renderer/display backend | deferred |
| `wwlib` | `Code/wwlib/convert.cpp` | display/surface | DirectDraw/Direct3D | Surface conversion belongs with display/render boundary review. | yes | no | maybe | renderer/display backend | deferred |
| `wwlib` | `Code/wwlib/ddraw.cpp` | display/surface | DirectDraw/Direct3D | DirectDraw implementation requires renderer/display boundary. | yes | no | maybe | renderer/display backend | deferred |
| `wwlib` | `Code/wwlib/dsurface.cpp` | display/surface | DirectDraw/Direct3D | Depends on legacy DirectDraw surface path. | yes | no | maybe | renderer/display backend | deferred |
| `wwlib` | `Code/wwlib/xsurface.cpp` | display/surface | DirectDraw/Direct3D | Surface/display concern, not portable foundation. | yes | no | maybe | renderer/display backend | deferred |
| `wwlib` | `Code/wwlib/jshell.cpp` | input/message | Win32 window/message loop | Shell/input handling requires product/platform boundary. | yes | maybe | maybe | platform window/input shell | deferred |
| `wwlib` | `Code/wwlib/keyboard.cpp` | input/message | Win32/input assumptions | Keyboard implementation requires input backend boundary. | yes | maybe | maybe | input backend | deferred |
| `wwlib` | `Code/wwlib/msgloop.cpp` | input/message | Win32 message loop | Message loop belongs to platform/product shell. | yes | maybe | maybe | platform window/message loop | deferred |
| `wwlib` | `Code/wwlib/registry.cpp` | registry/config | Win32 registry | Registry access needs config/platform policy. | yes | maybe | yes | platform registry/config | deferred |
| `wwlib` | `Code/wwlib/verchk.cpp` | registry/version | Win32 version APIs | Version/resource checks require platform policy. | yes | maybe | yes | platform version/resource | deferred |
| `wwlib` | `Code/wwlib/mono.cpp` | diagnostic console | Win32 console/handle assumptions | Diagnostic mono console path needs platform diagnostics design. | yes | maybe | yes | platform diagnostics | deferred |
| `wwlib` | `Code/wwlib/cpudetect.cpp` | platform CPU | x86/platform detection | CPU feature detection needs portable policy. | yes | yes | yes | platform CPU capabilities | deferred |
| `wwlib` | `Code/wwlib/mpu.cpp` | platform CPU | x86/platform assumptions | MPU/platform code needs CPU/platform boundary review. | yes | maybe | maybe | platform CPU capabilities | deferred |
| `wwlib` | `Code/wwlib/mutex.cpp` | threading/sync | platform threading | Threading implementation needs platform sync boundary. | yes | yes | yes | platform threading/synchronization | deferred |
| `wwlib` | `Code/wwlib/rawfile.cpp` | filesystem | filesystem/path | File path behavior needs platform/filesystem compatibility review. | yes | yes | yes | platform filesystem/path | deferred |
| `wwlib` | `Code/wwlib/rcfile.cpp` | content/archive | resource/file access | Resource file behavior needs content/filesystem boundary review. | yes | maybe | yes | content IO / platform filesystem | deferred |
| `wwlib` | `Code/wwlib/mixfile.cpp` | content/archive | archive/file access | MIX archive behavior is compatibility-sensitive and should be validated separately. | yes | yes | yes | engine_asset_content archive IO | deferred |
| `wwlib` | `Code/wwlib/ini.cpp` | config/content | config parsing | INI behavior is compatibility-sensitive and needs fixture parity. | yes | yes | yes | config/content parser parity | deferred |
| `wwlib` | `Code/wwlib/refcount.cpp` | foundation | unresolved scaffold classification | Deferred during support-island bootstrap; needs review before re-adding. | yes | yes | yes | foundation review | deferred |
| `wwlib` | `Code/wwlib/regexpr.cpp` | foundation/text | regex/text compatibility | Text behavior needs review before re-adding. | yes | maybe | yes | string/text compatibility | deferred |
| `wwlib` | `Code/wwlib/srandom.cpp` | foundation/random | random compatibility | Random behavior may be compatibility-sensitive. | yes | yes | yes | deterministic/random policy | deferred |
| `wwlib` | `Code/wwlib/strtok_r.cpp` | foundation/text | libc/text compatibility | Text helper needs review before re-adding. | yes | yes | yes | string/text compatibility | deferred |
| `wwlib` | `Code/wwlib/widestring.cpp` | encoding/string | WCHAR/encoding | Wide-character behavior is compatibility-sensitive across Win32/Linux. | yes | yes | yes | string/encoding/WCHAR policy | deferred |
| `wwlib` | `Code/wwlib/win.cpp` | platform/window | Win32 API | Platform/window helper needs platform boundary. | yes | maybe | maybe | platform window/shell | deferred |
| `wwnet` | `Code/wwnet/BWBalance.cpp` | bandwidth/session | WinSock/network transport | Live network/session layer deferred until socket boundary exists. | yes | yes | no | platform network transport | deferred |
| `wwnet` | `Code/wwnet/connect.cpp` | live socket/session | WinSock/network transport | Connection implementation depends on live socket APIs. | yes | yes | no | platform network transport | deferred |
| `wwnet` | `Code/wwnet/netutil.cpp` | live socket/session | WinSock/network transport | Socket utility implementation needs real transport abstraction. | yes | yes | no | platform network transport | deferred |
| `wwnet` | `Code/wwnet/packetmgr.cpp` | live socket/session | WinSock/network transport | Packet manager implementation uses live socket/session APIs. | yes | yes | no | platform network transport | deferred |
| `wwnet` | `Code/wwnet/rhost.cpp` | remote host/session | WinSock/network transport | Remote host implementation requires session/transport boundary. | yes | yes | no | platform network transport | deferred |
| `wwnet` | `Code/wwnet/singlepl.cpp` | session policy | network/session policy | Session path needs runtime/network design. | yes | yes | no | network session policy | deferred |
| `BinkMovie` | `BINKMOVIE_SUBTITLE_RENDER_SOURCES` | subtitle rendering | renderer/presentation glue | Subtitle rendering needs renderer/presentation boundary. | yes | no | maybe | renderer presentation adapter | deferred |
| `BinkMovie` | `BINKMOVIE_RAD_PLAYER_SOURCES` | media/video backend | RAD/Bink SDK | RAD/Bink playback requires media backend and third-party SDK policy. | yes | no | maybe | media/video backend | deferred |
| `WWAudio` | `WWAUDIO_MILES_BACKEND_SOURCES` | audio backend | DirectSound/Miles/audio backend | Concrete runtime implementation depends on proprietary Miles Sound System and legacy DirectSound assumptions. | yes | maybe | maybe | audio backend | deferred |
| `WWAudio` | `WWAUDIO_THREADING_SOURCES` | platform threading | threading/synchronization | Delayed-release threading uses Win32 thread/event types and should be reviewed with the platform threading boundary. | yes | maybe | maybe | platform threading/synchronization | deferred |
| `WWAudio` | `WWAUDIO_SCENE_METADATA_SOURCES` | audio scene metadata | DirectSound/Miles/audio backend | Higher-level audio scene objects still include backend-facing `WWAudio.h` / Miles types; metadata/backend coupling needs boundary cleanup. | yes | maybe | yes | audio scene/backend split | deferred |
| `wwphys` | `WWPHYS_SCENE_RENDER_GLUE_SOURCES` | scene/render glue | renderer backend | Scene/projector/decal/Umbra-facing code is not yet below a clean portable physics boundary. | yes | maybe | maybe | renderer/runtime scene boundary | deferred |
| `wwphys` | `WWPHYS_TERRAIN_RENDER_SOURCES` | terrain/render | renderer backend | Terrain material/patch rendering and dazzle effects are renderer-facing, not portable physics core. | yes | no | maybe | terrain renderer boundary | deferred |
| `wwphys` | `WWPHYS_RUNTIME_SIM_SOURCES` | runtime simulation | Win32 window/message loop | Current runtime/path subset still leaks through shared Win32-heavy headers such as `wwlib/win.h` (`HINSTANCE`, `HWND`). | yes | yes | maybe | runtime/platform surface cleanup | deferred |
| `wwui` | `WWUI_INPUT_CURSOR_SOURCES` | input/cursor | Win32 window/message loop | Mouse/input bridge still depends on Win32 message/input surfaces and should remain outside portable UI core for now. | yes | maybe | maybe | platform window/input boundary | deferred |
| `wwui` | `WWUI_IME_SOURCES` | IME integration | Win32 IME / string-encoding | IME manager/candidate code depends on `<imm.h>`, HWND, and Windows IME APIs. | yes | no | maybe | platform IME/text input boundary | deferred |
| `wwui` | `WWUI_DIALOG_CONTROL_SOURCES` | dialog/control | renderer backend | Even dialog/control code still reaches renderer presentation headers such as `Render2D.h` and Win32 callback/handle assumptions through shared UI surfaces. | yes | maybe | yes | UI presentation / platform UI split | deferred |
| `ww3d2` | `WW3D2_DX8_BACKEND_SOURCES` | explicit DX8 backend | DirectDraw/Direct3D | DX8 wrapper/device/state implementation is explicit renderer backend code and remains outside the non-Windows seam probe. | yes | no | yes | renderer backend/device abstraction | deferred |
| `ww3d2` | `WW3D2_ASSET_CONTENT_SOURCES` | asset/content loading | renderer backend | Even asset/content management still reaches `dx8wrapper.h` and `d3d8.h`, so the renderer asset pipeline is not yet cleanly separated from backend implementation. | yes | maybe | yes | renderer asset/runtime split | deferred |
| `ww3d2` | `WW3D2_RENDER_RUNTIME_SOURCES` | scene/render runtime | Win32 window/message loop | Shared renderer/runtime code still leaks through `wwlib/win.h` (`HINSTANCE`, `HWND`) and other Win32-heavy surfaces. | yes | maybe | yes | renderer runtime / platform surface split | deferred |

## Notes

- The ledger starts with known Batch 004-007 deferred work and should be expanded as new targets are ingested.
- Some `Needed for` values are provisional until client/FDS/tools product graphs are fully modeled.
- Do not remove entries just because a scaffold target builds. Update status when a boundary is implemented or parity is checked.
