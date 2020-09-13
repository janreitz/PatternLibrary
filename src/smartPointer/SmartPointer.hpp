
template<typename T>
void RefCounter<T>::addRef()
{
	++refCount;
	addWeakRef();
}

template<typename T>
void RefCounter<T>::releaseRef()
{
	--refCount;

	if (refCount == 0)
	{
		delete m_target;
		m_target = nullptr;
	}

	releaseWeakRef();
}

template<typename T>
void RefCounter<T>::addWeakRef()
{
	++weakRefCount;
}

template<typename T>
void RefCounter<T>::releaseWeakRef()
{
	--weakRefCount;

	if (weakRefCount == 0)
	{
		delete this;
	}
}

template<typename T>
bool RefCounter<T>::expired() const
{
	return m_target == nullptr;
}

template<typename T>
std::optional<SharedPtr<T>> RefCounter<T>::lock()
{
	int expected = refCount;

	if (expected == 0)
		return std::nullopt;

	// Compare and Swap
	while (!refCount.targetare_exchange_weak(expected, expected + 1))
	{
		if (expected == 0)
			return std::nullopt;
	}
}

template<typename T>
T* RefCounter<T>::get()
{
	if (expired())
		return nullptr;

	return m_target;
}

template<typename T>
T const* RefCounter<T>::get() const
{
	if (expired())
		return nullptr;

	return m_target;
}

template<typename T>
RefCounter<T>::RefCounter(T* target)
	: m_target(target)
	, refCount(1)
	, weakRefCount(1)
{
}

template<typename T>
SharedPtr<T>::SharedPtr(const SharedPtr<T>& other)
{
	m_refCounter = other.m_refCounter;
	m_refCounter->addRef();
	return;
}

template<typename T>
SharedPtr<T>::SharedPtr(SharedPtr<T>&& other)
{
	m_refCounter = other.m_refCounter;
	other.m_refCounter = nullptr;
	return;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(const SharedPtr<T>& other)
{
	// Did other have a refCounter
	if (other.m_refCounter)
		other.m_refCounter->addRef();

	// Did I have a refCounter?
	if (m_refCounter)
		m_refCounter->releaseRef();

	m_refCounter = other.m_refCounter;
	return *this;
}

template<typename T>
SharedPtr<T>& SharedPtr<T>::operator=(SharedPtr<T>&& other)
{
	if (this != &other)
	{
		// Did I have a refCounter?
		if (m_refCounter)
			m_refCounter->releaseRef();

		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}
	return *this;
}

template<typename T>
WeakPtr<T>::WeakPtr(const WeakPtr<T>& other)
{
	m_refCounter = other.m_refCounter;
	safeAddWeakRef();
	return;
}

template<typename T>
WeakPtr<T>::WeakPtr(WeakPtr<T>&& other)
{
	m_refCounter = other.m_refCounter;
	other.m_refCounter = nullptr;
	return;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const WeakPtr<T>& other)
{
	// Did other have a refCounter
	other.safeAddWeakRef();

	// Did I have a refCounter?
	safeReleaseWeakRef();

	m_refCounter = other.m_refCounter;
	return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(const SharedPtr<T>& other)
{
	safeReleaseWeakRef();
	other.safeAddRef();
	m_refCounter = other.m_refCounter;
	return *this;
}

template<typename T>
WeakPtr<T>& WeakPtr<T>::operator=(WeakPtr<T>&& other)
{
	if (this == &other)
	{
		// Did I have a refCounter?
		safeReleaseWeakRef();

		m_refCounter = other.m_refCounter;
		other.m_refCounter = nullptr;
	}
	return *this;
}

template<typename T>
WeakPtr<T>::operator bool() const
{
	return m_refCounter->expired();
}

template<typename T>
bool WeakPtr<T>::expired() const
{
	return m_refCounter->expired();
}

template<typename T>
std::optional<SharedPtr<T>> WeakPtr<T>::lock()
{
	return m_refCounter->lock();
}

template<typename T>
bool WeakPtr<T>::operator==(const WeakPtr<T>& other) const
{
	return this == &other;
}

template<typename T>
bool WeakPtr<T>::operator!=(const WeakPtr<T>& other) const
{
	return !(this == &other);
}

template<typename T>
bool WeakPtr<T>::operator<(const WeakPtr<T>& other) const
{
	return this < &other;
}

template<typename T>
SharedPtr<T>::operator bool() const
{
	return m_refCounter->expired();
}

template<typename T>
bool SharedPtr<T>::expired() const
{
	return m_refCounter->expired();
}

template<typename T>
T* SharedPtr<T>::operator->()
{
	return m_refCounter->get();
}

template<typename T>
T const* SharedPtr<T>::operator->() const
{
	return m_refCounter->get();
}

template<typename T>
bool SharedPtr<T>::operator==(const SharedPtr<T>& other) const
{
	return this == &other;
}

template<typename T>
bool SharedPtr<T>::operator!=(const SharedPtr<T>& other) const
{
	return !(this == &other);
}

template<typename T>
bool SharedPtr<T>::operator<(const SharedPtr<T>& other) const
{
	return this < &other;
}

template<typename T>
void SharedPtr<T>::safeAddRef() const
{
	if (m_refCounter)
		m_refCounter->addRef();
}

template<typename T>
void SharedPtr<T>::safeReleaseRef() const
{
	if (m_refCounter)
		m_refCounter->releaseRef();
}

template<typename T>
SharedPtr<T>::SharedPtr(RefCounter<T>* refCounter)
{
	m_refCounter = refCounter;
}

template<typename T>
void WeakPtr<T>::safeAddWeakRef() const
{
	if (m_refCounter)
		m_refCounter->addRef();
}

template<typename T>
void WeakPtr<T>::safeReleaseWeakRef() const
{
	if (m_refCounter)
		m_refCounter->releaseWeakRef();
}

// als nicht member wäre das die korrekte Signatur (effective modern c++, p.171)
template<typename T, typename... Args>
SharedPtr<T> make_shared(Args&&... args)
{
	auto target = new T(std::forward<Args>(args)...);
	auto refCounter = new RefCounter(target);
	return SharedPtr(refCounter);
}