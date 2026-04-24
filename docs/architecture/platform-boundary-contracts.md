# Platform Boundary Contracts

**Status:** Phase 3 — boundary definitions for recurring platform seams

This document defines the minimal interfaces that the `platform` bucket must provide to the rest of the codebase. These are design contracts — they describe what the platform layer must expose, not how it is implemented.

---

## 1. Threading and Synchronization

**File:** `Code/platform/threading.h` (future location)

```cpp
#pragma once

#include <cstdint>

// Thread handle — opaque to callers
using ThreadHandle = void*;
using ThreadID = uint32_t;

// Thread priority
enum class ThreadPriority {
    Low,
    Normal,
    High,
    Critical
};

// Thread creation
ThreadHandle CreateThread(
    void (*entry)(void*),
    void* userdata,
    ThreadPriority priority = ThreadPriority::Normal,
    uint32_t stack_size = 0 // 0 = default
);
void CloseThread(ThreadHandle handle);
int WaitForThread(ThreadHandle handle, uint32_t timeout_ms); // 0 = infinite, returns 0 on success
void ExitThread(int exit_code);

// Thread identity
ThreadID GetCurrentThreadID();
void SetThreadPriority(ThreadHandle handle, ThreadPriority priority);

// Mutex
class Mutex {
public:
    Mutex();
    ~Mutex();
    void Lock();
    bool TryLock();
    void Unlock();

private:
    void* _impl; // opaque
};

// Auto-lock guard
class MutexLock {
public:
    explicit MutexLock(Mutex& m) : _mutex(m) { _mutex.Lock(); }
    ~MutexLock() { _mutex.Unlock(); }
private:
    Mutex& _mutex;
};

// Event / waitable
class Event {
public:
    Event(bool manual_reset = false);
    ~Event();
    void Set();       // signal
    void Reset();     // non-manual only
    bool Wait(uint32_t timeout_ms); // returns true if signaled
};
```

**Current implementation:** `Code/wwlib/thread.cpp` now has an opt-in SDL3-backed non-Windows thread path when `RENEGADE_USE_SDL3=ON`. On Windows, native thread APIs remain in place.

**Deferred work:** Thread priority mapping on Linux is still approximate, event/mutex modernization is separate work, and broader platform synchronization cleanup still remains.

---

## 2. Filesystem

**File:** `Code/platform/filesystem.h` (future location)

```cpp
#pragma once

#include <cstdio>
#include <cstdint>
#include <string>

// File open mode
enum class FileMode {
    Read,
    Write,
    Append,
    ReadWrite
};

// File access
enum class FileAccess {
    Read  = 1 << 0,
    Write = 1 << 1,
};

// Seek origin
enum class SeekOrigin {
    Begin,
    Current,
    End,
};

// File handle — opaque
using FileHandle = void*;

// Open/close
FileHandle FileOpen(const char* path, FileMode mode);
void FileClose(FileHandle f);

// Read/write
uint64_t FileRead(FileHandle f, void* buf, uint64_t size);
uint64_t FileWrite(FileHandle f, const void* buf, uint64_t size);

// Seek
uint64_t FileSeek(FileHandle f, int64_t offset, SeekOrigin origin);
uint64_t FileTell(FileHandle f);

// Size
uint64_t FileSize(FileHandle f);
bool FileExists(const char* path);

// Directory iteration (for tools)
struct DirectoryEntry {
    char name[256];
    bool is_directory;
};
using DirectoryHandle = void*;
DirectoryHandle DirectoryOpen(const char* path);
bool DirectoryRead(DirectoryHandle dir, DirectoryEntry* out_entry);
void DirectoryClose(DirectoryHandle dir);
```

**Current implementation:** `Code/wwlib/rawfile.cpp` and `Code/wwtranslatedb/translatedb.cpp` have partial implementations. `Code/wwlib/mixfile.cpp` has MIX archive iteration.

**Deferred work:** Win32 FindFirstFile/FindNextFile is used throughout the codebase. A portable `DirectoryOpen/Read/Close` interface is needed. The MIX archive format is EA-specific and must be preserved for asset compatibility.

---

## 3. Networking / Sockets

**File:** `Code/platform/socket.h` (future location)

```cpp
#pragma once

#include <cstdint>
#include <cstddef>

// Socket handle — opaque
using SocketHandle = void*;

// Address family
enum class AddressFamily {
    Internet = 2, // AF_INET
};

// Socket type
enum class SocketType {
    Stream = 1,  // SOCK_STREAM (TCP)
    Datagram = 2, // SOCK_DGRAM (UDP)
};

// Protocol
enum class Protocol {
    TCP = 6,
    UDP = 17,
};

// Socket creation/destruction
SocketHandle SocketCreate(AddressFamily af, SocketType type, Protocol protocol);
void SocketClose(SocketHandle s);

// Bind/listen/accept (server)
int SocketBind(SocketHandle s, uint16_t port);
int SocketListen(SocketHandle s, int backlog);
SocketHandle SocketAccept(SocketHandle s, uint32_t timeout_ms); // nullptr on timeout

// Connect (client)
int SocketConnect(SocketHandle s, const char* host, uint16_t port, uint32_t timeout_ms);

// Send/recv
int SocketSend(SocketHandle s, const void* buf, int len);
int SocketReceive(SocketHandle s, void* buf, int len);

// Host byte order to network byte order
uint16_t Htons(uint16_t v);
uint16_t Ntohs(uint16_t v);
uint32_t Htonl(uint32_t v);
uint32_t Ntohl(uint32_t v);
```

**Current implementation:** `Code/wwnet/socket_compat.h` provides only type aliases. Live WinSock sources are deferred on non-Windows.

**Deferred work:** A real cross-platform socket implementation must replace the type aliases before `wwnet` live sources can build on Linux.

---

## 4. Time

**File:** `Code/platform/time.h` (future location)

```cpp
#pragma once

#include <cstdint>

// Millisecond timer — like Win32 timeGetTime()
uint32_t GetTimeMS();

// High-resolution counter
uint64_t GetPerformanceCounter();
uint64_t GetPerformanceFrequency();

// Sleep
void SleepMS(uint32_t ms);
```

**Current implementation:** `Code/wwlib/systimer.h` provides `timeGetTime()` via `gettimeofday` and `Sleep()` via `usleep`. This is a minimal shim, not a formal interface.

---

## 5. Registry / Config

**File:** `Code/platform/registry.h` (future location)

```cpp
#pragma once

#include <string>

// Registry hive (maps to Win32 HKEY)
enum class RegistryHive {
    LocalMachine,
    CurrentUser,
    Users,
};

// Registry key — opaque
using RegistryKey = void*;

// Open/create key
RegistryKey RegistryOpen(RegistryHive hive, const char* subkey, bool create = false);
void RegistryClose(RegistryKey key);

// Read/write string and DWORD
bool RegistryReadString(RegistryKey key, const char* value_name, char* buf, int buf_len);
bool RegistryReadDWORD(RegistryKey key, const char* value_name, uint32_t* out_val);
bool RegistryWriteString(RegistryKey key, const char* value_name, const char* value);
bool RegistryWriteDWORD(RegistryKey key, const char* value_name, uint32_t value);

// Config file fallback: on non-Windows platforms, registry calls can transparently fall back to an ini file or XML store.
```

**Current implementation:** `Code/wwlib/registry.cpp` uses native Win32 registry APIs. Non-Windows has no implementation.

**Deferred work:** A portable config/key-value store abstraction that works on both Windows (using registry) and Linux (using config files or a portable key-value store like libconfig).

---

## 6. Window / Shell (minimal for FDS)

**File:** `Code/platform/window.h` (future location)

For the FDS build, the platform window layer is not needed. For the client build, a minimal window creation and message pump interface is required:

```cpp
#pragma once

#include <cstdint>

// Window handle — opaque
using WindowHandle = void*;

// Window creation
WindowHandle WindowCreate(
    const char* title,
    int x, int y,
    int width, int height,
    bool fullscreen = false
);
void WindowDestroy(WindowHandle w);
void WindowShow(WindowHandle w);

// Message pump (returns false on WM_QUIT)
bool WindowPumpMessages();

// Get client HWND equivalent (for renderer)
WindowHandle WindowGetHWND(); // platform-specific handle
```

**Current implementation:** `Code/wwlib/msgloop.cpp` and `Code/wwlib/win.cpp` have Win32 implementations. Non-Windows scaffold defers these entirely.

**Deferred work:** Full window abstraction is needed only for the client build. FDS does not need it.

---

## 7. String / Encoding

**File:** `Code/platform/string.h` (future location)

```cpp
#pragma once

#include <cstdint>
#include <cstddef>

// Wide-character width decision:
// Win32 WCHAR = 16-bit (UTF-16LE)
// Linux wchar_t = 32-bit (UTF-32)
// For network and file serialization, preserve Win32 16-bit width.
// For platform-internal strings, use the native wchar_t width.
// The platform string layer must bridge these.

using WideChar = uint16_t; // Always 16-bit for serialized data

// UTF-8 <-> UTF-16 conversion helpers
int Utf8ToWideChar(const char* utf8, WideChar* wide_buf, int wide_buf_len);
int WideCharToUtf8(const WideChar* wide, char* utf8_buf, int utf8_buf_len);

// Case-insensitive comparison (locale-independent)
int StrCaseCmp(const char* a, const char* b);
int StrNCaseCmp(const char* a, const char* b, size_t n);

// TCHAR bridge — for Windows-era source compatibility
// On non-Windows, TCHAR = char and _T() = no-op
```

**Current implementation:** `Code/wwlib/wwstring.h` maps TCHAR/WCHAR to host types. `Code/wwbitpack/bitstream.h` preserves 16-bit WCHAR serialization via USHORT.

**Open issue:** The w3dhub OpenW3D fork uses ICU for Unicode normalization. This should be evaluated for adoption.

---

## Implementation priority for FDS

The FDS build needs these platform interfaces first:

1. **Threading** — mutex and event for game loop and network threads
2. **Time** — `GetTimeMS()` for game clock
3. **Filesystem** — file open/read/write/seek for asset loading
4. **Sockets** — UDP socket for game traffic

These are NOT needed for FDS:

- Window/shell (client only)
- Registry (client/launcher only)
- Full TCHAR/WCHAR bridging (client only)

This means the platform boundary can be built incrementally: threading → time → filesystem → sockets → window/registry/string.
