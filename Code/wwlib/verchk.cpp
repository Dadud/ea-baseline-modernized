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
 *                     $Archive:: /Commando/Code/wwlib/verchk.cpp                             $*
 *                                                                                             *
 *                       Author:: Patrick Smith                                                *
 *                                                                                             *
 *                     $Modtime:: 2/16/02 10:53p                                              $*
 *                                                                                             *
 *                    $Revision:: 6                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */


#include "verchk.h"
#if defined(_WIN32)
#include <windows.h>
#include <winnt.h>
#else
#include <errno.h>
#include <limits.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#endif
#include "rawfile.h"
#include "ffactory.h"


/******************************************************************************
*
* NAME
*     GetVersionInfo
*
* DESCRIPTION
*     Retrieve version information from files version resource.
*
* INPUTS
*     Filename - Name of file to retrieve version information for.
*     FileInfo - Pointer to VS_FIXEDFILEINFO structure to be filled in.
*
* RESULT
*     Success - True if successful in obtaining version information.
*
******************************************************************************/
#if !defined(_WIN32)
static unsigned long long Unix_Time_To_FileTime_100ns(time_t unix_time)
{
	return (static_cast<unsigned long long>(unix_time) * 10000000ULL) + 116444736000000000ULL;
}

static bool Get_Current_Executable_Path(char *buffer, size_t size)
{
	if (buffer == NULL || size == 0) {
		return false;
	}

	ssize_t length = readlink("/proc/self/exe", buffer, size - 1);
	if (length < 0 || static_cast<size_t>(length) >= size) {
		buffer[0] = 0;
		return false;
	}

	buffer[length] = 0;
	return true;
}
#endif

bool GetVersionInfo(char* filename, VS_FIXEDFILEINFO* fileInfo) {
	if (fileInfo == NULL) {
		return false;
	}

#if defined(_WIN32)
	//
	// Get the version information for this file
	//
	bool verok = true;
	unsigned long dummy_var = 0;
	unsigned long version_size = ::GetFileVersionInfoSize (filename, &dummy_var);
	if (version_size > 0) {
		//
		// Get the file version block
		//
		LPBYTE pblock = new BYTE[version_size];
		if (::GetFileVersionInfo (filename, 0L, version_size, pblock)) {
			//
			// Query the block for the file version information
			//
			UINT version_len = 0;
			VS_FIXEDFILEINFO *pversion_info = NULL;
			if (::VerQueryValue (pblock, "\\", (LPVOID *)&pversion_info, &version_len)) {
				memcpy(fileInfo, pversion_info, sizeof(VS_FIXEDFILEINFO));
			} else {
				verok = false;
			}
		}
		delete [] pblock;
		pblock = NULL;
	}
	return verok;
#else
	memset(fileInfo, 0, sizeof(VS_FIXEDFILEINFO));
	return false;
#endif
}


bool GetFileCreationTime(char* filename, FILETIME* createTime)
	{
	if (filename && createTime)
		{
		createTime->dwLowDateTime = 0;
		createTime->dwHighDateTime = 0;
#if defined(_WIN32)
		FileClass* file = _TheFileFactory->Get_File(filename);

		if (file && file->Open())
			{
			HANDLE handle = file->Get_File_Handle();

			if (handle != INVALID_HANDLE_VALUE)
				{
				if (GetFileTime(handle, NULL, NULL, createTime))
					{
					return true;
					}
				}
			}
#else
		struct stat file_status;
		if (stat(filename, &file_status) == 0)
			{
#if defined(__APPLE__) || defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__)
			time_t timestamp = file_status.st_birthtime;
#else
			time_t timestamp = file_status.st_mtime;
#endif
			unsigned long long filetime = Unix_Time_To_FileTime_100ns(timestamp);
			createTime->dwLowDateTime = static_cast<DWORD>(filetime & 0xFFFFFFFFULL);
			createTime->dwHighDateTime = static_cast<DWORD>(filetime >> 32);
			return true;
			}
#endif
		}

	return false;
	}


////////////////////////////////////////////////////////////////////////
//
//	Get_Image_File_Header
//
////////////////////////////////////////////////////////////////////////
bool
Get_Image_File_Header (const char *filename, IMAGE_FILE_HEADER *file_header)
{
	bool retval = false;

	//
	//	Attempt to open the file
	//
	FileClass *file=_TheFileFactory->Get_File(filename);

	if (file && file->Open ()) {
		
		//
		//	Read the dos header (all PE exectuable files begin with this)
		//
		IMAGE_DOS_HEADER dos_header;
		if (file->Read (&dos_header, sizeof (dos_header)) == sizeof (dos_header)) {
			
			//
			//	Determine the index where the image header resides
			//
			int file_header_offset = dos_header.e_lfanew + sizeof (DWORD);
			file->Seek (file_header_offset, SEEK_SET);
			
			//
			//	Read the image header from the file
			//
			int size = sizeof (IMAGE_FILE_HEADER);
			if (file->Read (file_header, size) == size) {
				retval = true;
			}
		}
	}

	_TheFileFactory->Return_File(file);
	file=NULL;

	return retval;
}


////////////////////////////////////////////////////////////////////////
//
//	Get_Image_File_Header
//
////////////////////////////////////////////////////////////////////////
bool
Get_Image_File_Header (HINSTANCE app_instance, IMAGE_FILE_HEADER *file_header)
{
	bool retval = false;

#if defined(_WIN32)
	//
	//	Read the dos header (all PE exectuable files begin with this)
	//
	IMAGE_DOS_HEADER *dos_header = (IMAGE_DOS_HEADER *)app_instance;
	if (dos_header != NULL) {
		
		//
		//	Determine the offset where the image header resides
		//
		int image_header_offset = dos_header->e_lfanew + sizeof (DWORD);

		//
		//	Copy the file header into the provided structure
		//
		::memcpy (	file_header,
						(((char *)dos_header) + image_header_offset),
						sizeof (IMAGE_FILE_HEADER));		
		retval = true;
	}
#else
	char current_executable[PATH_MAX];
	if (Get_Current_Executable_Path(current_executable, sizeof(current_executable))) {
		retval = Get_Image_File_Header(current_executable, file_header);
	}
#endif
	

	return retval;
}


////////////////////////////////////////////////////////////////////////
//
//	Compare_EXE_Version
//
//	Used to compare 2 versions of an executable, the currently executing
// exe and a version saved to disk.  These exe's do not need to have
// a version resource.
//
//	The return value is the same as strcmp, < 0 if the current process is
// older, 0 if they are the same, and > 0 if the current process is newer.
//
////////////////////////////////////////////////////////////////////////
int
Compare_EXE_Version (int app_instance, const char *filename)
{
	int retval = 0;

	//
	//	Get the image header for both executables
	//
	IMAGE_FILE_HEADER header1 = { 0 };
	IMAGE_FILE_HEADER header2 = { 0 };	
	if	(	::Get_Image_File_Header ((HINSTANCE)app_instance, &header1) && 
			::Get_Image_File_Header (filename, &header2))
	{
		retval = int(header1.TimeDateStamp - header2.TimeDateStamp);
	}

	return retval;
}
