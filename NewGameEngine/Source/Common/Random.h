#pragma once
#include <random>
/// <summary>
/// Random class. Thanks to https://github.com/TheCherno/OneHourParticleSystem/blob/master/OpenGL-Sandbox/src/Random.h.
/// </summary>
class Random
{
public:
	static void Init();
	static float Float();
private:
	static std::mt19937 s_RandomEngine;
	static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
};

