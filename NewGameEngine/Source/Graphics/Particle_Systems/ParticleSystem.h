#pragma once

#include "../../SceneObject.h"


/// <summary>
/// Struct represents Particle Properties.
/// Thanks to https://github.com/TheCherno/OneHourParticleSystem/blob/master/OpenGL-Sandbox/src/ParticleSystem.h.
/// Love you to death, TheCherno.
/// </summary>
struct ParticleProps
{
	DirectX::XMFLOAT2 Position;
	DirectX::XMFLOAT2 Velocity, VelocityVariation;
	DirectX::XMFLOAT4 ColorBegin, ColorEnd;
	float SizeBegin, SizeEnd, SizeVariation;
	float LifeTime = 1.0f;
};

/// <summary>
/// Class represents Particle System.
/// Thanks to https://github.com/TheCherno/OneHourParticleSystem/blob/master/OpenGL-Sandbox/src/ParticleSystem.h.
/// Love you to death, TheCherno.
/// </summary>
class ParticleSystem: public SceneObject
{
public:
	ParticleSystem();

	virtual void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3]) const;
	virtual void Draw(Graphics& gfx) const;

	void Emit(const ParticleProps& particleProps);
private:
	struct Particle
	{
		DirectX::XMFLOAT2 Position;
		float Rotation = 0.0f;
		float SizeBegin, SizeEnd;
		float LifeTime = 1.0f;
		float LifeRemaining = 0.0f;
		bool Active = false;
	};

	DirectX::XMFLOAT2 Velocity;
	DirectX::XMFLOAT4 ColorBegin, ColorEnd;

	//std::vector<Particle> m_ParticlePool;
	uint32_t m_PoolIndex = 999;

	int m_QuadVA = 0;
};