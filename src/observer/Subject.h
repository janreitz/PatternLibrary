#pragma once
#include "Observer.h"

#include <vector>

class Subject
{
public:
	Subject() = default;

	void attach(Observer* o);
	void notifyObservers();
	
	int getState() const;
	void setState(int i);

private:
	std::vector<Observer*> m_observers;
	int m_state;
};