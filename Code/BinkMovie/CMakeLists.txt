# Conservative scaffold for the original EA VC6 BinkMovie static library.
#
# This target keeps the original project name and physical layout. The source
# groups below make the historical bucket boundaries explicit before any final
# architecture or file moves happen.

set(BINKMOVIE_SUBTITLE_DATA_SOURCES
  ./subtitle.cpp
  ./subtitle.h
  ./subtitleparser.cpp
  ./subtitleparser.h
)

set(BINKMOVIE_SUBTITLE_RENDER_SOURCES
  ./subtitlemanager.cpp
  ./subtitlemanager.h
)

set(BINKMOVIE_RAD_PLAYER_SOURCES
  ./BINK.H
  ./BINKMovie.cpp
  ./BINKMovie.h
  ./RAD.H
)

set(BINKMOVIE_SOURCES
  ${BINKMOVIE_SUBTITLE_DATA_SOURCES}
  ${BINKMOVIE_SUBTITLE_RENDER_SOURCES}
  ${BINKMOVIE_RAD_PLAYER_SOURCES}
)

# Non-Windows (Linux/macOS): defer the RAD/Bink SDK path and subtitle renderer
# glue. On Windows with the real SDKs installed, these compile natively.
if(UNIX)
  list(REMOVE_ITEM BINKMOVIE_SOURCES
    ${BINKMOVIE_SUBTITLE_RENDER_SOURCES}
    ${BINKMOVIE_RAD_PLAYER_SOURCES}
  )
endif()

ea_vc6_static_library(BinkMovie
  SOURCES
    ${BINKMOVIE_SOURCES}
  INCLUDE_DIRS
    ..//
    ../DirectX/Inc
    ../ww3d2
    ../wwdebug
    ../wwlib
    ../wwmath
  DEFINES
    NDEBUG
    WIN32
    WINVER=0x400
    WWDEBUG
    _DEBUG
    _WINDOWS
  LINK_LIBRARIES
    wwdebug
    wwlib
    wwmath
)
