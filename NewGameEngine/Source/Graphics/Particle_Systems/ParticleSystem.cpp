#include "ParticleSystem.h"
#include "../../Common/Random.h"
#include "../../ObjectFactory.h"


ParticleSystem::ParticleSystem(Graphics& gfx, DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 VelocityVariation, DirectX::XMFLOAT4 ColorBegin, DirectX::XMFLOAT4 ColorEnd, float SizeBegin, float SizeEnd, float SizeVariation, float LifeTime, const char* vs, const char* ps, DirectX::XMFLOAT3 pos, const char* gs)
	:
	//SceneObject(gfx, vs, ps, pos, gs),
	velocity(Velocity),
	velocityVariation(VelocityVariation),
	colorBegin(ColorBegin),
	colorEnd(ColorEnd),
	sizeBegin(SizeBegin),
	sizeEnd(SizeEnd),
	sizeVariation(SizeVariation),
	lifeTime(LifeTime),
	pos(pos)
{
	for (int i = 1; i <= 1000; i++) {
		auto particle = std::make_shared<ParticleWrapper>(gfx, 1.0f, 12, 24, vs, ps, pos, gs);
		ObjectFactory::getInstance().getOM()->AddObject(particle);
		//particle->SetSize();
		this->particles.push_back(std::move(particle));
	}
	this->active = true;
	
	
	

}

//ParticleSystem::ParticleSystem(Graphics& gfx, DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 VelocityVariation, DirectX::XMFLOAT4 ColorBegin, DirectX::XMFLOAT4 ColorEnd, float SizeBegin, float SizeEnd, float SizeVariation, float LifeTime, const char* vs, const char* ps, float pos[3], const char* gs)
//{
//}

void ParticleSystem::Update(float musParams[3], float timeFrame) const
{
	if (this->active) {

		for (auto& drawable : this->particles) {
			if (!drawable->isActive()) {
				continue;
			}
			if (typeid(*drawable) == typeid(ParticleWrapper)) {
				auto particle = std::dynamic_pointer_cast<ParticleWrapper>(drawable);

				if (particle->LifeRemaining <= 0.0f)
				{
					particle->deactivate();
					continue;
				}



				particle->LifeRemaining -= timeFrame;



				particle->velocity.x += (particle->velocity.x > 0) ? timeFrame : -timeFrame;
				particle->velocity.y += (particle->velocity.y > 0) ? timeFrame : -timeFrame;
				particle->velocity.z += (particle->velocity.z > 0) ? timeFrame : -timeFrame;
				//
				particle->pos.x += particle->velocity.x;
				particle->pos.y += particle->velocity.y;
				particle->pos.z += particle->velocity.z;


				float life = particle->LifeRemaining / particle->LifeTime;

				float interpolSize = std::lerp(particle->SizeEnd, particle->SizeBegin, life);

				particle->size.x -= interpolSize * timeFrame;
				particle->size.y -= interpolSize * timeFrame;
				particle->size.z -= interpolSize * timeFrame;



				float x = std::lerp(particle->ColorEnd.x, particle->ColorBegin.x, life);
				float y = std::lerp(particle->ColorEnd.y, particle->ColorBegin.y, life);
				float z = std::lerp(particle->ColorEnd.z, particle->ColorBegin.z, life);


				particle->currentColor[0] = x;
				particle->currentColor[1] = y;
				particle->currentColor[2] = z;

				//DirectX::XMVECTOR currentColor = { particle->currentColor[0], particle->currentColor[1], particle->currentColor[2] };
				//auto dataCopy = cbData;
				////DirectX::XMVECTOR color = ;
				////const auto pos = DirectX::XMLoadFloat3(pos);
				//DirectX::XMStoreFloat3(&dataCopy.color, currentColor);
				//
				//cbuf.Update(gfx, dataCopy);
				//
				//cbuf.Bind(gfx);
				//
				//cbufG.Update(gfx, dataCopy);
				//
				//cbufG.Bind(gfx);

			}

		}

	}
}

void ParticleSystem::Emit(const ParticleProps& properties)
{
	auto particleAsDrawable = this->particles[poolIndex];
	std::shared_ptr<ParticleWrapper> particle = nullptr;

	if (typeid(*particleAsDrawable) == typeid(ParticleWrapper)) {
		particle = std::dynamic_pointer_cast<ParticleWrapper>(particleAsDrawable);
	}

	particle->active = true;
	//particle->SetPos(properties.Position);

	// Velocity
	particle->velocity = properties.Velocity;
	particle->velocity.x += properties.VelocityVariation.x * (Random::Float() - 0.5f);
	particle->velocity.y += properties.VelocityVariation.y * (Random::Float() - 0.5f);
	particle->velocity.z += properties.VelocityVariation.z * (Random::Float() - 0.5f);

	// Color
	particle->ColorBegin = properties.ColorBegin;
	particle->ColorEnd = properties.ColorEnd;

	particle->LifeTime = properties.LifeTime;
	particle->LifeRemaining = properties.LifeTime;
	particle->SizeBegin = properties.SizeBegin + properties.SizeVariation * (Random::Float() - 0.5f);
	particle->SizeEnd = properties.SizeEnd;


	particle->size.x = particle->SizeBegin;
	particle->size.y = particle->SizeBegin;
	particle->size.z = particle->SizeBegin;

	poolIndex = --poolIndex % this->particles.size();

}
