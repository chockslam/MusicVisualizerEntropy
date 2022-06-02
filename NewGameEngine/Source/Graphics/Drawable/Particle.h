/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "../Drawable/IndexedTriangleList.h"
#include "Drawable.h"
#include "../../SceneObject.h"
#include "../Bindable/ConstantBuffer.h"

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

class ParticleSystem : public SceneObject
{
public:
	ParticleSystem(Graphics& gfx, DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 VelocityVariation, DirectX::XMFLOAT4 ColorBegin, DirectX::XMFLOAT4 ColorEnd, float SizeBegin, float SizeEnd, float SizeVariation, float LifeTime, const char* vs, const char* ps, float pos[3], const char* gs = nullptr);
	virtual ~ParticleSystem() {};
	void StartParticles(std::string type = "sphere");
	virtual void Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3], float timeFrame) const;
	virtual void Draw(Graphics& gfx) const;
	void Emit(const ParticleProps& properties);
	void Reset();
private:
	virtual void _Reset();

	struct ParticleCBuf
	{
		alignas(16) DirectX::XMFLOAT3 color;
	};

	ParticleCBuf cbData;											// Struct object declared above.
	mutable PixelConstantBuffer<ParticleCBuf> cbuf;							// Pixel Constant buffer that uses structure declared above as a data.
	mutable GeometryConstantBuffer<ParticleCBuf> cbufG;			// Geometry Constant buffer that uses structure declared above as a data.


	DirectX::XMFLOAT3 velocity, velocityVariation;
	DirectX::XMFLOAT4 colorBegin, colorEnd;
	float sizeBegin, sizeEnd, sizeVariation;
	float lifeTime;
	int poolIndex = 999;
	
};


/// <summary>
/// Concrete implementation of Drawable - Sphere.
/// Thanks to https://github.com/TheCherno/OneHourParticleSystem/blob/master/OpenGL-Sandbox/src/ParticleSystem.h.
/// Love you to death, TheCherno.
/// </summary>
class Particle : public Drawable
{
	friend ParticleSystem;
public:
	Particle(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, const char* gs);
	DirectX::XMMATRIX GetTransformXM() const  override;
	
private:
	
	DirectX::XMFLOAT3 velocity;
	DirectX::XMFLOAT4 ColorBegin, ColorEnd;
	
	float currentColor[3];

	float SizeBegin, SizeEnd;

	float LifeTime = 1.0f;
	float LifeRemaining = 0.0f;

	float rotation;

};
