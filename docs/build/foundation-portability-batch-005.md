# Foundation Portability Batch 005: `wwnet` Source Boundary

## Purpose

Batch 005 expands the EA-baseline CMake scaffold one layer above the foundation/support libraries by modeling the original `wwnet` VC6 project without jumping to `Commando`, renderer, UI, WOL, or product targets.

The goal is not to finish cross-platform networking. The goal is to make the original networking bucket visible, compile the packet/object serialization side where safe, and clearly mark the live WinSock session/socket layer as a platform seam for later design.

## Target Added

- Original project: `Code/wwnet/wwnet.dsp`
- CMake target: `wwnet`
- Source of truth: generated VC6 manifest in `build-manifests/vc6-projects.json`

`wwnet` is included under the current `RENEGADE_BUILD_FOUNDATION_LIBS` scaffold because the target is a low-level dependency of `commando`, `Combat`, and other runtime systems. This is a temporary bootstrap grouping, not a final architecture statement.

## Source Boundary Found

The original `wwnet` project mixes two related but different concerns:

1. packet/stat/message/network-object helpers that can compile as data/serialization support; and
2. live WinSock session, socket, bandwidth, remote-host, and LAN servicing code.

For the non-Windows bootstrap, the following live socket/session sources are deferred:

```text
BWBalance.cpp
connect.cpp
netutil.cpp
packetmgr.cpp
rhost.cpp
singlepl.cpp
```

The headers remain in the manifest-derived source inventory so the original project shape stays visible.

## Narrow Compatibility Added

A local header was added:

```text
Code/wwnet/socket_compat.h
```

It only bridges WinSock type names needed by headers used from the packet/object side of `wwnet` on non-Windows:

- `SOCKET`
- `SOCKADDR`
- `LPSOCKADDR`
- `SOCKADDR_IN`
- `LPSOCKADDR_IN`
- `INVALID_SOCKET`
- `SOCKET_ERROR`

This is intentionally not a socket implementation and does not attempt to port `connect.cpp`, `netutil.cpp`, or `packetmgr.cpp`. A real platform networking layer should replace this boundary later.

## Verification

Configured and built the new target:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold --target wwnet -j4
```

Result:

```text
[100%] Built target wwnet
```

The full scaffold also now includes `wwnet` through the top-level CMake file.

## Modernization Rule Reinforced

Do not treat WinSock as a missing include problem. The socket/session sources are part of the future `platform`/network boundary and should be split deliberately rather than papered over with fake WinSock behavior.
