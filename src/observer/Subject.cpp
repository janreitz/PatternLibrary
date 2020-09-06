#include "Subject.h"

void Subject::attach(Observer* o)
{
	m_observers.push_back(o);
}

void Subject::notifyObservers()
{
	for (const auto o : m_observers)
	{
		o->notify();
	}
}

int Subject::getState() const
{
	return m_state;
}

void Subject::setState(int i)
{
	m_state = i;
	notifyObservers();
}
