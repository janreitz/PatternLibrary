#pragma once
#include "Observer.h"
#include "Subject.h"

class DBLogger : public Observer
{
public:
	DBLogger(Subject* sub)
		: Observer(sub)
	{
	}

	void notify() override
	{
		const auto newState = m_subject->getState();
		std::cout << "\nLogging '" << newState << "' to DB";
	}
};
