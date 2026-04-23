# Stub Contracts — Missing-from-Baseline Headers

**Status:** Documentation of intentionally stubbed headers that were absent from the EA baseline but referenced by other source files.

These stubs exist to unblock compilation. They are **not** complete implementations. This document defines what a full implementation would need to provide.

---

## 1. `gamespybanlist.h` — GameSpy Ban List

**Location:** `Code/Commando/gamespybanlist.h`
**Stub status:** Forward declaration + empty class
**Referenced by:** `player.h → playermanager.h`

### What it is

GameSpy's server-side ban management. Handles ban lists for multiplayer game servers — checks whether a player/user is banned before allowing connection.

### What a full implementation needs

```cpp
class GameSpyBanListClass {
public:
    GameSpyBanListClass();
    ~GameSpyBanListClass();

    // Load ban list from a data source (file, DB, HTTP)
    bool LoadBanList(const char* filename);
    
    // Check if a player is banned
    bool IsBanned(const char* player_name) const;
    bool IsBannedByIP(const char* ip_address) const;
    bool IsBannedByCDKey(const char* cd_key_hash) const;

    // Manage bans
    void AddBan(const char* player_name, const char* reason, const char* banned_by);
    void RemoveBan(const char* player_name);
    void ClearAllBans();

    // Persistence
    void SaveBanList(const char* filename);
};
```

### Notes

- Requires GameSpy SDK or reverse-engineered protocol
- Pure client-side (FDS doesn't need it)
- Needed for: product/client multiplayer anti-cheat

---

## 2. `Notify.h` — Observer Registration Pattern

**Location:** `Code/WWLib/Notify.h`
**Stub status:** Template no-op stub
**Referenced by:** `player.h`, `DlgMPTeamSelect.h`, `natter.h`, `AutoStart.h`, `wolgmode.h`

### What it is

A static Observer<T> pattern for event registration. The game uses it to subscribe components to WWOnline session events (user joined, channel message, game start, etc.).

```cpp
// Usage pattern in Renegade:
Observer<GameOptionsMessage>::NotifyMe(*mWOLSession);
Observer<WWOnline::ServerError>::StopObserving();
```

### What a full implementation needs

```cpp
template<typename T>
class Observer {
public:
    // Register an object to receive events of type T
    template<typename U>
    static void NotifyMe(U& obj);
    
    // Unregister
    template<typename U>
    static void StopObserving(U* obj = nullptr);
    
    // Dispatch event to all registered observers
    static void Notify(const T& event);
};
```

Key requirements:
- Thread-safe static registration table
- Automatic cleanup on observer destruction
- Support for multiple observer types per event type
- Non-intrusive (observer does not inherit from a base class)

### Notes

This is a core event system. A proper implementation would replace the no-op stubs throughout the online service layer. It is closely tied to the WWOnline session architecture.

---

## 3. `Signaler.h` — Event Broadcasting

**Location:** `Code/WWLib/Signaler.h`
**Stub status:** Includes `Notify.h`, minimal template stub
**Referenced by:** `DlgMPTeamSelect.h`

### What it is

Base class for Signaler<T>, an event broadcaster. Components inherit from `Signaler<T>` to send events to registered Observers.

```cpp
// Usage:
class WOLSessionClass : public Signaler<GameOptionsMessage> {
    void SendOptions(const GameOptionsMessage& msg) {
        Signal(msg); // notify all registered observers
    }
};
```

### What a full implementation needs

```cpp
template<typename T>
class Signaler {
public:
    virtual ~Signaler() = default;
    
    // Broadcast event to all registered observers
    virtual void Signal(const T& event);
    
    // Connect an observer
    template<typename U>
    void Attach(U& observer);
    
    // Disconnect
    template<typename U>
    void Detach(U& observer);
};
```

### Notes

Works with `Observer<T>` — both need a full implementation together.

---

## 4. `RefPtr.h` — Reference-Counted Smart Pointer

**Location:** `Code/WWOnline/RefPtr.h`
**Stub status:** Minimal template stub (compiles, no real refcounting)
**Referenced by:** `DlgMPTeamSelect.h`, `AutoStart.h`, `wolgmode.h`, `natter.h`

### What it is

A reference-counted smart pointer used throughout WWOnline for session, channel, user, and product objects. It manages lifetime via `AddRef()`/`Release()`.

### What a full implementation needs

The stub already has the interface. Full implementation requires:

```cpp
template<typename T>
class RefPtr {
public:
    RefPtr() : ptr_(nullptr) {}
    RefPtr(T* p) : ptr_(p) { if (ptr_) ptr_->AddRef(); }
    RefPtr(const RefPtr& other) : ptr_(other.ptr_) { if (ptr_) ptr_->AddRef(); }
    ~RefPtr() { if (ptr_) ptr_->Release(); }
    
    RefPtr& operator=(const RefPtr& other);
    
    T* operator->() const;
    T& operator*() const;
    operator T*() const;
    bool operator!() const;
    explicit operator bool() const;
    
private:
    T* ptr_;
};
```

```cpp
class RefCounted {
public:
    RefCounted() : refcount_(0) {}
    virtual ~RefCounted() {}
    void AddRef() { ++refcount_; }
    void Release() { if (--refcount_ == 0) delete this; }
protected:
    int refcount_;
};
```

### Notes

- Thread-safe `AddRef`/`Release` needed for online layer
- Used for: WWOnline session, channel, user, product objects
- The `RefCounted` base class must be the base for all WWOnline session objects

---

## 5. `WOLLangCodes.h` — WOL Language Constants

**Location:** `Code/WWOnline/WOLLangCodes.h`
**Stub status:** Empty header
**Referenced by:** (needs grep — likely UI/multiplayer code)

### What it is

Language code constants for Westwood Online's localized UI and server messages. Defines language IDs used in WOL packet protocols.

### What a full implementation needs

```cpp
namespace WOLLangCodes {
    constexpr int ENGLISH = 0;
    constexpr int GERMAN = 1;
    constexpr int FRENCH = 2;
    constexpr int SPANISH = 3;
    constexpr int ITALIAN = 4;
    constexpr int JAPANESE = 5;
    constexpr int KOREAN = 6;
    constexpr int CHINESE = 7;
    // ... more
    
    const char* GetLanguageName(int code);
    int GetLanguageCode(const char* name);
}
```

### Notes

- Pure data/constants — no logic needed
- WOL protocol is proprietary; language codes may be internal to WOL server
- Needed for: product/client WOL UI localization

---

## 6. `MenuDialog.h` — WWUI Menu Dialog Base

**Location:** `Code/WWUI/MenuDialog.h`
**Stub status:** Minimal class stub
**Referenced by:** `DlgMPTeamSelect.h`

### What it is

Base class for WWUI menu-style dialogs. Provides the framework for dialogs that act like menus in the WOL multiplayer UI.

### What a full implementation needs

```cpp
class MenuDialog {
public:
    MenuDialog();
    virtual ~MenuDialog();
    
    // Dialog lifecycle
    virtual void Show();
    virtual void Hide();
    virtual void Close();
    
    // Dialog result
    virtual void SetResult(int result);
    int GetResult() const;
    
    // Menu item management
    virtual void AddMenuItem(int id, const char* text);
    virtual void RemoveMenuItem(int id);
    virtual void SetSelectedItem(int id);
    virtual int GetSelectedItem() const;
    
    // Event handlers
    virtual void OnMenuItemSelected(int id);
    virtual void OnShow();
    virtual void OnHide();
    virtual void OnClose();
};
```

### Notes

- Part of WWUI framework (separate from Win32 dialogs)
- Required for WOL multiplayer dialogs (team select, game lobby, etc.)
- If the WWUI framework is rebuilt, this is part of it

---

## 7. `PopupDialog.h` — WWUI Popup Dialog Base

**Location:** `Code/WWUI/PopupDialog.h`
**Stub status:** Minimal class stub
**Referenced by:** `DlgDownload.h`, `DlgMPConnectionRefused.h`

### What it is

Base class for WWUI popup/message-box-style dialogs. Used for modal popups like download progress, connection refused, etc.

### What a full implementation needs

```cpp
class PopupDialog {
public:
    PopupDialog();
    virtual ~PopupDialog();
    
    // Dialog lifecycle
    virtual void Show();
    virtual void Hide();
    virtual void Close();
    
    // Content
    virtual void SetTitle(const char* title);
    virtual void SetMessage(const char* message);
    
    // Buttons
    virtual void AddButton(int id, const char* label);
    virtual void SetDefaultButton(int id);
    
    // Progress (for download dialogs)
    virtual void SetProgress(int percent);
    virtual void SetProgressText(const char* text);
    
    // Result
    virtual void SetResult(int result);
    int GetResult() const;
};
```

### Notes

- Similar to `MenuDialog` — both are WWUI framework dialogs
- `DlgDownload.h` and `DlgMPConnectionRefused.h` are concrete popup dialogs that inherit from this

---

## 8. `WOLGameInfo.h` — WOL Game Info

**Location:** `Code/Commando/WOLGameInfo.h`
**Stub status:** Minimal class stub with WWOnline namespace forward declarations
**Referenced by:** `DlgMPTeamSelect.h`

### What it is

Holds game info for a Westwood Online game session — player count, map, game mode, etc. Used in the multiplayer lobby UI.

### What a full implementation needs

```cpp
namespace WWOnline {
    class ChannelData;
    class UserData;
};

class WOLGameInfo {
public:
    WOLGameInfo();
    WOLGameInfo(WWOnline::ChannelData* channel);
    ~WOLGameInfo();
    
    // Populate from WOL channel data
    void ImportFromChannel(WWOnline::ChannelData* channel);
    void ExportToChannel(WWOnline::ChannelData* channel);
    
    // Accessors
    const char* GetGameName() const;
    int GetPlayerCount() const;
    int GetMaxPlayers() const;
    const char* GetMapName() const;
    const char* GetGameMode() const;
    bool IsPassworded() const;
    
    // Validation
    static bool IsValidGameChannel(WWOnline::ChannelData* channel);
    bool CanUserJoin(WWOnline::UserData* user) const;
    
    // WOLBrowser dependency removed — stub does not include WOLBrowser.h
};
```

### Notes

- The stub deliberately excludes `WOLBrowser.h` (COM/IDL ActiveX, Windows-only) to avoid that dependency
- Without WOLBrowser, the stub cannot fully reconstruct the game info synchronization
- This is primarily a client/UI concern, not FDS

---

## Summary Table

| Stub | Needed for | Win32-only? | Priority |
|------|-----------|-------------|---------|
| `gamespybanlist.h` | Client multiplayer anti-cheat | Yes (GameSpy SDK) | Low |
| `Notify.h` + `Signaler.h` | WWOnline event system | Yes | Medium |
| `RefPtr.h` | WWOnline object lifetime | Yes | Medium |
| `WOLLangCodes.h` | WOL localization | Yes | Low |
| `MenuDialog.h` | WOL menu dialogs | Yes | Low |
| `PopupDialog.h` | WOL popup dialogs | Yes | Low |
| `WOLGameInfo.h` | WOL game lobby | Yes | Low |
| `AudibleSound.h` | Audio subsystem | Yes | Medium |

**Priority guide:**
- **Low:** Only needed for client UI/online, not FDS
- **Medium:** Needed for WWOnline session layer (affects client online features)
- **High:** (none yet — the big seams are renderer, audio SDK, and math types)
