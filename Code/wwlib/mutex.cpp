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

#include "mutex.h"
#include "wwdebug.h"
#if defined(_WIN32)
#include <windows.h>
#elif defined(OPENW3D_SDL3)
#include <SDL3/SDL_mutex.h>
#include <SDL3/SDL_timer.h>
#endif


// ----------------------------------------------------------------------------

MutexClass::MutexClass(const char* name) : handle(NULL), locked(false)
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			(void)name;
			handle = SDL_CreateMutex();
			WWASSERT(handle);
		#else
			//assert(0);
		#endif
	#else
		handle=CreateMutex(NULL,false,name);
		WWASSERT(handle);
	#endif
}

MutexClass::~MutexClass()
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			WWASSERT(!locked);
			if (handle) {
				SDL_DestroyMutex(reinterpret_cast<SDL_Mutex*>(handle));
				handle = NULL;
			}
		#else
			//assert(0);
		#endif
	#else
		WWASSERT(!locked); // Can't delete locked mutex!
		CloseHandle(handle);
	#endif
}

bool MutexClass::Lock(int time)
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			SDL_Mutex *mutex = reinterpret_cast<SDL_Mutex*>(handle);
			if (mutex == NULL) {
				return false;
			}
			if (time == WAIT_INFINITE) {
				SDL_LockMutex(mutex);
				locked++;
				return true;
			}
			if (time <= 0) {
				if (!SDL_TryLockMutex(mutex)) {
					return false;
				}
				locked++;
				return true;
			}
			Uint64 deadline = SDL_GetTicks() + static_cast<Uint64>(time);
			do {
				if (SDL_TryLockMutex(mutex)) {
					locked++;
					return true;
				}
				SDL_Delay(1);
			} while (SDL_GetTicks() < deadline);
			return false;
		#else
			//assert(0);
			return true;
		#endif
	#else
		int res = WaitForSingleObject(handle,time==WAIT_INFINITE ? INFINITE : time);
		if (res!=WAIT_OBJECT_0) return false;
		locked++;
		return true;
	#endif
}

void MutexClass::Unlock()
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			WWASSERT(locked);
			locked--;
			SDL_UnlockMutex(reinterpret_cast<SDL_Mutex*>(handle));
		#else
			//assert(0);
		#endif
	#else
		WWASSERT(locked);
		locked--;
		int res=ReleaseMutex(handle);
		WWASSERT(res);
	#endif
}

// ----------------------------------------------------------------------------

MutexClass::LockClass::LockClass(MutexClass& mutex_,int time) : mutex(mutex_)
{
	failed=!mutex.Lock(time);
}

MutexClass::LockClass::~LockClass()
{
	if (!failed) mutex.Unlock();
}







// ----------------------------------------------------------------------------

CriticalSectionClass::CriticalSectionClass() : handle(NULL), locked(false)
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			handle = SDL_CreateMutex();
			WWASSERT(handle);
		#else
			//assert(0);
		#endif
	#else
		handle=new char[sizeof(CRITICAL_SECTION)];
		InitializeCriticalSection((CRITICAL_SECTION*)handle);
	#endif
}

CriticalSectionClass::~CriticalSectionClass()
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			WWASSERT(!locked); // Can't delete locked mutex!
			if (handle) {
				SDL_DestroyMutex(reinterpret_cast<SDL_Mutex*>(handle));
				handle = NULL;
			}
		#else
			//assert(0);
		#endif
	#else
		WWASSERT(!locked); // Can't delete locked mutex!
		DeleteCriticalSection((CRITICAL_SECTION*)handle);
		delete[] handle;
	#endif
}

void CriticalSectionClass::Lock()
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			SDL_LockMutex(reinterpret_cast<SDL_Mutex*>(handle));
			locked++;
		#else
			//assert(0);
		#endif
	#else
		EnterCriticalSection((CRITICAL_SECTION*)handle);
		locked++;
	#endif
}

void CriticalSectionClass::Unlock()
{
	#ifdef _UNIX
		#if defined(OPENW3D_SDL3)
			WWASSERT(locked);
			locked--;
			SDL_UnlockMutex(reinterpret_cast<SDL_Mutex*>(handle));
		#else
			//assert(0);
		#endif
	#else
		WWASSERT(locked);
		locked--;
		LeaveCriticalSection((CRITICAL_SECTION*)handle);
	#endif
}

// ----------------------------------------------------------------------------

CriticalSectionClass::LockClass::LockClass(CriticalSectionClass& critical_section) : CriticalSection(critical_section)
{
	CriticalSection.Lock();
}

CriticalSectionClass::LockClass::~LockClass()
{
	CriticalSection.Unlock();
}


