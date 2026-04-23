/*
** Non-Windows socket type bridge for the EA-baseline CMake scaffold.
**
** This is intentionally narrow: it only exposes the WinSock type names needed by
** wwnet headers while the live socket/session sources remain deferred on
** non-Windows bootstrap builds.
*/

#ifndef WWNET_SOCKET_COMPAT_H
#define WWNET_SOCKET_COMPAT_H

#if defined(_WIN32)
#include <winsock.h>
#else
#include <netinet/in.h>
#include <sys/socket.h>

typedef int SOCKET;
typedef struct sockaddr SOCKADDR;
typedef struct sockaddr * LPSOCKADDR;
typedef struct sockaddr_in SOCKADDR_IN;
typedef struct sockaddr_in * LPSOCKADDR_IN;

#ifndef INVALID_SOCKET
#define INVALID_SOCKET (-1)
#endif

#ifndef SOCKET_ERROR
#define SOCKET_ERROR (-1)
#endif

#endif

#endif
