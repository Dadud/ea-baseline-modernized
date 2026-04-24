// Minimal ATL stub for MSVC builds without ATL installed.
// Provides: CComPtr, CComBSTR, CComModule, and core COM types needed by WWOnline.
// Real ATL applications should use a full Visual Studio installation with ATL.

#ifndef __ATLBASE_H__
#define __ATLBASE_H__

#include <windows.h>
#include <unknwn.h>
#include <oleauto.h>

// CComPtr<T> - minimal ATL smart pointer =======================================
template <class T>
class CComPtr {
public:
    T* p;
    CComPtr() : p(nullptr) {}
    CComPtr(T* lp) : p(lp) { if (p) p->AddRef(); }
    CComPtr(const CComPtr<T>& lp) : p(lp.p) { if (p) p->AddRef(); }
    ~CComPtr() { if (p) p->Release(); }

    T* operator=(T* lp) { if (lp) lp->AddRef(); if (p) p->Release(); p = lp; return *this; }
    T* operator=(const CComPtr<T>& lp) { return (*this = lp.p); }
    operator T*() const { return p; }
    T* operator->() const { return p; }
    T** operator&() { return &p; }
    bool operator!() const { return (p == nullptr); }
    bool operator==(T* lp) const { return p == lp; }
    void Release() { if (p) { p->Release(); p = nullptr; } }
    void Attach(T* lp) { if (p) p->Release(); p = lp; }
    T* Detach() { T* t = p; p = nullptr; return t; }
};

// CComBSTR - minimal ATL BSTR wrapper ==========================================
class CComBSTR {
public:
    BSTR bstr;
    CComBSTR() : bstr(nullptr) {}
    explicit CComBSTR(int nSize) { bstr = ::SysAllocStringLen(nullptr, nSize); }
    CComBSTR(LPCOLESTR sz) { if (sz) bstr = ::SysAllocString(sz); else bstr = nullptr; }
    CComBSTR(const CComBSTR& src) { bstr = src.bstr ? ::SysAllocStringLen(src.bstr, ::SysStringLen(src.bstr)) : nullptr; }
    ~CComBSTR() { ::SysFreeString(bstr); }
    CComBSTR& operator=(BSTR s) { ::SysFreeString(bstr); bstr = s; return *this; }
    unsigned int Length() const { return ::SysStringLen(bstr); }
    operator BSTR() const { return bstr; }
    BSTR* operator&() { return &bstr; }
};

// ATL module stub
class CComModule {};

// ATLASSERT stub
#ifdef _ATL_ASSERT
#undef _ATL_ASSERT
#endif
#define _ATL_ASSERT(expr) ((void)0)

// TypedEvent - typed event source pattern used by WWOnline =====================
// Self-referential template: derived class passes itself as first arg.
template<class T, class T_arg>
class TypedEvent {
public:
    typedef T_arg ArgType;
    T_arg m_arg;
    TypedEvent() : m_arg() {}
    TypedEvent(T_arg arg) : m_arg(arg) {}
    virtual ~TypedEvent() {}
};

#endif // __ATLBASE_H__
