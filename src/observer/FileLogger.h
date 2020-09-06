#pragma once
#include "Observer.h"
#include "Subject.h"

class FileLogger : public Observer
{
public:
	FileLogger(Subject* sub)
		: Observer(sub)
	{
	}

	void notify() override
	{
		const auto newState = m_subject->getState();
		std::cout << "\nLogging '" << newState << "' to File";
	}
};
