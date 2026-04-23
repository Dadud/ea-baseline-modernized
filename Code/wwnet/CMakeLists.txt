# Manifest-derived from Code/wwnet/wwnet.dsp.
#
# wwnet is the first low-level networking target in the scaffold. It is
# intentionally modeled with the original VC6 target name before any future
# platform/network bucket split. The source variables below classify the old
# bucket logically; they do not move files or rename the target.

set(WWNET_PACKET_OBJECT_SOURCES
    ./fromaddress.h
    ./msgstat.cpp
    ./msgstat.h
    ./msgstatlist.cpp
    ./msgstatlist.h
    ./msgstatlistgroup.cpp
    ./msgstatlistgroup.h
    ./netstats.cpp
    ./netstats.h
    ./networkobject.cpp
    ./networkobject.h
    ./networkobjectfactory.cpp
    ./networkobjectfactory.h
    ./networkobjectfactorymgr.cpp
    ./networkobjectfactorymgr.h
    ./networkobjectmgr.cpp
    ./networkobjectmgr.h
    ./packettype.h
    ./wwpacket.cpp
    ./wwpacket.h
)

set(WWNET_LIVE_SOCKET_SESSION_SOURCES
    ./connect.cpp
    ./connect.h
    ./netutil.cpp
    ./netutil.h
    ./packetmgr.cpp
    ./packetmgr.h
    ./singlepl.cpp
    ./singlepl.h
)

set(WWNET_BANDWIDTH_REMOTE_HOST_SOURCES
    ./BWBalance.cpp
    ./BWBalance.h
    ./rhost.cpp
    ./rhost.h
)

set(WWNET_SOCKET_TYPE_BRIDGE_SOURCES
    ./socket_compat.h
)

set(WWNET_SOURCES
    ${WWNET_PACKET_OBJECT_SOURCES}
    ${WWNET_LIVE_SOCKET_SESSION_SOURCES}
    ${WWNET_BANDWIDTH_REMOTE_HOST_SOURCES}
    ${WWNET_SOCKET_TYPE_BRIDGE_SOURCES}
)

# The original wwnet target includes both packet/object serialization helpers
# and the live WinSock connection/session layer. Keep the source inventory
# visible, but defer the live socket/session policy sources from the
# non-Windows bootstrap until a deliberate platform networking layer exists.
if(NOT WIN32)
  set(WWNET_SOURCES
    ${WWNET_PACKET_OBJECT_SOURCES}
    ${WWNET_SOCKET_TYPE_BRIDGE_SOURCES}
    ./connect.h
    ./netutil.h
    ./packetmgr.h
    ./rhost.h
    ./singlepl.h
    ./BWBalance.h
  )
endif()

ea_vc6_static_library(wwnet
  SOURCES
    ${WWNET_SOURCES}
  INCLUDE_DIRS
    .
    ..
    ../wolapi
    ../wwbitpack
    ../wwdebug
    ../wwlib
    ../WWMath
    ../wwonline
    ../wwsaveload
    ../wwutil
  DEFINES
    WWDEBUG
  LINK_LIBRARIES
    wwdebug
    wwlib
    wwmath
    wwbitpack
    wwsaveload
    wwtranslatedb
    wwutil
)
