# Batch 018: Commando Product Shell — Dependency Mapping and datasafe Win32 Gating

## What was done

### datasafe Win32 gating (narrow)

`datasafe.h` and `datasafe.cpp` had direct `<windows.h>`, `HANDLE`, mutex, and thread-ID dependencies baked into the core data-safe encryption architecture.

Narrow non-Windows compatibility was added:

- `#include <windows.h>` and `#include <malloc.h>` gated under `#ifdef _WIN32`
- `GetCurrentThreadId()` usage in `PreferredThread` init and `ThreadLockClass` gated under `#ifdef _WIN32`
- Non-Windows gets `PreferredThread = 0` as a no-op scaffold value
- `CloseHandle(SafeMutex)` in `Shutdown` is already mutex-path guarded

This preserves the `safe_int`/`safe_float` encrypted storage architecture for Linux while deferring the Windows mutex thread-safety path.

### Commando path/case fixes

- `DlgMPTeamSelect.h`: `PlayerManager.h` → `playermanager.h` (case)
- `DlgMPTeamSelect.h`, `gamedata.h`, `playermanager.h`: Windows `\\` path separators → `/`
- `wollocalemgr.h`, `mpsettingsmgr.cpp`: `wwonline\\wolsession.h` → `wwonline/wolsession.h`
- `wolgmode.h`, `wolgmode.cpp`, `natter.h`, `natter.cpp`, and 83 other Commando files: batch-fixed Windows `\\` path separators → `/`
- `mpsettingsmgr.cpp`: `WWOnline\\WOLLangCodes.h` → `WWOnline/WOLLangCodes.h`
- 10 Commando files: `WWAudio.H` → `WWAudio.h` (case)

### Missing stubs created

These are **not fake implementations** — they are minimal forward-declaration stubs for genuinely missing EA baseline headers that blocked compilation:

| File | Reason missing | Stub purpose |
|------|---------------|--------------|
| `gamespybanlist.h` | Not published in EA baseline | Forward declaration for `GameSpyBanListClass` — used by `player.h` → `playermanager.h` chain |
| `WWLib/Notify.h` | WWLib framework closed-source | Static `Observer<T>::NotifyMe()` / `StopObserving()` + `Signaler<T>` pattern for online event system |
| `WWLib/Signaler.h` | WWLib framework closed-source | Includes `Notify.h` |
| `WWOnline/RefPtr.h` | WWOnline framework closed-source | `RefPtr<T>`, `RefPtrConst<T>`, `RefCounted` smart pointers |
| `WWOnline/WOLLangCodes.h` | WWOnline framework closed-source | Language code constants stub |
| `WWUI/MenuDialog.h` | WWUI framework closed-source | Dialog base class stub |
| `WWUI/PopupDialog.h` | WWUI framework closed-source | Popup dialog stub |
| `WOLGameInfo.h` | WOLBrowser ActiveX closed-source | WOL game info stub (without WOLBrowser dependency) |
| `AudibleSound.h` | Not published in EA baseline | `AudibleSoundClass` used by `CombatGameModeClass::BackgroundMusic` |
| `datasafe.h` | Already existed | Modified: added Win32 gating |

### wwnet/rhost.h Win32 gating

`#include <winsock.h>` gated under `#ifdef _WIN32`.

### CMakeLists.txt

- Added `../WWLib`, `../WWOnline`, `../WWUI`, `../WOLBrowser` to `target_include_directories`
- 141 total files modified (87 path-separator fixes, case fixes, stubs)

## What this revealed

### datasafe is runtime-safe (with narrow caveats)

The `safe_int`/`safe_float`/`safe_double` encrypted storage types are **pure C++**. They don't need Win32 — only the optional `THREAD_SAFE_DATA_SAFE` mutex path does. On non-Windows, the thread-safety check becomes a no-op (`PreferredThread = 0`).

The `safe_*` wrappers are used extensively in `Combat` (weaponmanager, playerdata, damage) for health, shields, scores, money, etc.

### Commando is a massive WWOnline/WWUI/WWLib/WOL seam

The `Commando` product shell is deeply coupled to EA's closed-source Windows online and UI framework:

- **WWOnline**: Session management, channel events, user data, game options, connection status, ladder, download, IRC server data. 30+ header files.
- **WWUI**: Menu dialogs, popup dialogs, progress controls. Minimal dialog base classes.
- **WWLib**: Observer pattern (Notify), Signaler pattern, reference counting. Event-driven architecture.
- **WOLBrowser**: COM/IDL-generated ActiveX control with deep `rpc.h`, `ole2.h`, Windows RPC dependencies. Pure Windows UI.

Plus third-party SDK seams:
- **WWAudio/Miles**: `WWAudio.H` case issue (Miles Sound System audio SDK)
- **ww3d2**: DirectX 8 wrapper (`d3d8.h`)
- **GameSpy**: CD key auth, ban list, QR challenge

Plus missing baseline headers:
- `gamespybanlist.h` (referenced but not published)
- `AudibleSound.h` (referenced but not published)

### WWOnline/WWLib/WWUI are a unified framework

The Observer pattern (`Observer<T>::NotifyMe()`) is the backbone of the entire online service layer. `RefPtr<T>` provides reference-counted smart pointers throughout. This is a complete event-driven service architecture that ties together session management, chat, ladder, download, and game options.

## Remaining blockers for commando non-Windows build

These would need resolution for a full non-Windows `commando` build:

1. **WWOnline/WWLib/WWUI full stubs** — the event system, session management, and dialog framework require substantial reconstruction
2. **WWAudio/Miles** — `Mss.H` Miles Sound System header not found; WWAudio project itself not yet modeled
3. **ww3d2 DirectX 8** — `dx8wrapper.h` needs `d3d8.h`; ww3d2 itself not yet modeled for non-Windows
4. **`gamespybanlist.h`** — genuinely missing from baseline; needs header reconstruction
5. **`AudibleSound.h`** — genuinely missing from baseline; needs header reconstruction
6. **COM/RPC** — WOLBrowser's COM interfaces cannot be stubbed minimally

## Classification

`Commando` is classified as a **product shell seam** — it is the primary client executable target that ties together all subsystems. It is not buildable on non-Windows without either:
- Full WWOnline/WWLib/WWUI reconstruction (months of work)
- Stub implementations for the online/UI layer (changes behavior)
- A headless/FDS mode that bypasses the online/UI layers

## Default scaffold impact

None — default scaffold (9 foundation targets) remains at 10/10 targets building successfully.

## Source island classification for commando

```
COMMANDO_PRODUCT_STARTUP_SOURCES         - main(), WinMain, assert/except/logfile configs
COMMANDO_CLIENT_FRONTEND_UI_SOURCES      - UI dialogs, HUD, screens (WWUI/WWOnline dependent)
COMMANDO_RUNTIME_ORCHESTRATION_SOURCES   - game modes, init, shutdown, main loop
COMMANDO_NETWORK_GAME_SOURCES            - cNetwork, pkthandlers, netinterface
COMMANDO_ONLINE_SERVICE_SOURCES          - WOL*, AutoStart, CDKeyAuth, GameSpy (WWOnline/WWLib dependent)
COMMANDO_PLAYER_MANAGEMENT_SOURCES       - Player, PlayerManager (datasafe, WWOnline dependent)
COMMANDO_SCRIPT_EVENT_SOURCES            - Script events, console functions
COMMANDO_SAVE_LOAD_SOURCES               - Save/load, chunk I/O
COMMANDO_GAME_DATA_SOURCES               - gamedata, settings, server settings
COMMANDO_DIAGNOSTICS_SOURCES             - Debug/Diagnostic subsystems
COMMANDO_WINDOWS_ONLY_SOURCES            - WinMain, dialog resource files (.rc)
COMMANDO_DEPENDENCY_STUBS                - Missing baseline stubs
```

---

**Status**: `datasafe` Win32 gating complete. `Commando` probe complete — architectural reality documented. Default scaffold verified green.
