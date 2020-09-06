#include <vector>
#include <functional>
#include <string>
#include <iostream>

using namespace std;

// This is a callback based approach to the Observer pattern. 
// One Observer can attach to multiple subjects of different types.
// The Observer can encode contextual information in the capture.
// No base classes are needed.

class SubjectString
{
public:
	void attach(function<void(const string& str)> cb)
	{
		m_callbacks.push_back(cb);
	}

	void setState(const string& str)
	{
		m_state = str;
		for (const auto cb : m_callbacks)
		{
			cb(str);
		}
	}

private:
	vector<function<void(const string& str)>> m_callbacks;
	string m_state;
};

class SubjectInt
{
public:
	void attach(function<void(const int& str)> cb)
	{
		m_callbacks.push_back(cb);
	}

	void setState(const int& str)
	{
		m_state = str;
		for (const auto cb : m_callbacks)
		{
			cb(str);
		}
	}

private:
	vector<function<void(const int& str)>> m_callbacks;
	int m_state;
};

class Observer
{
public:
	void attachToString(SubjectString* sub)
	{
		const auto cb = [this](const string& str) {
			logString(str);
		};
		sub->attach(cb);
	}

	void attachToInt(SubjectInt* sub)
	{
		const auto cb = [this](const int& i) {
			logInt(i);
		};
		sub->attach(cb);
	}

private:
	void logString(const string& str)
	{
		cout << "\nLogging string '" << str << "'";
	}

	void logInt(int i)
	{
		cout << "\nLogging int '" << i << "'";
	}
};


int main()
{
	SubjectString subString;
	SubjectInt subInt;
	Observer o;
	
	o.attachToString(&subString);
	o.attachToInt(&subInt);

	subString.setState("Hi, this is a string");
	subInt.setState(42);

	return 0;
}