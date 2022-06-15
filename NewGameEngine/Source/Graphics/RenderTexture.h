#pragma once
#include <wrl.h>
#include <d3d11.h>
/// <summary>
/// Render Target Texture class facilitates rendering a scene to texture
/// Thanks to https://www.rastertek.com/dx11tut22.html 
/// </summary>
class RenderTexture {
public:
	RenderTexture();

	bool Initialize(ID3D11Device*, int, int);
	void Shutdown();

	void SetRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*);
	void ClearRenderTarget(ID3D11DeviceContext*, ID3D11DepthStencilView*, float, float, float, float);
	ID3D11ShaderResourceView* GetShaderResourceView();

private:
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_renderTargetTexture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_renderTargetView;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResourceView;
};