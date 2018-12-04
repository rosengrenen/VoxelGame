#include "pch.h"

#include "Timer.h"

Timer::Timer()
{
	using namespace std::chrono;
	m_timestamp = high_resolution_clock::now();
}

double Timer::Elapsed() const
{
	using namespace std::chrono;
	return duration_cast<duration<double>>(high_resolution_clock::now() - m_timestamp).count();
}

double Timer::Reset()
{
	using namespace std::chrono;
	auto currentTimestamp = high_resolution_clock::now();
	double dur = duration_cast<duration<double>>(currentTimestamp - m_timestamp).count();
	m_timestamp = currentTimestamp;
	return dur;
}
