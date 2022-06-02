#include "Particle.h"
#include "../../Common/Random.h"
#include "../../Graphics/Graphics.h"
#include "../Bindable/BindableBase.h"
#include "../Bindable/BindableCodex.h"
#include "../Drawable/Sphere.h"
#include "../../Common/Utilities.h"
#include <typeinfo>
#include <cmath>

Particle::Particle(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, const char* gs)
{


	namespace dx = DirectX;

	this->size.x = radius;
	this->size.y = radius;
	this->size.z = radius;
	// Make Sphere using static function and scale it.
	auto model = Sphere::Make(latDiv, longDiv);
	model.Transform(dx::XMMatrixScaling(this->size.x, this->size.y, this->size.z));
	// Uniqueness of the VertexBuffer and IndexBuffer are determined by the existance of the geometry shader (or its name). It needed solely for the purposes of the implememntation of the size and long/lat divisions of the sphere.
	// It saves resources of the application. 
	// Not suitable for the use in any other case.
	std::string GS_St;
	if (gs)
		GS_St = gs;
	else
		GS_St = "";

	// Add binds to the vector using Bindable Codex.
	AddBind(VertexBuffer::Resolve(gfx, "sphere_vertices" + GS_St, convertVertexStructres(model.vertices)));
	AddBind(IndexBuffer::Resolve(gfx, "sphere_indices" + GS_St, model.indices));


	// Vertex Shader 
	auto pvs = VertexShader::Resolve(gfx, vs);
	auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();		// byte of the vertex shader need in the input layout
	AddBind(std::move(pvs));

	// If geometry shader is specified bind it. else bind a dummy one, that just passes data to the pixel shader
	if (gs) {
		AddBind(GeometryShader::Resolve(gfx, gs));
	}
	else {
		AddBind(GeometryShader::Resolve(gfx, "DummyGS.cso"));
	}

	// pixel shader
	AddBind(PixelShader::Resolve(gfx, ps));

	// Input layout
	const std::vector<D3D11_INPUT_ELEMENT_DESC> ied =
	{
		{ "Position",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 },
		{ "Normal",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }
	};

	AddBind(InputLayout::Resolve(gfx, ied, pvsbc));

	// Topology
	AddBind(Topology::Resolve(gfx, D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST));

	// TransformCbuf
	AddBind(std::make_shared<TransformCbuf>(gfx, *this));


	this->active = false;
}

DirectX::XMMATRIX Particle::GetTransformXM() const
{
	auto transform	= DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
	transform		*= DirectX::XMMatrixScaling(this->size.x, this->size.y, this->size.z);
	return transform;													// Returns model transformation matrix in world space
}


ParticleSystem::ParticleSystem(Graphics& gfx, DirectX::XMFLOAT3 Velocity, DirectX::XMFLOAT3 VelocityVariation, DirectX::XMFLOAT4 ColorBegin, DirectX::XMFLOAT4 ColorEnd, float SizeBegin, float SizeEnd, float SizeVariation, float LifeTime, const char* vs, const char* ps, float pos[3], const char* gs)
	:
	SceneObject(gfx, vs, ps, pos, gs),
	velocity(Velocity),
	velocityVariation(VelocityVariation),
	colorBegin(ColorBegin),
	colorEnd(ColorEnd),
	sizeBegin(SizeBegin),
	sizeEnd(SizeEnd),
	sizeVariation(SizeVariation),
	lifeTime(LifeTime),
	cbuf(gfx),
	cbufG(gfx, 1u)
{
	for (int i = 1; i <= 1000; i++) {
		auto particle = std::make_shared<Particle>(gfx, 0.4f, 12, 24, vs,  ps, gs);
		particle->SetPos(this->pos);
		this->meshes.push_back(std::move(particle));
	}
	this->active = true;
	

}

void ParticleSystem::StartParticles(std::string type)
{
	for (auto& drawable : this->meshes) {
		
	}
}

void ParticleSystem::Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[3], float timeFrame) const
{

	if (this->active) {

		for (auto& drawable : this->meshes) {
			if (!drawable->isActive()) {
				continue;
			}
			if (typeid(*drawable) == typeid(Particle)) {
				auto particle = std::dynamic_pointer_cast<Particle>(drawable);
			
				if (particle->LifeRemaining <= 0.0f)
				{
					particle->deactivate();
					continue;
				}

				particle->LifeRemaining -= 0.00016f;

				particle->velocity.x += timeFrame;
				particle->velocity.y += timeFrame;
				particle->velocity.z += timeFrame;
			
				particle->pos.x += particle->velocity.x;
				particle->pos.y += particle->velocity.y;
				particle->pos.z += particle->velocity.z;


				float life = particle->LifeRemaining / particle->LifeTime;

				float interpolSize = std::lerp(particle->SizeEnd, particle->SizeBegin, life);
				particle->size.x *= interpolSize;
				particle->size.y *= interpolSize;
				particle->size.z *= interpolSize;



				float x = std::lerp(particle->ColorEnd.x, particle->ColorBegin.x, life);
				float y = std::lerp(particle->ColorEnd.y, particle->ColorBegin.y, life);
				float z = std::lerp(particle->ColorEnd.z, particle->ColorBegin.z, life);

				
				particle->currentColor[0] = x;
				particle->currentColor[1] = y;
				particle->currentColor[2] = z;
				
				DirectX::XMVECTOR currentColor = { particle->currentColor[0], particle->currentColor[1], particle->currentColor[2] };
				auto dataCopy = cbData;
				//DirectX::XMVECTOR color = ;
				//const auto pos = DirectX::XMLoadFloat3(pos);
				DirectX::XMStoreFloat3(&dataCopy.color, currentColor);

				cbuf.Update(gfx, dataCopy);
				cbufG.Update(gfx, dataCopy);
				
				cbuf.Bind(gfx);
				cbufG.Bind(gfx);

			}
		
		}

	}

}

void ParticleSystem::Draw(Graphics& gfx) const
{
	if (this->active) {
		for (auto& drawable : this->meshes) {
			if (!drawable->isActive())
				continue;
			drawable->Draw(gfx);
		}
	}
}

void ParticleSystem::Emit(const ParticleProps& properties)
{
	auto particleAsDrawable = this->meshes[poolIndex];
	std::shared_ptr<Particle> particle = nullptr;

	if (typeid(*particleAsDrawable) == typeid(Particle)) {
		particle = std::dynamic_pointer_cast<Particle>(particleAsDrawable);
	}

	particle->active = true;
	particle->SetPos(properties.Position);

	// Velocity
	particle->velocity    =	properties.Velocity;
	particle->velocity.x +=	properties.VelocityVariation.x * (Random::Float() - 0.5f);
	particle->velocity.y +=	properties.VelocityVariation.y * (Random::Float() - 0.5f);
	particle->velocity.z +=	properties.VelocityVariation.z * (Random::Float() - 0.5f);

	// Color
	particle->ColorBegin = properties.ColorBegin;
	particle->ColorEnd   = properties.ColorEnd;

	particle->LifeTime		= properties.LifeTime;
	particle->LifeRemaining = properties.LifeTime;
	particle->SizeBegin		= properties.SizeBegin + properties.SizeVariation * (Random::Float() - 0.5f);
	particle->SizeEnd		= properties.SizeEnd;

	poolIndex = --poolIndex % this->meshes.size();

}

void ParticleSystem::Reset()
{
	SceneObject::Reset();
	this->_Reset();
}

void ParticleSystem::_Reset()
{
	this->cbData = {
		{ 0.0f,0.0f,0.0f }
	};
}
