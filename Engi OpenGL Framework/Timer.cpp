#include "Timer.h"

using namespace std::chrono;

Timer::Timer()
{
	watch = high_resolution_clock::now();
}


Timer::~Timer()
{}

void Timer::Reset()
{
	watch = high_resolution_clock::now();
}

const unsigned long long Timer::Elapsed_s() const
{
	return duration_cast<seconds>(high_resolution_clock::now() - watch).count();
}

const unsigned long long Timer::Elapsed_ms() const
{
	return duration_cast<milliseconds>(high_resolution_clock::now() - watch).count();
}

const unsigned long long Timer::Elapsed_us() const
{
	return duration_cast<microseconds>(high_resolution_clock::now() - watch).count();
}

const unsigned long long Timer::Elapsed_ns() const
{
	return duration_cast<nanoseconds>(high_resolution_clock::now() - watch).count();
}