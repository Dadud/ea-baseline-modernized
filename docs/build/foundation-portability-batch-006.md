# Foundation Portability Batch 006: Logical Source Islands

## Purpose

Batch 006 pauses target expansion and makes the current scaffold easier to understand. It keeps original EA target names and file locations, but changes the `wwlib` and `wwnet` CMake source lists from one large bucket plus an unexplained filter into named logical source islands.

This supports the modernization goal of reorganizing the codebase into understandable buckets before physically moving files.

## What changed

### `wwlib`

`Code/wwlib/CMakeLists.txt` now classifies manifest-derived sources into named variables:

- `WWLIB_FOUNDATION_SOURCES`
- `WWLIB_PLATFORM_SOURCES`
- `WWLIB_DISPLAY_SURFACE_SOURCES`
- `WWLIB_INPUT_MESSAGE_SOURCES`
- `WWLIB_REGISTRY_VERSION_SOURCES`
- `WWLIB_ENCODING_STRING_SOURCES`
- `WWLIB_CONTENT_ARCHIVE_SOURCES`
- `WWLIB_WINDOWS_INTEGRATION_SOURCES`
- `WWLIB_DIAGNOSTIC_CONSOLE_SOURCES`

The CMake target is still named `wwlib`. No files moved.

The non-Windows bootstrap still defers the same compile-blocking implementation files that represent unresolved platform/display/input/registry/archive/string seams. The difference is that the source inventory now explains which future architecture island those files belong to.

### `wwnet`

`Code/wwnet/CMakeLists.txt` now classifies sources into:

- `WWNET_PACKET_OBJECT_SOURCES`
- `WWNET_LIVE_SOCKET_SESSION_SOURCES`
- `WWNET_BANDWIDTH_REMOTE_HOST_SOURCES`
- `WWNET_SOCKET_TYPE_BRIDGE_SOURCES`

The CMake target is still named `wwnet`. No files moved.

On non-Windows, the scaffold still builds packet/stat/network-object helpers and keeps socket/session implementation sources deferred until a real platform networking layer exists.

## Why this matters

Before this batch, the scaffold had two risks:

1. deferred source filters could become unexplained build hacks; and
2. future work could mistake a compileable subset for the final architecture.

After this batch, the CMake files are also architecture notes. They document where the historical EA buckets split across the future 9-bucket model.

## Verification

Expected verification command:

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold -j4
```

Expected result remains:

```text
[100%] Built target wwnet
```

## Rules preserved

- Original VC6 target names are preserved.
- Physical file layout is unchanged.
- No OpenW3D fork changes are replayed.
- No product targets are modeled yet.
- No fake DirectDraw or fake WinSock implementation is added.
