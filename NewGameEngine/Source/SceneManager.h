#pragma once
#include <string>
#include "Graphics/Graphics.h" 
#include "Graphics/Particle_Systems/ParticleSystem.h" 
class SceneManager
{
public:
	SceneManager();
	void StartScene(Graphics& gfx, std::string scene);
	void Update(float musParams[3], float timeFrame);
	void EmitParticles(float musParams[3], ParticleSystem& ps, std::string feature = "All");
private:
	float kdTimeForParticles;
	int partCount;
	void AddParticleSystem(Graphics& gfx);
	void makeGeometrySphere(Graphics& gfx);
	void FillSpheresAlgorithm(Graphics& gfx, DirectX::XMFLOAT3 offset, int size, std::string shader_1, std::string shader_2, std::string gs = nullptr); // Original algroithm to create a square of speheres. 
	std::shared_ptr<ParticleSystem> testPS1;
	std::shared_ptr<ParticleSystem> testPS2;
	std::shared_ptr<ParticleSystem> testPS3;
};
