#include "ObjectFactory.h"
#include "SceneManager.h"
#include "../Source/Graphics/Drawable/WrapperSolidSphere.h"
#include "../Source/Graphics/Drawable/Particle.h"

void ObjectFactory::SetUpObjectManager(std::shared_ptr<ObjectManager> om)
{
	if (!this->om) {
		this->om = om;
	}
}

void ObjectFactory::SetUpLevelManager()
{
	if (this->om) {
		this->om->SetLevelManager(std::move(std::make_shared<SceneManager>()));
	}
}

void ObjectFactory::AddSphere(Graphics& gfx, float pos[3], float radius, float latDiv, float longDiv, const char* vs, const char* ps, const char* gs)
{
	if (this->om) {
		this->om->AddObject(
			std::make_shared<WrapperSolidSphere>(
			gfx,
			radius, latDiv, longDiv,
			vs,
			ps,
			pos,
			gs
			)
		);
	}
}

void ObjectFactory::AddParticleSystem(Graphics& gfx, DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 VelocityVariation, DirectX::XMFLOAT4 ColorBegin, DirectX::XMFLOAT4 ColorEnd, float SizeBegin, float SizeEnd, float SizeVariation, float LifeTime, const char* vs, const char* ps, float pos[3], const char* gs)
{
	
	if (this->om) {
		this->om->AddObject(std::make_shared<ParticleSystem>(
			gfx,
			Velocity,
			VelocityVariation,
			ColorBegin,
			ColorEnd,
			SizeBegin,
			SizeEnd,
			SizeVariation,
			LifeTime,
			vs,
			ps,
			pos,
			gs
			)
		);
	}
}
