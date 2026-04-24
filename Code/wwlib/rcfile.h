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
 *                 Project Name : wwlib                                                        *
 *                                                                                             *
 *                     $Archive:: /Commando/Code/wwlib/rcfile.h                               $*
 *                                                                                             *
 *                       Author:: Greg Hjelstrom                                               *
 *                                                                                             *
 *                     $Modtime:: 11/02/01 1:21p                                              $*
 *                                                                                             *
 *                    $Revision:: 8                                                           $*
 *                                                                                             *
 *---------------------------------------------------------------------------------------------*
 * Functions:                                                                                  *
 * - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */

#if defined(_MSC_VER)
#pragma once
#endif

#ifndef RCFILE_H
#define RCFILE_H

#include "always.h"
#include "wwfile.h"
#include "win.h"

#ifdef OPENW3D_SDL3
#include <cstdint>
#include <map>
#include <string>
#endif

/*
** ResourceFileClass
** This is a file class which allows you to read from a binary file that you have
** imported into your resources.
** - Win32: uses native PE resources of type "File".
** - SDL3 path: uses a static in-memory resource registry that later batches can populate.
*/
class ResourceFileClass : public FileClass
{
	public:

#ifdef OPENW3D_SDL3
		ResourceFileClass(char const *filename);
#endif
#ifdef _WIN32
		ResourceFileClass(HMODULE hmodule, char const *filename);
#endif
		virtual ~ResourceFileClass(void);
		
		virtual char const * File_Name(void) const					{ return ResourceName; }
		virtual char const * Set_Name(char const *filename);
		virtual int Create(void)											{ return false; }
		virtual int Delete(void)											{ return false; }
		virtual bool Is_Available(int /*forced=false*/)				{ return Is_Open (); }
		virtual bool Is_Open(void) const									{ return (FileBytes != NULL); } 
		
		virtual int Open(char const * /*fname*/, int /*rights=READ*/)	{ return Is_Open(); }
		virtual int Open(int /*rights=READ*/)							{ return Is_Open(); }

		virtual int Read(void *buffer, int size);
		virtual int Seek(int pos, int dir=SEEK_CUR);
		virtual int Size(void);
		virtual int Write(void const * /*buffer*/, int /*size*/)	{ return 0; }
		virtual void Close(void)											{ }
		virtual void Error(int error, int canretry = false, char const * filename=NULL);
		virtual void Bias(int start, int length=-1) {}

		virtual const unsigned char *Peek_Data(void) const					{ return FileBytes; }

	protected:

		char *				ResourceName;

#ifdef _WIN32
		HMODULE				hModule;
#endif
		const unsigned char *	FileBytes;
		const unsigned char *	FilePtr;
		const unsigned char *	EndOfFile;

};

#ifdef OPENW3D_SDL3
struct StaticResourceFileClass {
	const char *filename;
	const std::uint8_t *data;
	size_t size;
};

inline std::map<std::string, StaticResourceFileClass>& GetStaticResources() {
	static std::map<std::string, StaticResourceFileClass> map;
	return map;
}
#endif

#endif
