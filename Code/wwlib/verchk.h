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

/***********************************************************************************************
 ***              C O N F I D E N T I A L  ---  W E S T W O O D  S T U D I O S               ***
 ***********************************************************************************************
 *                                                                                             *
 *                 Project Name : LevelEdit                                                    *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/wwlib/verchk.h                               $*
 *                                                                                             *
 *                       Author:: Patrick Smith                                                *
 *                                                                                             *
 *                     $Modtime:: 8/15/01 11:29a                                              $*
 *                                                                                             *
 *                    $Revision:: 3                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#if defined(_MSC_VER)
#pragma once
#endif

#ifndef __VERCHK_H
#define __VERCHK_H

#if defined(_WIN32)
#include <windows.h>
#else
#include "bittype.h"

typedef void * HINSTANCE;

typedef struct _FILETIME {
	DWORD dwLowDateTime;
	DWORD dwHighDateTime;
} FILETIME;

typedef struct tagVS_FIXEDFILEINFO {
	DWORD dwSignature;
	DWORD dwStrucVersion;
	DWORD dwFileVersionMS;
	DWORD dwFileVersionLS;
	DWORD dwProductVersionMS;
	DWORD dwProductVersionLS;
	DWORD dwFileFlagsMask;
	DWORD dwFileFlags;
	DWORD dwFileOS;
	DWORD dwFileType;
	DWORD dwFileSubtype;
	DWORD dwFileDateMS;
	DWORD dwFileDateLS;
} VS_FIXEDFILEINFO;

#pragma pack(push, 1)
typedef struct _IMAGE_DOS_HEADER {
	WORD   e_magic;
	WORD   e_cblp;
	WORD   e_cp;
	WORD   e_crlc;
	WORD   e_cparhdr;
	WORD   e_minalloc;
	WORD   e_maxalloc;
	WORD   e_ss;
	WORD   e_sp;
	WORD   e_csum;
	WORD   e_ip;
	WORD   e_cs;
	WORD   e_lfarlc;
	WORD   e_ovno;
	WORD   e_res[4];
	WORD   e_oemid;
	WORD   e_oeminfo;
	WORD   e_res2[10];
	long   e_lfanew;
} IMAGE_DOS_HEADER;

typedef struct _IMAGE_FILE_HEADER {
	WORD  Machine;
	WORD  NumberOfSections;
	DWORD TimeDateStamp;
	DWORD PointerToSymbolTable;
	DWORD NumberOfSymbols;
	WORD  SizeOfOptionalHeader;
	WORD  Characteristics;
} IMAGE_FILE_HEADER;
#pragma pack(pop)
#endif

// Obtain version information from the specified file.
bool GetVersionInfo(char* filename, VS_FIXEDFILEINFO* fileInfo);

// Retreive creation time of specified file.
bool GetFileCreationTime(char* filename, FILETIME* createTime);

////////////////////////////////////////////////////////////////////////
//
//	Compare_EXE_Version
//
//	Used to compare 2 versions of an executable, the currently executing
// exe and a version saved to disk.  These exe's do not need to have
// a version resource.
//
//	The return value is the same as strcmp, -1 if the current process is
// older, 0 if they are the same, and +1 if the current process is newer.
//
////////////////////////////////////////////////////////////////////////
int Compare_EXE_Version (int app_instance, const char *filename);


#endif //__VERCHK_H

