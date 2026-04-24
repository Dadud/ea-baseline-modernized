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

#define _WIN32_WINNT 0x0400

#include "thread.h"
#include "Except.h"
#include "wwdebug.h"
#if defined(_WIN32)
#include <process.h>
#include <windows.h>
#endif
#if defined(OPENW3D_SDL3)
#include <SDL3/SDL_timer.h>
#endif
#pragma warning ( push )
#pragma warning ( disable : 4201 )
#include "systimer.h"
#pragma warning ( pop )

ThreadClass::ThreadClass(const char *thread_name, ExceptionHandlerType exception_handler) : handle(0), running(false), thread_priority(0)
{
	if (thread_name) {
		assert(strlen(thread_name) < sizeof(ThreadName) - 1);
		strcpy(ThreadName, thread_name);
	} else {
		strcpy(ThreadName, "No name");;
	}

	ExceptionHandler = exception_handler;
}

ThreadClass::~ThreadClass()
{
	Stop();
}

#if defined(OPENW3D_SDL3)
int SDLCALL ThreadClass::Internal_Thread_Function(void* params)
#else
void __cdecl ThreadClass::Internal_Thread_Function(void* params)
#endif
{
	ThreadClass* tc=reinterpret_cast<ThreadClass*>(params);
	tc->running=true;
#if defined(_WIN32)
	tc->ThreadID = GetCurrentThreadId();
#elif defined(OPENW3D_SDL3)
	tc->ThreadID = SDL_GetCurrentThreadID();
#else
	tc->ThreadID = 0;
#endif

#if defined(_WIN32)
	Register_Thread_ID(tc->ThreadID, tc->ThreadName);

	if (tc->ExceptionHandler != NULL) {
		__try {
			tc->Thread_Function();
		} __except(tc->ExceptionHandler(GetExceptionCode(), GetExceptionInformation())) {};
	} else {
		tc->Thread_Function();
	}

#else //_WIN32
	tc->Thread_Function();
#endif //_WIN32

#if defined(_WIN32)
	Unregister_Thread_ID(tc->ThreadID, tc->ThreadName);
#endif // _WIN32
	tc->handle=0;
	tc->ThreadID = 0;
#if defined(OPENW3D_SDL3)
	return 0;
#endif
}

void ThreadClass::Execute()
{
	WWASSERT(!handle);	// Only one thread at a time!
	#if defined(OPENW3D_SDL3)
		handle = SDL_CreateThread(&Internal_Thread_Function, ThreadName, this);
		WWASSERT(handle);
		WWDEBUG_SAY(("ThreadClass::Execute: Started SDL3 thread %s, thread ID is %X\n", ThreadName, SDL_GetThreadID(handle)));
	#elif !defined(_WIN32)
		// assert(0);
		return;
	#else
		handle=_beginthread(&Internal_Thread_Function,0,this);
		SetThreadPriority((HANDLE)handle,THREAD_PRIORITY_NORMAL+thread_priority);
		WWDEBUG_SAY(("ThreadClass::Execute: Started thread %s, thread ID is %X\n", ThreadName, handle));
	#endif
}

void ThreadClass::Set_Priority(int priority)
{
	#if defined(OPENW3D_SDL3)
		thread_priority=priority;
	#elif !defined(_WIN32)
		// assert(0);
		return;
	#else
		thread_priority=priority;
		if (handle) SetThreadPriority((HANDLE)handle,THREAD_PRIORITY_NORMAL+thread_priority);
	#endif
}

void ThreadClass::Stop(unsigned ms)
{
	#if defined(OPENW3D_SDL3)
		running=false;
		if (handle) {
			SDL_WaitThread(handle, NULL);
			handle = 0;
		}
		(void)ms;
	#elif !defined(_WIN32)
		// assert(0);
		return;
	#else
		running=false;
		unsigned time=TIMEGETTIME();
		while (handle) {
			if ((TIMEGETTIME()-time)>ms) {
				int res=TerminateThread((HANDLE)handle,0);
				WWASSERT(res);	// Thread still not killed!
				handle=0;
			}
			Sleep(0);
		}
	#endif
}

void ThreadClass::Sleep_Ms(unsigned ms)
{
#if defined(_WIN32)
	Sleep(ms);
#elif defined(OPENW3D_SDL3)
	SDL_Delay(ms);
#else
	(void)ms;
#endif
}

#ifdef _WIN32
HANDLE test_event = ::CreateEvent (NULL, FALSE, FALSE, "");
#endif

void ThreadClass::Switch_Thread()
{
	#if defined(OPENW3D_SDL3)
		SDL_Delay(0);
	#elif !defined(_WIN32)
		return;
	#else
		//	::SwitchToThread ();
		::WaitForSingleObject (test_event, 1);
		//	Sleep(1);	// Note! Parameter can not be 0 (or the thread switch doesn't occur)
	#endif
}

// Return calling thread's unique thread id
unsigned ThreadClass::_Get_Current_Thread_ID()
{
	#if defined(OPENW3D_SDL3)
		return SDL_GetCurrentThreadID();
	#elif !defined(_WIN32)
		return 0;
	#else
		return GetCurrentThreadId();
	#endif
}

bool ThreadClass::Is_Running()
{
	return !!handle;
}
