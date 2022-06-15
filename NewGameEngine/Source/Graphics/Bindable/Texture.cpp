/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)

#include "Texture.h"
#include "../Surface.h"
#include "BindableCodex.h"

namespace wrl = Microsoft::WRL;

Texture::Texture(Graphics& gfx, const std::string& path, bool isRenderTarget, unsigned int slot)
	: 
	path(path),
	slot(slot)
{
	Surface s;
	// load surface
	if(!isRenderTarget){
		s = Surface::FromFile(path);
		hasAlpha = s.AlphaLoaded();
	}

	// create texture resource
	D3D11_TEXTURE2D_DESC textureDesc = {};
	if (!isRenderTarget) {
		textureDesc.Width = s.GetWidth();
		textureDesc.Height = s.GetHeight();
	}
	else
	{
		textureDesc.Width =  512;
		textureDesc.Height = 340;
	}
	textureDesc.MipLevels = 1;
	textureDesc.ArraySize = 1;
	textureDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	textureDesc.SampleDesc.Count = 1;
	textureDesc.SampleDesc.Quality = 0;
	textureDesc.Usage = D3D11_USAGE_DEFAULT;
	textureDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	if(isRenderTarget)
		textureDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
	textureDesc.CPUAccessFlags = 0;
	textureDesc.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA sd = {};
	if (!isRenderTarget) {
		sd.pSysMem = s.GetBufferPtr();
		sd.SysMemPitch = s.GetWidth() * sizeof(Surface::Color);
		GetDevice(gfx)->CreateTexture2D(
			&textureDesc, &sd, &pTexture2D
		);
	}
	else {
		GetDevice(gfx)->CreateTexture2D(
			&textureDesc, NULL, &pTexture2D
		);
	}

	// create the resource view on the texture
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Format = textureDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;
	GetDevice(gfx)->CreateShaderResourceView(
		pTexture2D.Get(), &srvDesc, &pTextureView
	);
}

void Texture::Bind(Graphics& gfx) 
{
	GetContext(gfx)->PSSetShaderResources(slot, 1u, pTextureView.GetAddressOf());						
}

std::shared_ptr<Texture> Texture::Resolve(Graphics& gfx, const std::string& path, UINT slot)
{
	return Codex::Resolve<Texture>(gfx, path, slot);
}

std::string Texture::GenerateUID(const std::string& path, UINT slot)
{
	using namespace std::string_literals;
	return typeid(Texture).name() + "#"s + path + "#" + std::to_string(slot);
}

std::string Texture::GetUID() const 
{
	return GenerateUID(path, slot);
}

bool Texture::HasAlpha() const 
{
	return hasAlpha;
}

Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> Texture::GetTextureView() const 
{
	return pTextureView;
}

Microsoft::WRL::ComPtr<ID3D11Texture2D> Texture::GetTexture2D() const
{
	return pTexture2D;
}
