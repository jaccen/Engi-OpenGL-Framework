#pragma once

#include <chrono>

class Timer
{
private:
	std::chrono::high_resolution_clock::time_point watch;
public:
	Timer();
	~Timer();
	void Reset();
	const unsigned long long Elapsed_s() const;
	const unsigned long long Elapsed_ms() const;
	const unsigned long long Elapsed_us() const;
	const unsigned long long Elapsed_ns() const;
};