# Helpers for modeling original EA Visual Studio 6 projects in CMake.
#
# The initial modernization phase is descriptive, not architectural: targets keep
# their original project names, and source lists are generated from the parsed
# VC6 manifest. This gives us a checkable bridge from legacy metadata to modern
# build tooling before semantic refactors or physical file moves.

function(ea_normalize_legacy_sources out_var)
  set(normalized_sources)
  foreach(source IN LISTS ARGN)
    if(IS_ABSOLUTE "${source}")
      list(APPEND normalized_sources "${source}")
    else()
      list(APPEND normalized_sources "${CMAKE_CURRENT_LIST_DIR}/${source}")
    endif()
  endforeach()
  set(${out_var} ${normalized_sources} PARENT_SCOPE)
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

  ea_normalize_legacy_sources(target_sources_normalized ${EA_VC6_SOURCES})
  ea_filter_existing_sources(${target_name} target_sources_existing ${target_sources_normalized})

  add_library(${target_name} STATIC ${target_sources_existing})
  target_include_directories(${target_name} PUBLIC
    "${CMAKE_CURRENT_LIST_DIR}"
    ${EA_VC6_INCLUDE_DIRS}
  )

  if(EA_VC6_DEFINES)
    target_compile_definitions(${target_name} PUBLIC ${EA_VC6_DEFINES})
  endif()

  if(EA_VC6_LINK_LIBRARIES)
    target_link_libraries(${target_name} PUBLIC ${EA_VC6_LINK_LIBRARIES})
  endif()
endfunction()
