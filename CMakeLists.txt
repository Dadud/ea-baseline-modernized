cmake_minimum_required(VERSION 3.20)

# EA Renegade Baseline — CMake Modernization Scaffold
#
# PURPOSE
#   Preserve the original EA Visual Studio 6 project structure as the source
#   of truth while growing a reproducible, multi-platform CMake build.
#
# ARCHITECTURAL APPROACH
#   Targets map to original VC6 project names. Source membership is preserved.
#   Platform seams are classified within CMakeLists.txt files using named
#   source groups (e.g. WWLIB_DISPLAY_SURFACE_SOURCES) so that physical file
#   reorganization can happen later without losing architectural intent.
#
# WINDOWS vs LINUX BEHAVIOR
#   On Windows: full original source compiles natively (Win32 APIs, DirectX,
#   Miles Sound System, etc.) provided the corresponding SDK paths are set.
#   On Linux/macOS (UNIX): platform-specific sources are filtered and deferred
#   so the scaffold can build without fake stubs.
#
#   The filter is controlled by `if(UNIX)` — targets that compile their full
#   source on Windows will have their Windows-specific sources filtered on UNIX.
#
# BUCKET MAPPING (9-bucket model)
#   foundation       — wwdebug, wwmath, wwbitpack, wwsaveload, wwtranslatedb
#   platform        — wwlib (filtered on UNIX), wwnet (filtered on UNIX)
#   engine_runtime   — Scripts (filtered on UNIX), Combat (needs ww3d2)
#   engine_asset_content — BinkMovie (filtered on UNIX)
#   audio           — WWAudio (needs Miles SDK)
#   input_and_control — SControl, wwui (needs Win32 text boundary)
#   renderer        — ww3d2 (needs DirectX 8 SDK)
#   physics         — wwphys (needs ww3d2 math types)
#   products        — Commando (needs all above), tools (mostly Win32/MFC)
#
# C++98 standard preserved to minimize semantic changes.
# -fpermissive bridges VC6-era constructs on GCC/Clang.

project(CnC_Renegade_EA_Baseline LANGUAGES CXX)

# MSVC implicit include directories (UCRT headers, SDK paths).
# CMake should set CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES automatically for MSVC,
# but when using NMake Makefiles the UCRT path sometimes doesn't propagate to all
# targets. This ensures the Windows SDK UCRT include path is available globally.
if(MSVC AND CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
  include_directories(SYSTEM ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
elseif(MSVC)
  # Fallback: add the Windows 10 SDK UCRT path explicitly for NMake builds.
  # Version 10.0.19041.0 matches the Windows 10 20H2 SDK commonly installed.
  include_directories(SYSTEM "C:/Program Files (x86)/Windows Kits/10/include/10.0.19041.0/ucrt")
endif()

option(RENEGADE_USE_SDL3 "Enable SDL3-backed platform seam pieces where implemented" OFF)

if(RENEGADE_USE_SDL3)
  find_package(PkgConfig REQUIRED)
  pkg_check_modules(SDL3 REQUIRED IMPORTED_TARGET sdl3)
  add_compile_definitions(OPENW3D_SDL3=1)
elseif(WIN32)
  add_compile_definitions(OPENW3D_WIN32=1)
endif()

include(cmake/EAProject.cmake)

# =============================================================================
# WINDOWS BUILD: enable what can build with available SDKs
# =============================================================================
# Default to ON for Windows users who have the SDKs installed.
# Linux/macOS users can explicitly enable individual targets.
if(WIN32)
  set(RENEGADE_BUILD_FOUNDATION_LIBS_DEFAULT ON)
  set(RENEGADE_BUILD_PLATFORM_LIBS_DEFAULT ON)
  set(RENEGADE_BUILD_ENGINE_SEAMS_DEFAULT ON)
  set(RENEGADE_BUILD_SCRIPT_SEAMS_DEFAULT ON)
  # Renderer, physics, audio, UI, Combat, and Commando default OFF on Windows
  # until the user provides SDK paths or explicitly enables them.
  set(RENEGADE_BUILD_RENDERER_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_PHYS_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_AUDIO_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_UI_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_COMBAT_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_COMMANDO_SEAMS_DEFAULT OFF)
else()
  # Linux/macOS defaults — conservative, proven portable subset only.
  set(RENEGADE_BUILD_FOUNDATION_LIBS_DEFAULT ON)
  set(RENEGADE_BUILD_PLATFORM_LIBS_DEFAULT ON)
  set(RENEGADE_BUILD_ENGINE_SEAMS_DEFAULT ON)
  set(RENEGADE_BUILD_SCRIPT_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_RENDERER_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_PHYS_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_AUDIO_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_UI_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_COMBAT_SEAMS_DEFAULT OFF)
  set(RENEGADE_BUILD_COMMANDO_SEAMS_DEFAULT OFF)
endif()

# =============================================================================
# BUCKET 1: FOUNDATION LIBRARIES
# Low-level, dependency-free utility libraries. All compile on Windows and Linux.
# =============================================================================
option(RENEGADE_BUILD_FOUNDATION_LIBS "Build foundation libraries: wwdebug, WWMath, wwbitpack, wwsaveload, wwtranslatedb" ${RENEGADE_BUILD_FOUNDATION_LIBS_DEFAULT})

if(RENEGADE_BUILD_FOUNDATION_LIBS)
  add_subdirectory(Code/wwdebug)
  add_subdirectory(Code/WWMath)
  add_subdirectory(Code/wwbitpack)
  add_subdirectory(Code/wwsaveload)
  add_subdirectory(Code/wwtranslatedb)
endif()

# =============================================================================
# BUCKET 2: PLATFORM LAYER
# Platform-abstraction libraries. On UNIX, platform-specific sources are filtered.
# On Windows, the full original source compiles natively.
# =============================================================================
option(RENEGADE_BUILD_PLATFORM_LIBS "Build platform libraries: wwlib, wwnet" ${RENEGADE_BUILD_PLATFORM_LIBS_DEFAULT})

if(RENEGADE_BUILD_PLATFORM_LIBS)
  add_subdirectory(Code/wwlib)
  add_subdirectory(Code/wwnet)
endif()

# =============================================================================
# BUCKET 3: ENGINE — RUNTIME + ASSET CONTENT
# =============================================================================
option(RENEGADE_BUILD_SCRIPT_SEAMS  "Build Scripts DLL (full source on Windows; runtime core only on UNIX)" ${RENEGADE_BUILD_SCRIPT_SEAMS_DEFAULT})
option(RENEGADE_BUILD_ENGINE_SEAMS   "Build engine seams: BinkMovie (subtitle data), SControl" ${RENEGADE_BUILD_ENGINE_SEAMS_DEFAULT})

if(RENEGADE_BUILD_ENGINE_SEAMS)
  add_subdirectory(Code/BinkMovie)
  add_subdirectory(Code/SControl)
endif()

if(RENEGADE_BUILD_SCRIPT_SEAMS)
  add_subdirectory(Code/Scripts)
endif()

# =============================================================================
# BUCKET 4: RENDERER + PHYSICS + AUDIO
# Requires the corresponding SDK to be installed and paths set.
# =============================================================================
option(RENEGADE_BUILD_RENDERER_SEAMS "Build renderer (ww3d2) — requires DirectX 8 SDK" ${RENEGADE_BUILD_RENDERER_SEAMS_DEFAULT})
option(RENEGADE_BUILD_PHYS_SEAMS     "Build physics (wwphys) — blocked by ww3d2 header tangle" ${RENEGADE_BUILD_PHYS_SEAMS_DEFAULT})
option(RENEGADE_BUILD_AUDIO_SEAMS    "Build audio (WWAudio) — requires Miles Sound System SDK" ${RENEGADE_BUILD_AUDIO_SEAMS_DEFAULT})

if(RENEGADE_BUILD_RENDERER_SEAMS)
  add_subdirectory(Code/ww3d2)
endif()

if(RENEGADE_BUILD_PHYS_SEAMS)
  add_subdirectory(Code/wwphys)
endif()

if(RENEGADE_BUILD_AUDIO_SEAMS)
  add_subdirectory(Code/WWAudio)
endif()

# =============================================================================
# BUCKET 5: INPUT + UI
# =============================================================================
option(RENEGADE_BUILD_UI_SEAMS "Build UI (wwui) — Win32 message loop + WCHAR throughout" ${RENEGADE_BUILD_UI_SEAMS_DEFAULT})

if(RENEGADE_BUILD_UI_SEAMS)
  add_subdirectory(Code/wwui)
endif()

# =============================================================================
# BUCKET 6: GAMEPLAY RUNTIME
# =============================================================================
option(RENEGADE_BUILD_COMBAT_SEAMS "Build Combat gameplay runtime — blocked by ww3d2 header tangle" ${RENEGADE_BUILD_COMBAT_SEAMS_DEFAULT})

if(RENEGADE_BUILD_COMBAT_SEAMS)
  add_subdirectory(Code/Combat)
endif()

# =============================================================================
# BUCKET 7: PRODUCT SHELL
# =============================================================================
option(RENEGADE_BUILD_COMMANDO_SEAMS "Build Commando product shell — requires all above seams" ${RENEGADE_BUILD_COMMANDO_SEAMS_DEFAULT})

if(RENEGADE_BUILD_COMMANDO_SEAMS)
  add_subdirectory(Code/Commando)
endif()

# =============================================================================
# BUCKET 8: TOOLS
# =============================================================================
option(RENEGADE_BUILD_TOOLS_SEAMS "Build tools workspace (probe individual tools before enabling)" OFF)

if(RENEGADE_BUILD_TOOLS_SEAMS)
  # Many tools are entirely Win32/MFC-coupled and have no portable subset.
  # See: docs/architecture/project-scaffold-status.md
endif()

# =============================================================================
# BUILD OPTIONS
# =============================================================================
option(RENEGADE_STRICT_SOURCE_MANIFEST "Fail configure if a manifest-listed source file is absent" OFF)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)
