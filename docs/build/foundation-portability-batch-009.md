# Foundation Portability Batch 009: SControl Server-Control Seam

## Purpose

Batch 009 adds the original EA VC6 `SControl` static-library project to the conservative CMake scaffold.

`SControl` is intentionally treated as a small server-control/network seam, not as a generic foundation library. It contains the remote/server control protocol wrapper and its UDP socket transport.

## Added target

Original VC6 project:

```text
Code/SControl/SControl.dsp
```

CMake target:

```text
SControl
```

The original target name is preserved.

## Source islands

`Code/SControl/CMakeLists.txt` now records two logical source islands:

```text
SCONTROL_PROTOCOL_SOURCES
SCONTROL_SOCKET_TRANSPORT_SOURCES
```

Interpretation:

- `SCONTROL_PROTOCOL_SOURCES` contains the server-control request/response/authentication wrapper.
- `SCONTROL_SOCKET_TRANSPORT_SOURCES` contains the UDP socket transport used by that control protocol.

## Non-Windows socket handling

The first build blocker was the expected WinSock header dependency:

```text
Code/SControl/servercontrolsocket.h:45:17: fatal error: winsock.h: No such file or directory
```

This batch does **not** add a fake WinSock implementation. Instead, it adds a narrow non-Windows include/type bridge in `servercontrolsocket.h` for the specific POSIX socket APIs already used by this transport:

- `SOCKET` as `int`
- `LPSOCKADDR` as `struct sockaddr *`
- `INVALID_SOCKET` / `SOCKET_ERROR`
- `closesocket` -> `close`
- `ioctlsocket` -> `ioctl`
- `WSAGetLastError()` -> `errno`
- `WSAEWOULDBLOCK` -> `EWOULDBLOCK`
- `Sleep(milliseconds)` -> `usleep(milliseconds * 1000)`

This keeps the original Windows path intact under `_WIN32` while letting the scaffold compile the real UDP transport on POSIX-like platforms.

## Known follow-up

The target currently builds under the scaffold with legacy warnings around `int *` vs `socklen_t *` in POSIX socket calls. Those warnings remain tolerated by the temporary scaffold `-fpermissive` policy and should be cleaned up later when the shared platform/network transport boundary is designed.

## Verification

Target build:

```bash
cmake --build build/cmake-scaffold --target SControl -j4
```

Result:

```text
[100%] Built target SControl
```

Full scaffold verification should now include:

- `wwdebug`
- `wwmath`
- `wwbitpack`
- `wwsaveload`
- `wwtranslatedb`
- `wwlib`
- `wwutil`
- `wwnet`
- `BinkMovie`
- `SControl`
