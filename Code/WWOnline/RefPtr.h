/*
** MISSING_FROM_EA_BASELINE stub.
** WWOnline/RefPtr.h - Reference-counted smart pointer.
** Referenced by: DlgMPTeamSelect.h, AutoStart.h, wolgmode.h, natter.h
** Status: MISSING_FROM_EA_BASELINE
*/

#ifndef WWONLINE_REFPTR_H
#define WWONLINE_REFPTR_H

/*
** Minimal reference-counted smart pointer stub.
** Full RefPtr/RefCounted semantics from EA's WWOnline not implemented.
*/
template<typename T>
class RefPtr
{
public:
	RefPtr() : ptr_(nullptr) {}
	RefPtr(T* p) : ptr_(p) { if (ptr_) ptr_->AddRef(); }
	RefPtr(const RefPtr& other) : ptr_(other.ptr_) { if (ptr_) ptr_->AddRef(); }
	~RefPtr() { if (ptr_) ptr_->Release(); }

	RefPtr& operator=(const RefPtr& other) {
		if (this != &other) {
			if (ptr_) ptr_->Release();
			ptr_ = other.ptr_;
			if (ptr_) ptr_->AddRef();
		}
		return *this;
	}

	T* operator->() const { return ptr_; }
	T& operator*() const { return *ptr_; }
	operator T*() const { return ptr_; }
	T* get() const { return ptr_; }
	bool operator!() const { return ptr_ == nullptr; }
	explicit operator bool() const { return ptr_ != nullptr; }

private:
	T* ptr_;
};

template<typename T>
class RefPtrConst
{
public:
	RefPtrConst() : ptr_(nullptr) {}
	RefPtrConst(const RefPtr<T>& other) : ptr_(other.get()) {}
	RefPtrConst(T* p) : ptr_(p) {}

	const T* operator->() const { return ptr_; }
	const T& operator*() const { return *ptr_; }
	operator T*() const { return ptr_; }
	T* get() const { return ptr_; }

private:
	T* ptr_;
};

/*
** IUnknown-style AddRef/Release for RefPtr.
** Stub: actual reference counting not implemented.
*/
class RefCounted
{
public:
	RefCounted() : refcount_(0) {}
	virtual ~RefCounted() {}

	void AddRef() { refcount_++; }
	void Release() { if (--refcount_ == 0) delete this; }

protected:
	int refcount_;
};

#endif // WWONLINE_REFPTR_H
