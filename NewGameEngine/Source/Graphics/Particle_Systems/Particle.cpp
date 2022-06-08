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
	AddBind(VertexBuffer::Resolve(gfx, "sphere_vertices" + GS_St + "Particle", convertVertexStructres(model.vertices)));
	AddBind(IndexBuffer::Resolve(gfx, "sphere_indices" + GS_St + "Particle", model.indices));


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


	//this->velocity = { 0.01f, 0.01f , 0.01f };

	this->active = true;
}

DirectX::XMMATRIX Particle::GetTransformXM() const
{
	auto transform =
		DirectX::XMMatrixIdentity() *
		DirectX::XMMatrixScaling(this->size.x, this->size.y, this->size.z) *
		DirectX::XMMatrixTranslation(this->pos.x, this->pos.y, this->pos.z);
		
	return transform;													// Returns model transformation matrix in world space
}


