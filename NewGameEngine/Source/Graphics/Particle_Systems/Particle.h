/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#pragma once
#include "../Drawable/IndexedTriangleList.h"
#include "../Drawable/Drawable.h"
#include "../../SceneObject.h"
#include "../Bindable/ConstantBuffer.h"



/// <summary>
/// Concrete implementation of Drawable - Sphere.
/// Thanks to https://github.com/TheCherno/OneHourParticleSystem/blob/master/OpenGL-Sandbox/src/ParticleSystem.h.
/// Love you to death, TheCherno.
/// </summary>
class Particle : public Drawable
{
public:
	Particle(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, const char* gs);
	DirectX::XMMATRIX GetTransformXM() const  override;
};
