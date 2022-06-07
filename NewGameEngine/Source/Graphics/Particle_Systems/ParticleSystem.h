#pragma once

#include "ParticleWrapper.h"


/// <summary>
/// Particle Properties.
/// Thanks to https://github.com/TheCherno/OneHourParticleSystem/blob/master/OpenGL-Sandbox/src/ParticleSystem.h.
/// Love you to death, TheCherno.
/// </summary>
struct ParticleProps
{

	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT3 Velocity, VelocityVariation;
	DirectX::XMFLOAT4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;

};

class ParticleSystem
{
public:
	ParticleSystem() {};
	ParticleSystem(Graphics& gfx, DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 VelocityVariation, DirectX::XMFLOAT4 ColorBegin, DirectX::XMFLOAT4 ColorEnd, float SizeBegin, float SizeEnd, float SizeVariation, float LifeTime, const char* vs, const char* ps, float pos[3], const char* gs = nullptr);
	virtual ~ParticleSystem() {};
	virtual void Update(float musParams[3], float timeFrame) const;
	void Emit(const ParticleProps& properties);
	DirectX::XMFLOAT3 getPos() { return this->pos; };

private:

	
	DirectX::XMFLOAT3 pos, velocity, velocityVariation;
	DirectX::XMFLOAT4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime;
	int poolIndex = 999;

	bool active;


	std::vector<std::shared_ptr<ParticleWrapper>> particles;

};