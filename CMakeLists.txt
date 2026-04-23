cmake_minimum_required(VERSION 3.20)

# Conservative EA-baseline modernization scaffold.
#
# This file intentionally models the original Visual Studio 6 project names and
# source membership first. New architecture names/buckets should be introduced
# later as logical boundaries after baseline parity is understood.
project(CnC_Renegade_EA_Baseline LANGUAGES CXX)

option(RENEGADE_BUILD_FOUNDATION_LIBS "Configure low-level EA baseline static library targets" ON)
option(RENEGADE_BUILD_AUDIO_SEAMS "Configure experimental audio seam targets" OFF)
option(RENEGADE_BUILD_PHYS_SEAMS "Configure experimental physics seam targets" OFF)
option(RENEGADE_BUILD_UI_SEAMS "Configure experimental UI/input seam targets" OFF)
option(RENEGADE_BUILD_RENDERER_SEAMS "Configure experimental renderer seam targets" OFF)
option(RENEGADE_BUILD_COMBAT_SEAMS "Configure experimental gameplay/runtime seam targets" OFF)
option(RENEGADE_STRICT_SOURCE_MANIFEST "Fail configure if a manifest-listed source is missing" OFF)

set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)
set(CMAKE_CXX_EXTENSIONS ON)

include(cmake/EAProject.cmake)

if(RENEGADE_BUILD_FOUNDATION_LIBS)
  add_subdirectory(Code/wwdebug)
  add_subdirectory(Code/wwlib)
  add_subdirectory(Code/WWMath)
  add_subdirectory(Code/wwbitpack)
  add_subdirectory(Code/wwsaveload)
  add_subdirectory(Code/wwtranslatedb)
  add_subdirectory(Code/wwutil)
  add_subdirectory(Code/wwnet)
  add_subdirectory(Code/BinkMovie)
  add_subdirectory(Code/SControl)
endif()

if(RENEGADE_BUILD_AUDIO_SEAMS)
  add_subdirectory(Code/WWAudio)
endif()

if(RENEGADE_BUILD_PHYS_SEAMS)
  add_subdirectory(Code/wwphys)
endif()

if(RENEGADE_BUILD_UI_SEAMS)
  add_subdirectory(Code/wwui)
endif()

if(RENEGADE_BUILD_RENDERER_SEAMS)
  add_subdirectory(Code/ww3d2)
endif()

if(RENEGADE_BUILD_COMBAT_SEAMS)
  add_subdirectory(Code/Combat)
endif()
