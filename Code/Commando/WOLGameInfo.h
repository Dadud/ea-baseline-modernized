/*
** MISSING_FROM_EA_BASELINE stub.
** WOLGameInfo.h - WOL game information class.
** Referenced by: DlgMPTeamSelect.h
** Status: MISSING_FROM_EA_BASELINE - WOLBrowser is a COM/IDL ActiveX control, Windows-only
**
** This stub provides minimal forward declarations to unblock non-Windows compilation.
** WOLGameInfo and WOLBrowser are part of EA's Windows online UI layer and
** are not relevant to FDS or headless server builds.
*/

#ifndef WOLGAMEINFO_H
#define WOLGAMEINFO_H

/*
** WOLGameInfo class stub - no WOLBrowser dependency
** Full WOL game info and WOLBrowser ActiveX control not available on non-Windows.
*/

namespace WWOnline {
	class ChannelData;
	class UserData;
};

class WOLGameInfo
{
public:
	WOLGameInfo() {}
	~WOLGameInfo() {}

	static bool IsValidGameChannel(WWOnline::ChannelData* channel);
	WOLGameInfo(WWOnline::ChannelData* channel);
	void ImportFromChannel(WWOnline::ChannelData* channel);
	void ExportToChannel(WWOnline::ChannelData* channel);
	bool CanUserJoin(WWOnline::UserData* user);

	// TODO: reconstruct full WOLGameInfo interface
};

#endif // WOLGAMEINFO_H
