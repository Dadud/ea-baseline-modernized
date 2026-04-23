cmake_minimum_required(VERSION 3.20)

# EA Renegade Baseline — CMake Modernization Scaffold
#
# PURPOSE
#   Preserve the original EA Visual Studio 6 project structure as the source
#   of truth while growing a reproducible, multi-platform CMake build.
#
# ARCHITECTURAL APPROACH
#   Targets map to original VC6 project names.  Source membership is preserved.
#   Platform seams are classified within CMakeLists.txt files using named
#   source groups (e.g. WWLIB_DISPLAY_SURFACE_SOURCES) so that physical file
#   reorganization can happen later without losing architectural intent.
#
# BUCKET MAPPING (9-bucket model)
#   foundation       — wwdebug, wwmath, wwbitpack, wwsaveload, wwtranslatedb
#   platform        — wwlib (filtered), wwnet (filtered)
#   engine_runtime   — Scripts (filtered)
#   engine_asset_content — BinkMovie (filtered)
#   audio           — WWAudio (deferred)
#   input_and_control — SControl (filtered), wwui (deferred)
#   renderer        — ww3d2 (deferred)
#   physics         — wwphys (deferred)
#   products        — Commando (deferred), tools (deferred)
#
# BUCKET ORDER (respects compile dependencies)
#   foundation → platform → engine_runtime + engine_asset_content
#             → audio + physics → renderer
#             → input_and_control
#             → products
#
# PLATFORM GATES
#   Win32-only sources are gated with #ifdef _WIN32 in source files.
#   Non-Windows scaffold compiles only the portable/filtered source subset.
#   Do NOT add fake platform stubs; classify and defer instead.
#
# MULTI-CONFIGURATION
#   GNU/Clang are the tested compilers on Linux.  MSVC is the native target on Windows.
#   C++98 standard is preserved for now to minimize semantic changes.
#   -fpermissive is applied globally to bridge VC6-era constructs.

project(CnC_Renegade_EA_Baseline LANGUAGES CXX)

include(cmake/EAProject.cmake)

# =============================================================================
# BUCKET 1: FOUNDATION LIBRARIES
# Low-level, dependency-free utility libraries.  All compile on Linux.
# =============================================================================
option(RENEGADE_BUILD_FOUNDATION_LIBS "Build foundation libraries: wwdebug, WWMath, wwbitpack, wwsaveload, wwtranslatedb" ON)

if(RENEGADE_BUILD_FOUNDATION_LIBS)
  # wwdebug  — logging, profiling, assertions
  add_subdirectory(Code/wwdebug)

  # WWMath   — fixed-point and soft-float math (no external deps)
  add_subdirectory(Code/WWMath)

  # wwbitpack — archive/bitstream serialization (foundational for save/load, asset)
  add_subdirectory(Code/wwbitpack)

  # wwsaveload — save/load infrastructure (depends on wwbitpack, wwmath, wwlib)
  add_subdirectory(Code/wwsaveload)

  # wwtranslatedb — translation/database layer
  add_subdirectory(Code/wwtranslatedb)
endif()

# =============================================================================
# BUCKET 2: PLATFORM LAYER
# Platform-abstraction libraries.  Each has a portable subset that compiles;
# platform-specific sources are classified and deferred on non-Windows.
# =============================================================================
option(RENEGADE_BUILD_PLATFORM_LIBS "Build platform libraries: wwlib, wwnet" ON)

if(RENEGADE_BUILD_PLATFORM_LIBS)
  # wwlib — platform services (filesystem, threading, registry, string, display).
  #         WARNING: This is a MIXED bucket.  The CMakeLists.txt contains named
  #         source groups that classify every source as portable or platform-specific.
  #         Only the portable subset is compiled on Linux.  Do NOT add fake stubs.
  #         See: Code/wwlib/CMakeLists.txt for the full source island map.
  add_subdirectory(Code/wwlib)

  # wwnet — networking layer.  Packet helpers and stat tracking compile on Linux.
  #         Live WinSock session sources are deferred on non-Windows.
  #         See: Code/wwnet/CMakeLists.txt for source island map.
  add_subdirectory(Code/wwnet)
endif()

# =============================================================================
# BUCKET 3: ENGINE — RUNTIME + ASSET CONTENT
# =============================================================================
option(RENEGADE_BUILD_SCRIPT_SEAMS  "Build Scripts DLL runtime (filtered — DLL entry/Win32 paths deferred)"  OFF)
option(RENEGADE_BUILD_ENGINE_SEAMS   "Build engine seam targets: BinkMovie, SControl"                      ON)

if(RENEGADE_BUILD_ENGINE_SEAMS)
  # BinkMovie — video playback.  Subtitle data/parser compiles on Linux.
  #             RAD/Bink SDK path and subtitle renderer glue are deferred.
  #             See: Code/BinkMovie/CMakeLists.txt for source island map.
  add_subdirectory(Code/BinkMovie)

  # SControl  — script/console control.  Mostly portable C++.
  add_subdirectory(Code/SControl)
endif()

if(RENEGADE_BUILD_SCRIPT_SEAMS)
  # Scripts   — mission scripting DLL.  Produces libScripts.so on Linux.
  #             Windows DLL entrypoints and toolkit extensions are deferred.
  #             See: Code/Scripts/CMakeLists.txt for source island map.
  add_subdirectory(Code/Scripts)
endif()

# =============================================================================
# BUCKET 4: RENDERER + PHYSICS + AUDIO
# These are architecturally SEPARATE from game logic but physically share
# directories with Win32/DirectX/Miles SDK headers.  They cannot be compiled
# on Linux without first separating SDK headers from game-logic headers.
# See: docs/architecture/renderer-seam-classification.md
# =============================================================================
option(RENEGADE_BUILD_RENDERER_SEAMS "Build renderer (ww3d2) — DIRECTX 8 SDK REQUIRED" OFF)
option(RENEGADE_BUILD_PHYS_SEAMS     "Build physics (wwphys) — blocked by ww3d2 header tangle"           OFF)
option(RENEGADE_BUILD_AUDIO_SEAMS    "Build audio (WWAudio) — MILES SOUND SYSTEM SDK REQUIRED"          OFF)

if(RENEGADE_BUILD_RENDERER_SEAMS)
  add_subdirectory(Code/ww3d2)
endif()

if(RENEGADE_BUILD_PHYS_SEAMS)
  # BLOCKED: wwphys includes ww3d2 headers (Vector3, Sphere, OBBox) for math types,
  # but those headers live in the same directory as DirectX code and cannot be
  # compiled without separating game-logic math from renderer backend.
  add_subdirectory(Code/wwphys)
endif()

if(RENEGADE_BUILD_AUDIO_SEAMS)
  add_subdirectory(Code/WWAudio)
endif()

# =============================================================================
# BUCKET 5: INPUT + UI
# =============================================================================
option(RENEGADE_BUILD_UI_SEAMS "Build UI (wwui) — Win32 message loop + WCHAR throughout" OFF)

if(RENEGADE_BUILD_UI_SEAMS)
  # BLOCKED: 158 WCHAR signal hits and 42 Win32 message-loop hits across 84 sources.
  # Deep coupling to Win32 window management, IME, and wide-character text rendering.
  add_subdirectory(Code/wwui)
endif()

# =============================================================================
# BUCKET 6: GAMEPLAY RUNTIME
# =============================================================================
option(RENEGADE_BUILD_COMBAT_SEAMS "Build Combat gameplay runtime — blocked by ww3d2 header tangle" OFF)

if(RENEGADE_BUILD_COMBAT_SEAMS)
  # BLOCKED: Combat (265 sources) transitively includes ww3d2 headers for math types
  # (Vector3, Sphere, OBBox, AABox, etc.) but those headers live alongside DirectX
  # code in ww3d2.  67 of 265 Combat sources need these types.  Cannot build
  # until game-logic math types are physically separated from renderer headers.
  add_subdirectory(Code/Combat)
endif()

# =============================================================================
# BUCKET 7: PRODUCT SHELL
# =============================================================================
option(RENEGADE_BUILD_COMMANDO_SEAMS "Build Commando product shell — requires all above seams" OFF)

if(RENEGADE_BUILD_COMMANDO_SEAMS)
  add_subdirectory(Code/Commando)
endif()

# =============================================================================
# BUCKET 8: TOOLS
# =============================================================================
option(RENEGADE_BUILD_TOOLS_SEAMS "Build tools (probe individual tools before enabling)" OFF)

if(RENEGADE_BUILD_TOOLS_SEAMS)
  # Probe individual tools before adding them here.  Many tools are entirely
  # Win32/MFC-coupled and have no portable subset.
  #
  # KNOWN DEFERRALS:
  #   BandTest    — entire target: WinSock2 bandwidth tester
  #   MakeMix     — entire target: Win32 FindFirstFile directory iteration
  #   ChunkView   — entire target: MFC GUI application
  #   MixViewer   — entire target: MFC GUI application
  #   VerStamp    — entire target: x86 assembly resource stamper
  #   W3DView     — entire target: MFC + Direct3D viewer
  #   bin2cpp     — entire target: Win32 HANDLE file-to-array converter
endif()

# =============================================================================
# BUILD OPTIONS
# =============================================================================
option(RENEGADE_STRICT_SOURCE_MANIFEST "Fail configure if a manifest-listed source file is absent" OFF)

# Compiler flags chosen to minimize semantic changes during baseline modernization.
# These are TEMPORARY bridges, not final architecture.
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)
