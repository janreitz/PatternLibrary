#include <optional>
#include <atomic>

template<typename T> class SharedPtr;

template<typename T>
class RefCounter
{
public:
	void addRef();
	void releaseRef();

	void addWeakRef();
	void releaseWeakRef();

	bool expired() const;
	std::optional<SharedPtr<T>>lock();

	T* get();
	T const* get() const;

private:
	RefCounter(T* target);

	std::atomic<int> refCount;
	std::atomic<int> weakRefCount;

	T* m_target;

	template<typename T> friend class SharedPtr;
	template<typename T, typename... Args>
	friend SharedPtr<T> make_shared(Args&&...);
};


template<typename T>
class SharedPtr
{
public:
	SharedPtr(const SharedPtr& other);
	SharedPtr(SharedPtr&& other);

	SharedPtr& operator=(const SharedPtr& other);
	SharedPtr& operator=(SharedPtr&& other);
	bool operator==(const SharedPtr& other) const;
	bool operator!=(const SharedPtr& other) const;
	bool operator<(const SharedPtr& other) const;

	operator bool() const;

	template<typename... Args>
	static SharedPtr make(Args&&...args)
	{
		auto target = new T(std::forward<Args>(args)...);
		auto refCounter = new RefCounter(target);
		return SharedPtr(refCounter);
	};

	bool expired() const;

	T* operator->();
	T const* operator->() const;

private:
	SharedPtr(RefCounter<T>* refCounter);

	void safeAddRef() const;
	void safeReleaseRef() const;
	
	RefCounter<T>* m_refCounter;

	template<typename T> friend class WeakPtr;
	template<typename T, typename... Args>
	friend SharedPtr<T> make_shared(Args&&...);
};

template<typename T>
class WeakPtr
{
public:
	WeakPtr(const WeakPtr& other);
	WeakPtr(WeakPtr&& other);

	WeakPtr& operator=(const WeakPtr& other);
	WeakPtr& operator=(const SharedPtr<T>& other);
	WeakPtr& operator=(WeakPtr&& other);
	bool operator==(const WeakPtr& other) const;
	bool operator!=(const WeakPtr& other) const;
	bool operator<(const WeakPtr& other) const;
	
	operator bool() const;

	bool expired() const;
	std::optional<SharedPtr<T>> lock();

private:
	void safeAddWeakRef() const;
	void safeReleaseWeakRef() const;

	WeakPtr(RefCounter<T>* refCounter);
	RefCounter<T>* m_refCounter;
};

// als nicht member wäre das die korrekte Signatur (effective modern c++, p.171)
template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args);

#include "SmartPointer.hpp"

