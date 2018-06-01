//
// EPITECH PROJECT, 2018
// 
// File description:
// 
//

#ifndef _TIMER_HH_
#define _TIMER_HH_

#include <chrono>

template<typename resolution = std::chrono::milliseconds>

class Timer
{
public:
	using Clock = std::conditional_t<std::chrono::high_resolution_clock::is_steady,
					 std::chrono::high_resolution_clock, std::chrono::steady_clock>;
private:
	Clock::time_point mStart = Clock::now();
	Clock::time_point mStop;
public:
	Timer() = default;
	~Timer() = default;

	void stop() { mStop = Clock::now(); }
	void reset() { mStart = Clock::now(); }
	int elapsed() { return  std::chrono::duration_cast<resolution>(Clock::now() - mStart).count(); }
};

#endif
