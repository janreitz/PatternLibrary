#include "Subject.h"
#include "DBLogger.h"
#include "FileLogger.h"

int main()
{
	Subject sub;
	DBLogger dbLogger(&sub);
	FileLogger fileLogger(&sub);

	sub.setState(10);

	return 0;
}