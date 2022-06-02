#include "Timer.h"

/// <summary>
/// Implementation copied from https://github.com/planetchili/hw3d/blob/master/hw3d/ChiliTimer.h.
/// Thank you, Chili :* 
/// Sorry for changing the name of the class... Love you anyways))
/// </summary>
using namespace std::chrono;

Timer::Timer()
{
	last = steady_clock::now();
}

float Timer::Mark()
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float Timer::Peek() const
{
	return duration<float>(steady_clock::now() - last).count();
}
