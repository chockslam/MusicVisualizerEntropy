#pragma once
#include <chrono>



/// <summary>
/// Class Header copied from https://github.com/planetchili/hw3d/blob/master/hw3d/ChiliTimer.h.
/// Thank you, Chili :* 
/// Sorry for changing the name of the class... Love you anyways))
/// </summary>
class Timer
{
public:
	Timer();
	float Mark();
	float Peek() const;
private:
	std::chrono::steady_clock::time_point last;
};