/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#include "SolidSphere.h"
#include "../Bindable/BindableBase.h"
#include "Sphere.h"
#include "../../Common/Utilities.h"



SolidSphere::SolidSphere(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, const char* gs)
{
	
	namespace dx = DirectX;

	// Make Sphere using static function and scale it.
	auto model = Sphere::Make(latDiv,longDiv);
	model.Transform(dx::XMMatrixScaling(radius, radius, radius));
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
	AddBind(IndexBuffer::Resolve(gfx,"sphere_indices" + GS_St, model.indices));

	// Vertex Shader 
	auto pvs = VertexShader::Resolve(gfx, vs);
	auto pvsbc = static_cast<VertexShader&>(*pvs).GetBytecode();		// byte of the vertex shader need in the input layout
	AddBind(std::move(pvs));

	// If geometry shader is specified bind it. else bind a dummy one, that just passes data to the pixel shader
	if(gs){
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
	AddBind(Topology::Resolve( gfx,D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST ));

	// TransformCbuf
	AddBind(std::make_shared<TransformCbuf>(gfx, *this));

	this->active = true;
}




DirectX::XMMATRIX SolidSphere::GetTransformXM() const 
{
	return DirectX::XMMatrixTranslation(pos.x, pos.y, pos.z);		// Returns model transformation matrix in world space
}
