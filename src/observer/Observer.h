#pragma once
#include <vector>
#include <iostream>

class Subject;

// This classic Observer only works with a single subject
class Observer
{
public:
	Observer(Subject* sub);

	virtual void notify() = 0;
protected:
	Subject* m_subject;
};

