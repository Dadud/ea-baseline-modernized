#ifndef _WINDEF_H
#define _WINDEF_H
#include <stdint.h>
#include <stdbool.h>
#ifndef WINVER
#define WINVER 0x0500
#endif
#ifndef _WIN32_WINDOWS
#define _WIN32_WINDOWS 0x0500
#endif
#include <winnt.h>
typedef void* HANDLE;
typedef void* PVOID;
typedef void* LPVOID;
typedef HANDLE HGLOBAL;
typedef HANDLE HLOCAL;
typedef HANDLE HDWP;
#define DECLARE_HANDLE(name) typedef HANDLE name
DECLARE_HANDLE(HWND);
DECLARE_HANDLE(HMODULE);
DECLARE_HANDLE(HINSTANCE);
DECLARE_HANDLE(HTASK);
DECLARE_HANDLE(HKEY);
DECLARE_HANDLE(HDC);
DECLARE_HANDLE(HGLRC);
DECLARE_HANDLE(HFONT);
DECLARE_HANDLE(HBITMAP);
DECLARE_HANDLE(HBRUSH);
DECLARE_HANDLE(HPEN);
DECLARE_HANDLE(HMETAFILE);
DECLARE_HANDLE(HRGN);
DECLARE_HANDLE(HMENU);
DECLARE_HANDLE(HICON);
DECLARE_HANDLE(HCURSOR);
DECLARE_HANDLE(HMONITOR);
typedef unsigned int DWORD;
typedef unsigned int UINT;
typedef int INT;
typedef long LONG;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef long LPARAM;
typedef unsigned int WPARAM;
typedef long LRESULT;
typedef HANDLE HMODULE;
#endif
