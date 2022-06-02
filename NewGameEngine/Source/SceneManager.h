#pragma once
#include <string>
#include "Graphics/Graphics.h" 
class SceneManager
{
public:
	SceneManager();
	void StartScene(Graphics& gfx, std::string scene);
	void Update(float timeFrame);
private:
	float kdTimeForParticles;
	int partCount;
	void AddParticleSystem(Graphics& gfx);
	void makeGeometrySphere(Graphics& gfx);
	void FillSpheresAlgorithm(Graphics& gfx, float offset[3], int size, std::string shader_1, std::string shader_2, std::string gs = nullptr); // Original algroithm to create a square of speheres. 
	
};
