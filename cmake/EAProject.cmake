# Helpers for modeling original EA Visual Studio 6 projects in CMake.
#
# The initial modernization phase is descriptive, not architectural: targets keep
# their original project names, and source lists are generated from the parsed
# VC6 manifest. This gives us a checkable bridge from legacy metadata to modern
# build tooling before semantic refactors or physical file moves.

# -----------------------------------------------------------------------------
# SDK path options — used on Windows when real SDKs are available.
# These are optional; targets that need missing SDKs will defer gracefully.
# -----------------------------------------------------------------------------
option(RENEGADE_USE_DX8_SDK "Enable DirectX 8 SDK path (Code/DirectX/Include)" OFF)
option(RENEGADE_USE_MILES_SDK "Enable Miles Sound System SDK path (Code/Miles/Include)" OFF)
option(RENEGADE_USE_BINK_SDK "Enable RAD Bink SDK path (Code/Bink/)" OFF)

set(RENEGADE_DX8_INCLUDE_DIR "" CACHE PATH "Path to DirectX 8 SDK Include directory")
set(RENEGADE_MILES_INCLUDE_DIR "" CACHE PATH "Path to Miles Sound System Include directory")
set(RENEGADE_BINK_INCLUDE_DIR "" CACHE PATH "Path to RAD Bink SDK Include directory")

# Helper to append an include directory if the path exists.
function(ea_add_sdk_include out_var path)
  if(EXISTS "${path}")
    list(APPEND _dirs "${path}")
    message(STATUS "  SDK include found: ${path}")
  endif()
  set(${out_var} ${_dirs} PARENT_SCOPE)
endfunction()

# Per-target SDK include aggregator. Call after the target is defined.
# Usage: ea_provide_sdk_includes(target_name DX8_DIR ... MILES_DIR ...)
function(ea_provide_sdk_includes)
  set(options)
  set(one_value_args TARGET)
  set(multi_value_args DX8_DIR MILES_DIR BINK_DIR)
  cmake_parse_arguments(EA_SDK "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  if(EA_SDK_DX8_DIR AND RENEGADE_USE_DX8_SDK AND EA_SDK_DX8_DIR)
    target_include_directories(${EA_SDK_TARGET} PRIVATE "${EA_SDK_DX8_DIR}")
  endif()
  if(EA_SDK_MILES_DIR AND RENEGADE_USE_MILES_SDK AND EA_SDK_MILES_DIR)
    target_include_directories(${EA_SDK_TARGET} PRIVATE "${EA_SDK_MILES_DIR}")
  endif()
  if(EA_SDK_BINK_DIR AND RENEGADE_USE_BINK_SDK AND EA_SDK_BINK_DIR)
    target_include_directories(${EA_SDK_TARGET} PRIVATE "${EA_SDK_BINK_DIR}")
  endif()
endfunction()

function(ea_normalize_legacy_paths out_var)
  set(normalized_paths)
  foreach(path IN LISTS ARGN)
    if(IS_ABSOLUTE "${path}")
      list(APPEND normalized_paths "${path}")
    else()
      list(APPEND normalized_paths "${CMAKE_CURRENT_LIST_DIR}/${path}")
    endif()
  endforeach()
  set(${out_var} ${normalized_paths} PARENT_SCOPE)
endfunction()

function(ea_filter_existing_sources target_name out_var)
  set(existing_sources)
  foreach(source IN LISTS ARGN)
    if(EXISTS "${source}")
      list(APPEND existing_sources "${source}")
    elseif(RENEGADE_STRICT_SOURCE_MANIFEST)
      message(FATAL_ERROR "${target_name}: manifest source does not exist: ${source}")
    else()
      message(WARNING "${target_name}: skipping manifest source that is absent from EA release tree: ${source}")
    endif()
  endforeach()
  set(${out_var} ${existing_sources} PARENT_SCOPE)
endfunction()

function(ea_vc6_static_library target_name)
  set(options)
  set(one_value_args)
  set(multi_value_args SOURCES INCLUDE_DIRS DEFINES LINK_LIBRARIES)
  cmake_parse_arguments(EA_VC6 "${options}" "${one_value_args}" "${multi_value_args}" ${ARGN})

  ea_normalize_legacy_paths(target_sources_normalized ${EA_VC6_SOURCES})
  ea_filter_existing_sources(${target_name} target_sources_existing ${target_sources_normalized})
  ea_normalize_legacy_paths(target_include_dirs_normalized ${EA_VC6_INCLUDE_DIRS})

  add_library(${target_name} STATIC ${target_sources_existing})
  if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_compile_options(${target_name} PRIVATE -fpermissive)
  endif()
  target_include_directories(${target_name} PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    ${target_include_dirs_normalized}
  )

  if(EA_VC6_DEFINES)
    target_compile_definitions(${target_name} PUBLIC ${EA_VC6_DEFINES})
  endif()

  if(EA_VC6_LINK_LIBRARIES)
    target_link_libraries(${target_name} PUBLIC ${EA_VC6_LINK_LIBRARIES})
  endif()
endfunction()
