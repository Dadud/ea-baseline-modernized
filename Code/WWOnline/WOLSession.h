// WWOnline/WOLSession.h - Westwood Online session management
// Status: STUB (WWOnline platform-specific multiplayer infrastructure)
#ifndef __WOLSESSION_H__
#define __WOLSESSION_H__

#include "WOLLoginInfo.h"
#include "WOLUser.h"
#include "WOLChannel.h"
#include "WOLPing.h"
#include "WOLAgeCheck.h"
#include "WOLDownload.h"
#include "WOLPageMsg.h"
#include "RefPtr.h"

namespace WWOnline {

    // Connection status enum (used by SessionClass and WOLConnect)
    enum ConnectionStatus {
        ConnectionDisconnected = 0,
        ConnectionConnecting   = 1,
        ConnectionConnected   = 2,
        ConnectionDisconnecting = 3
    };

    // Forward declarations for WWOnline session types
    class SessionClass;
    class UserManager;
}

#endif // __WOLSESSION_H__
