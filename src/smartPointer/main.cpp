#include "SmartPointer.h"

int main() {
	auto intPtr = make_shared<int>();

	auto staticIntPtr = SharedPtr<int>::make();

	return 0;
}