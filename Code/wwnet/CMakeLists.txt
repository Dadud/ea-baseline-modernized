# Manifest-derived from Code/wwnet/wwnet.dsp.
#
# wwnet is the first low-level networking target in the scaffold. It is
# intentionally modeled with the original VC6 target name before any future
# platform/network bucket split.
set(WWNET_SOURCES
    ./BWBalance.cpp
    ./BWBalance.h
    ./connect.cpp
    ./connect.h
    ./fromaddress.h
    ./msgstat.cpp
    ./msgstat.h
    ./msgstatlist.cpp
    ./msgstatlist.h
    ./msgstatlistgroup.cpp
    ./msgstatlistgroup.h
    ./netstats.cpp
    ./netstats.h
    ./netutil.cpp
    ./netutil.h
    ./networkobject.cpp
    ./networkobject.h
    ./networkobjectfactory.cpp
    ./networkobjectfactory.h
    ./networkobjectfactorymgr.cpp
    ./networkobjectfactorymgr.h
    ./networkobjectmgr.cpp
    ./networkobjectmgr.h
    ./packetmgr.cpp
    ./packetmgr.h
    ./packettype.h
    ./rhost.cpp
    ./rhost.h
    ./singlepl.cpp
    ./singlepl.h
    ./wwpacket.cpp
    ./wwpacket.h
)

# The original wwnet target includes both packet/object serialization helpers
# and the live WinSock connection/session layer. Keep the source inventory
# visible, but defer the live socket/session sources from the non-Windows
# bootstrap until a deliberate platform networking layer exists.
if(NOT WIN32)
  list(REMOVE_ITEM WWNET_SOURCES
    ./BWBalance.cpp
    ./connect.cpp
    ./netutil.cpp
    ./packetmgr.cpp
    ./rhost.cpp
    ./singlepl.cpp
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
