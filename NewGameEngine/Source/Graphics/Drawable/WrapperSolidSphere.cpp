#include "WrapperSolidSphere.h"
#include <imgui.h>
#include <implot.h>

WrapperSolidSphere::WrapperSolidSphere(Graphics& gfx, float radius, int latDiv, int longDiv, const char* vs, const char* ps, DirectX::XMFLOAT3 pos, const char* gs, std::string channel)
	:
	SceneObject(gfx, vs, ps, pos, gs, channel),
	cbuf(gfx),
	cbufG(gfx, 1u)
{
	
	auto mesh = std::make_shared<SolidSphere>(gfx, radius, latDiv, longDiv, vs, ps, gs);
	this->meshes.push_back(std::move(mesh));
}

WrapperSolidSphere::~WrapperSolidSphere()
{
	
}

void WrapperSolidSphere::Reset()
{
	SceneObject::Reset();
	this->_Reset();
}






void WrapperSolidSphere::Bind(Graphics& gfx, DirectX::FXMMATRIX view, float musParams[6], float timeFrame) const
{
	if (this->active) {
		auto dataCopy = cbData;
		DirectX::XMVECTOR color = { 1.0f, 1.0f ,1.0f };
		if (this->channel == "left")
			color = { musParams[0], musParams[1], musParams[2] };
		else if(this->channel == "right")
			color = { musParams[3], musParams[4], musParams[5] };
		//const auto pos = DirectX::XMLoadFloat3(pos);
		DirectX::XMStoreFloat3(&dataCopy.color, color);

		// Update pixel constant buffer amd geometry constant buffer with audio data.
		cbuf.Update(gfx, dataCopy);
		cbufG.Update(gfx, dataCopy);

		// Bind constant buffers to the pipeline
		cbuf.Bind(gfx);
		cbufG.Bind(gfx);
	}
	
}

void WrapperSolidSphere::_Reset()
{
	this->cbData = {
		{ 0.4f,0.4f,0.4f },
	};
}
