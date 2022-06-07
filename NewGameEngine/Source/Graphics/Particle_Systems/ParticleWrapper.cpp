#include "ParticleWrapper.h"
#include "Particle.h"

ParticleWrapper::ParticleWrapper(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps,  float pos[3], const char* gs)
	:
	SceneObject(gfx, vs, ps, pos, gs),
	cbuf(gfx),
	cbufG(gfx, 1u)
{

	this->velocity = { 0.01f, 0.01f , 0.01f };
	this->active = false;

	auto mesh = std::make_shared<Particle>(gfx, radius, latDiv, longDiv, vs, ps, gs);
	this->meshes.push_back(std::move(mesh));
}

void ParticleWrapper::Reset()
{
	SceneObject::Reset();
	this->_Reset();
}

void ParticleWrapper::Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3], float timeFrame) const
{
	if (this->active) {
		auto dataCopy = cbData;
		//const auto pos = DirectX::XMLoadFloat3(pos);
		DirectX::XMVECTOR color = { currentColor[0], currentColor[1], currentColor[2] };

		DirectX::XMStoreFloat3(&dataCopy.color, color);

		// Update pixel constant buffer amd geometry constant buffer with audio data.
		cbuf.Update(gfx, dataCopy);
		cbufG.Update(gfx, dataCopy);

		// Bind constant buffers to the pipeline
		cbuf.Bind(gfx);
		cbufG.Bind(gfx);
	}
}


void ParticleWrapper::_Reset()
{
	this->cbData = {
		{ 0.0f,0.0f,0.0f },
	};
}
