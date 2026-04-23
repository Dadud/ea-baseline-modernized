/*
** MISSING_FROM_EA_BASELINE stub.
** WWLib/Notify.h - Static Observer registration pattern.
** Referenced by: playermanager.h, DlgMPTeamSelect.h, natter.h, AutoStart.h, wolgmode.h
** Status: MISSING_FROM_EA_BASELINE
**
** This stub provides a minimal static Observer<T>::NotifyMe() / StopObserving()
** registration system sufficient to unblock product-shell compilation.
** Full event dispatch semantics not implemented.
*/

#ifndef WWLIB_NOTIFY_H
#define WWLIB_NOTIFY_H

/*
** Forward declarations for WWOnline event types.
** These event classes carry data for Observer notifications.
*/
namespace WWOnline {
	class Session;
	class UserData;
	class ChannelData;
	class Product;
	class IRCServerData;
	class ServerError;
	class UserEvent;
	class UserIPEvent;
	class ChannelEvent;
	class GameOptionsMessage;
	class ConnectionStatus;
	class DownloadWait;
	class Server;
	class GameStartEvent;
	class LadderInfoEvent;
};

class WOLPagedEvent;
class DlgWOLWaitEvent;
class WolGameModeClass;
class LadderInfoEvent;
class GameOptionsMessage;

/*
** Minimal static Observer<T> registration system.
**
** Usage pattern in Renegade:
**   Observer<GameOptionsMessage>::NotifyMe(*mWOLSession);
**   Observer<WWOnline::ServerError>::StopObserving();
**
** This is a no-op stub for Linux/non-Windows scaffold compilation.
** Full static registration tracking not implemented.
*/
template<typename T>
class Observer
{
public:
	/*
	** Register an object to receive events of type T.
	** Stub: no-op on non-Windows.
	*/
	template<typename U>
	static void NotifyMe(U& /*obj*/) {}

	/*
	** Unregister an object from receiving events of type T.
	** Stub: no-op on non-Windows.
	*/
	template<typename U>
	static void StopObserving(U* /*obj*/ = nullptr) {}
};

/*
** Minimal non-intrusive Signaler<T> base for event broadcasting.
** Used throughout Renegade's event-driven online service layer.
** This is a no-op stub.
*/
template<typename T>
class Signaler
{
public:
	virtual ~Signaler() {}
	virtual void Signal(const T& /*event*/) {}
};

#endif // WWLIB_NOTIFY_H
