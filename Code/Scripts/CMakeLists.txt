# Conservative scaffold for the original EA VC6 Scripts DLL project.
#
# This target keeps the original project name and physical layout. The source
# groups below make DLL entry/export glue, script registration/runtime core,
# mission/toolkit content, and local support/header-mirror seams explicit
# before final product-shell and scripting architecture work.

set(SCRIPTS_DLL_ENTRY_SOURCES
  ./DLLmain.cpp
  ./DPrint.cpp
  ./DPrint.h
)

set(SCRIPTS_RUNTIME_CORE_SOURCES
  ./CustomEvents.h
  ./ScriptFactory.cpp
  ./ScriptFactory.h
  ./ScriptNode.h
  ./ScriptRegistrant.h
  ./ScriptRegistrar.cpp
  ./ScriptRegistrar.h
  ./scripts.cpp
  ./scripts.h
  ./scriptcommands.h
  ./scriptevents.h
)

set(SCRIPTS_MISSION_TOOLKIT_SOURCES
  ./DrMobius.cpp
  ./Mission00.cpp
  ./Mission00.h
  ./Mission01.cpp
  ./Mission02.cpp
  ./Mission03.cpp
  ./Mission04.cpp
  ./Mission05.cpp
  ./Mission06.cpp
  ./Mission07.cpp
  ./Mission09.cpp
  ./Mission1.h
  ./Mission10.cpp
  ./Mission10.h
  ./Mission11.cpp
  ./Mission11.h
  ./Mission2.h
  ./Mission3.h
  ./Mission4.h
  ./Mission5.h
  ./Mission6.h
  ./Mission7.h
  ./Mission8.h
  ./Mission9.h
  ./MissionDemo.cpp
  ./MissionX0.cpp
  ./MissionX0.h
  ./Test_BMG.cpp
  ./Test_Cinematic.cpp
  ./Test_DAK.cpp
  ./Test_DAY.cpp
  ./Test_DLS.cpp
  ./Test_DME.cpp
  ./Test_GTH.cpp
  ./Test_JDG_EVA.cpp
  ./Test_PDS.cpp
  ./Test_RAD.cpp
  ./Test_RMV.cpp
  ./Test_RMV_Toolkit.cpp
  ./Toolkit.cpp
  ./Toolkit.h
  ./Toolkit_Actions.cpp
  ./Toolkit_Animations.cpp
  ./Toolkit_Broadcaster.cpp
  ./Toolkit_Explosions.cpp
  ./Toolkit_Objectives.cpp
  ./Toolkit_Objects.cpp
  ./Toolkit_Powerup.cpp
  ./Toolkit_Siege.cpp
  ./Toolkit_Sounds.cpp
  ./Toolkit_Spawners.cpp
  ./Toolkit_Triggers.cpp
  ./mission08.cpp
)

set(SCRIPTS_LOCAL_SUPPORT_SOURCES
  ./actionparams.h
  ./always.h
  ./bool.h
  ./combatsound.h
  ./gameobjobserver.h
  ./slist.h
  ./string_ids.h
  ./strtrim.cpp
  ./strtrim.h
  ./vector.h
  ./vector3.h
  ./visualc.h
  ./wwmath.h
)

set(SCRIPTS_ALL_SOURCES
  ${SCRIPTS_DLL_ENTRY_SOURCES}
  ${SCRIPTS_RUNTIME_CORE_SOURCES}
  ${SCRIPTS_MISSION_TOOLKIT_SOURCES}
  ${SCRIPTS_LOCAL_SUPPORT_SOURCES}
)

set(SCRIPTS_BUILD_SOURCES
  ${SCRIPTS_ALL_SOURCES}
)

if(NOT WIN32)
  # Scripts is an original DLL/plugin seam. Keep the full inventory visible, but
  # defer the Windows DLL entry layer and the large mission/toolkit content body
  # while probing whether the script runtime core is separable from product-shell
  # and Windows export assumptions.
  list(REMOVE_ITEM SCRIPTS_BUILD_SOURCES
    ${SCRIPTS_DLL_ENTRY_SOURCES}
    ${SCRIPTS_MISSION_TOOLKIT_SOURCES}
  )
endif()

add_library(Scripts SHARED ${SCRIPTS_BUILD_SOURCES})

target_include_directories(Scripts PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(Scripts PRIVATE
  BUILDING_DLL
  NDEBUG
  WIN32
  WIN32_LEAN_AND_MEAN
  WINVER=0x400
  WWDEBUG
  _DEBUG
  _LIB
  _MBCS
  _WINDOWS
)

if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
  target_compile_options(Scripts PRIVATE -fpermissive)
endif()

set_target_properties(Scripts PROPERTIES
  OUTPUT_NAME Scripts
)
