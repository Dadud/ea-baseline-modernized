# Conservative scaffold for the original EA VC6 SControl static library.
#
# This target keeps the original project name and physical layout. The source
# groups below make the server-control protocol and socket transport seam
# explicit before any final platform/network architecture work happens.

set(SCONTROL_PROTOCOL_SOURCES
  ./servercontrol.cpp
  ./servercontrol.h
)

set(SCONTROL_SOCKET_TRANSPORT_SOURCES
  ./servercontrolsocket.cpp
  ./servercontrolsocket.h
)

set(SCONTROL_SOURCES
  ${SCONTROL_PROTOCOL_SOURCES}
  ${SCONTROL_SOCKET_TRANSPORT_SOURCES}
)

ea_vc6_static_library(SControl
  SOURCES
    ${SCONTROL_SOURCES}
  INCLUDE_DIRS
    ../wwdebug
    ../wwlib
  DEFINES
    NDEBUG
    WIN32
    WWDEBUG
    _DEBUG
    _LIB
    _MBCS
  LINK_LIBRARIES
    wwdebug
    wwlib
)
