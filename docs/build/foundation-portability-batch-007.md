# Foundation portability batch 007: model `BinkMovie` as a media/content seam

Batch 007 adds the original EA VC6 `BinkMovie` static-library project to the conservative CMake scaffold.

## Scope

This batch is intentionally bounded:

- preserve the original target name: `BinkMovie`
- preserve the original physical source layout under `Code/BinkMovie`
- keep the original VC6 source inventory visible in CMake
- classify the source inventory into logical islands before final architecture/file moves
- do not add a fake RAD/Bink SDK
- do not add a fake renderer/DX8 layer

## Source islands

`Code/BinkMovie/CMakeLists.txt` now names the historical bucket as three source islands:

- `BINKMOVIE_SUBTITLE_DATA_SOURCES`
  - subtitle data object
  - subtitle control-file parser
  - portable enough to compile in the current bootstrap scaffold
- `BINKMOVIE_SUBTITLE_RENDER_SOURCES`
  - subtitle manager and text rendering glue
  - depends on renderer text primitives such as `Render2DSentenceClass`
- `BINKMOVIE_RAD_PLAYER_SOURCES`
  - Bink movie playback implementation
  - depends on the RAD/Bink SDK and concrete `ww3d2`/DX8 renderer types

On non-Windows bootstrap builds, only the subtitle data/parser island is compiled. The renderer/RAD playback islands remain visible but deferred.

## Compatibility fixes

Only narrow compatibility fixes were applied:

- `subtitleparser.h` now includes `vector.h` using a portable include spelling instead of the Windows path separator form `<wwlib\\vector.h>`.
- `subtitleparser.cpp` maps the MSVC wide-character case-insensitive helpers `wcsicmp` and `wcsnicmp` to the libc equivalents `wcscasecmp` and `wcsncasecmp` for non-MSVC compilers.

These are scaffold compatibility fixes, not a final text/Unicode architecture.

## Verified command

```bash
cmake -S . -B build/cmake-scaffold -DRENEGADE_BUILD_FOUNDATION_LIBS=ON
cmake --build build/cmake-scaffold --target BinkMovie -j4
```

Result:

```text
[100%] Built target BinkMovie
```

The full scaffold was also verified after adding the target.

## Architectural finding

`BinkMovie` is not just a movie-player leaf. It contains at least three concerns:

1. subtitle data/control parsing, which is closer to `engine_asset_content`
2. subtitle rendering glue, which belongs near the future `renderer`/presentation boundary
3. Bink/RAD playback, which is a media backend seam and depends on proprietary/third-party SDK headers plus current DX8 renderer types

That makes it a useful small target to model now, but not a target to make fully portable by stubbing third-party or renderer APIs.
