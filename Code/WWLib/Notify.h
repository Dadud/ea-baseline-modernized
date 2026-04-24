/*
** MISSING_FROM_EA_BASELINE stub.
** WWLib/Notify.h - Static Observer registration pattern.
** Status: MISSING_FROM_EA_BASELINE
*/

#ifndef WWLIB_NOTIFY_H
#define WWLIB_NOTIFY_H

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
** Minimal Observer<T> / Notifier<T> registration system (stub).
** Real Notifier<T>/Observer<T> pattern from wwlib/Notify.h.
** Stub: registration is a no-op; HandleNotification is a no-op.
*/
template<typename T>
class Notifier;  // Forward declaration

template<typename T>
class Observer
{
public:
	Observer() {}
	virtual ~Observer() {}

	//! Handle event notification (stub: no-op)
	virtual void HandleNotification(T&) {}

	//! Request notification of this event (stub: no-op)
	virtual void NotifyMe(Notifier<T>& /*notifier*/) {}

	//! Stop observing all events (stub: no-op)
	void StopObserving() {}
};

//! Notifier<T> template (stub) - event source that holds observers
template<typename T>
class Notifier
{
public:
	Notifier() {}
	virtual ~Notifier() {}

	//! Add an observer to receive notifications (stub: no-op)
	void AddObserver(Observer<T>& /*observer*/) {}

	//! Remove an observer (stub: no-op)
	void RemoveObserver(Observer<T>& /*observer*/) {}
};

/*
** Minimal Signaler<T> base for event broadcasting.
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

//! Declare notifier interface for an event type (stub)
#define DECLARE_NOTIFIER(Event) \
	virtual void NotifyObservers(Event& /*event*/) {} \
	virtual void AddObserver(Observer<Event>& /*observer*/) {} \
	virtual void RemoveObserver(Observer<Event>& /*observer*/) {}
