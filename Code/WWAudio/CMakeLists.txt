# Conservative scaffold for the original EA VC6 WWAudio static library.
#
# This target keeps the original project name and physical layout. The source
# groups below make audio event/content metadata, scene state, threading, and
# Miles/backend concerns explicit before final audio backend architecture work.

set(WWAUDIO_EVENT_SOURCES
  ./AudioEvents.cpp
  ./AudioEvents.h
)

set(WWAUDIO_SAVELOAD_SOURCES
  ./AudioSaveLoad.cpp
  ./AudioSaveLoad.h
  ./SoundChunkIDs.h
)

set(WWAUDIO_SCENE_METADATA_SOURCES
  ./AABTreeSoundCullClass.h
  ./LogicalListener.cpp
  ./LogicalListener.h
  ./LogicalSound.cpp
  ./LogicalSound.h
  ./PriorityVector.h
  ./SoundCullObj.h
  ./SoundScene.cpp
  ./SoundScene.h
  ./SoundSceneObj.cpp
  ./SoundSceneObj.h
)

set(WWAUDIO_THREADING_SOURCES
  ./Threads.cpp
  ./Threads.h
)

set(WWAUDIO_UTILITY_SOURCES
  ./Utils.cpp
  ./Utils.h
)

set(WWAUDIO_MILES_BACKEND_SOURCES
  ./AudibleSound.cpp
  ./AudibleSound.h
  ./FilteredSound.cpp
  ./FilteredSound.h
  ./Listener.cpp
  ./Listener.h
  ./Sound3D.cpp
  ./Sound3D.h
  ./SoundBuffer.cpp
  ./SoundBuffer.h
  ./SoundPseudo3D.cpp
  ./SoundPseudo3D.h
  ./WWAudio.cpp
  ./WWAudio.h
  ./listenerhandle.cpp
  ./listenerhandle.h
  ./sound2dhandle.cpp
  ./sound2dhandle.h
  ./sound3dhandle.cpp
  ./sound3dhandle.h
  ./soundhandle.cpp
  ./soundhandle.h
  ./soundstreamhandle.cpp
  ./soundstreamhandle.h
)

set(WWAUDIO_SOURCES
  ${WWAUDIO_EVENT_SOURCES}
  ${WWAUDIO_SAVELOAD_SOURCES}
  ${WWAUDIO_SCENE_METADATA_SOURCES}
  ${WWAUDIO_THREADING_SOURCES}
  ${WWAUDIO_UTILITY_SOURCES}
  ${WWAUDIO_MILES_BACKEND_SOURCES}
)

if(NOT WIN32)
  # The backend implementation depends heavily on the proprietary Miles Sound
  # System and legacy DirectSound assumptions. Keep the island visible, but
  # defer it from the portable bootstrap until an audio backend boundary exists.
  list(REMOVE_ITEM WWAUDIO_SOURCES
    ${WWAUDIO_MILES_BACKEND_SOURCES}
  )
endif()

ea_vc6_static_library(WWAudio
  SOURCES
    ${WWAUDIO_SOURCES}
  INCLUDE_DIRS
    ../DirectX/include
    ../miles6/include
    ../ww3d2
    ../wwdebug
    ../wwlib
    ../WWMath
    ../wwsaveload
  DEFINES
    G_CODE_BASE
    NDEBUG
    PARAM_EDITING_ON
    WIN32
    WIN32_LEAN_AND_MEAN
    WINVER=0x400
    WWDEBUG
    _DEBUG
    _LIB
    _MBCS
    _WINDOWS
  LINK_LIBRARIES
    wwdebug
    wwlib
    wwmath
    wwsaveload
)
