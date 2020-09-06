#include "Observer.h"
#include "Subject.h"

Observer::Observer(Subject* sub)
	: m_subject(sub)
{
	m_subject->attach(this);
}