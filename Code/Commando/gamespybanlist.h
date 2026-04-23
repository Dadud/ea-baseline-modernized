/*
**	Command & Conquer Renegade(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/*
** NOTE: This header is MISSING from the EA baseline release.
** It was referenced by player.h but not included in the published source.
** This stub provides minimal forward declarations needed to compile.
** The full GameSpyBanList class interface should be reconstructed from:
**   - GameSpyBanList.cpp (implementation reference)
**   - OpenW3D fork if it contains the original header
**   - EA internal source if available
*/

/*
** Minimal stub for gamespybanlist.h
** Known to be used by: player.h -> playermanager.h -> <throughout product shell>
** This stub exists only to unblock compilation of the commando product shell seam.
** Status: MISSING_FROM_EA_BASELINE - needs full header reconstruction
*/

#ifndef GAMESPYBANLIST_H
#define GAMESPYBANLIST_H

/*
** Forward declaration only — implementation lives in GameSpyBanList.cpp
** Do not add method definitions here without proper header reconstruction.
*/

class GameSpyBanListClass
{
public:
	GameSpyBanListClass(void) {};
	~GameSpyBanListClass(void) {};

	// TODO: reconstruct full interface from GameSpyBanList.cpp
	// Known methods referenced in GameSpyBanList.cpp need to be added here
};

#endif // GAMESPYBANLIST_H
