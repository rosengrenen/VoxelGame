#include "pch.h"

int main()
{
	LOG_LEVEL(Warn);
	constexpr double PI = 3.1415;
    LOG_INFO("Hello, {}!", "World"); // Does not show, because info is of lower importance than warn
	LOG_WARN("The value of PI is {}!!", PI); // Does show
    return 0;
}
