#ifndef _RPCNDR_H
#define _RPCNDR_H
typedef unsigned char byte;
typedef short wchar_t;
typedef long HRESULT;
typedef unsigned long ULONG;
typedef void* PVOID;
typedef const void* LPCVOID;
#define STDMETHODCALLTYPE
#define STDAPIVCALLTYPE
#define interface struct
#define _stdcall
#define WINAPI __stdcall
#define APIENTRY WINAPI
#define THIS void
#define THIS_
#define PURE = 0
#define STDMETHOD(type) virtual type STDMETHODCALLTYPE
#define STDMETHOD_(type1, type2) virtual type1 STDMETHODCALLTYPE
#ifndef DECLARE_INTERFACE_
#define DECLARE_INTERFACE_(iface, base) interface iface : public base
#endif
#ifndef DECLARE_INTERFACE
#define DECLARE_INTERFACE(iface) DECLARE_INTERFACE_(iface, IUnknown)
#endif
typedef struct _GUID { unsigned long Data1; unsigned short Data2; unsigned short Data3; unsigned char Data4[8]; } GUID;
typedef GUID IID;
typedef IID CLSID;
typedef CLSID *LPCLSID;
typedef const GUID *LPCGUID;
typedef GUID *PGUID;
typedef IID *LPIID;
#define REFGUID const GUID &
#define REFIID const IID &
#define REFCLSID const IID &
#define IsEqualGUID(rguid1, rguid2) (!memcmp(rguid1, rguid2, sizeof(GUID)))
#ifndef DEFINE_GUID
#define DEFINE_GUID(name, l, w1, w2, b1, b2, b3, b4, b5, b6, b7, b8) static const GUID name = { l, w1, w2, { b1, b2, b3, b4, b5, b6, b7, b8 } }
#endif
#endif
