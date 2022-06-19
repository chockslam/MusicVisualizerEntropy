/// CODE was written with help by ChiliTomatoNoodle (https://www.youtube.com/c/ChiliTomatoNoodle) (https://github.com/planetchili/hw3d)
#include "Graphics.h"
#include <sstream>
#include <d3dcompiler.h>
#include <cmath>
#include <DirectXMath.h>
#include "../GUI/GUIwrap.h"

#include "../Common/WNDconst.h"

namespace wrl = Microsoft::WRL;
namespace dx = DirectX;




Graphics::Graphics(HWND& hWnd)
{


	
	DXGI_SWAP_CHAIN_DESC sd = {};
	//sd.BufferCount = 2;
	sd.BufferDesc.Width = 0;
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = hWnd;
	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT swapCreateFlags = 0u;
	D3D_FEATURE_LEVEL featureLevel;
	const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
	//creating swapChain + device
	D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		swapCreateFlags,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&sd,
		&pSwap,
		&pDevice,
		nullptr,
		&pContext
	);

	// gain access to texture subresource in swap chain (back buffer)
	wrl::ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &pWindowTarget);

	// create depth stensil state
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	wrl::ComPtr<ID3D11DepthStencilState> pDSState;
	pDevice->CreateDepthStencilState(&dsDesc, &pDSState);

	// bind depth state
	pContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	// create depth stensil texture
	wrl::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC descDepth = {};
	descDepth.Width = S_WIDTH;
	descDepth.Height = S_HEIGHT;
	descDepth.MipLevels = 1u;
	descDepth.ArraySize = 1u;
	descDepth.Format = DXGI_FORMAT_D32_FLOAT;
	descDepth.SampleDesc.Count = 1u;
	descDepth.SampleDesc.Quality = 0u;
	descDepth.Usage = D3D11_USAGE_DEFAULT;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	pDevice->CreateTexture2D(&descDepth, nullptr, &pDepthStencil);

	// create view of depth stensil texture
	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;
	pDevice->CreateDepthStencilView(
		pDepthStencil.Get(), &descDSV, &pDSV
	);

	// bind depth stensil view to OM
	pContext->OMSetRenderTargets(1u, pWindowTarget.GetAddressOf(), NULL);
	// configure viewport
	
	vpWindow.Width = W_WIDTH;
	vpWindow.Height = W_HEIGHT;
	vpWindow.MinDepth = 0.0f;
	vpWindow.MaxDepth = 1.0f;
	vpWindow.TopLeftX = 0.0f;
	vpWindow.TopLeftY = 0.0f;
	pContext->RSSetViewports(1u, &vpWindow);

	vpRenderTexture.Width = S_WIDTH;
	vpRenderTexture.Height = S_HEIGHT;
	vpRenderTexture.MinDepth = 0.0f;
	vpRenderTexture.MaxDepth = 1.0f;
	vpRenderTexture.TopLeftX = 0.0f;
	vpRenderTexture.TopLeftY = 0.0f;

	currentTarget = pWindowTarget.GetAddressOf();

	pRenderTexture = std::make_shared<RenderTexture>();
	pRenderTexture->Initialize(pDevice.Get(), S_WIDTH, S_HEIGHT);
	initImgui(hWnd);


}

Graphics::~Graphics()
{
	ImGui_ImplDX11_Shutdown();
	ImGui::DestroyContext(imgCont);
	ImPlot::DestroyContext(impCont);
}

void Graphics::EndFrame(float red, float green, float blue)
{
	pSwap->Present(1u, 0u);

	
	const float color[] = { red,green,blue,1.0f };
	pContext->ClearRenderTargetView(pWindowTarget.Get(), color);
	pContext->ClearDepthStencilView(pDSV.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);

}


void Graphics::BeginFrame(float red, float green, float blue) 
{
	
		ImGui_ImplDX11_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();

		const float color[] = { red,green,blue,1.0f };
	
}
void Graphics::DrawIndexed(UINT count) 
{
	pContext->DrawIndexed(count, 0u, 0u);
}

void Graphics::SetProjection(DirectX::FXMMATRIX proj) 
{
	projection = proj;					// Set projection matrix.
}

std::shared_ptr<RenderTexture> Graphics::getRenderTexture()
{
	return this->pRenderTexture;
}

void Graphics::switchRenderTargetToTexture()
{
	//ImVec2 view = ImGui::GetContentRegionAvail();
	ImGui::Render();

	//pContext->OMSetRenderTargets(1, pWindowTarget.GetAddressOf(), NULL);
	//const float color[] = { 0.07f, 0.0f, 0.12f,1.0f };
	//pContext->ClearRenderTargetView(pWindowTarget.Get(), color);

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
	if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
	}

	pContext->RSSetViewports(1u, &vpRenderTexture);

	pRenderTexture->SetRenderTarget(pContext.Get(), pDSV.Get());
	
	pRenderTexture->ClearRenderTarget(pContext.Get(), pDSV.Get(), 0.07f, 0.0f, 0.12f, 1.0f);

}

void Graphics::switchRenderTargetToWindow()
{
	pContext->RSSetViewports(1u, &vpWindow);
	pContext->OMSetRenderTargets(1u, pWindowTarget.GetAddressOf(), NULL);
	
}

void Graphics::ResizeImgui(UINT width, UINT height)
{
	CleanupRenderTarget();
	vpWindow.Width = width;
	vpWindow.Height = height;
	vpWindow.MinDepth = 0.0f;
	vpWindow.MaxDepth = 1.0f;
	vpWindow.TopLeftX = 0;
	vpWindow.TopLeftY = 0;
	pContext->RSSetViewports(1, &vpWindow);
	pSwap->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	CreateRenderTarget();

	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2(width, height);

	//  pContext->OMSetRenderTargets(0, 0, 0);
	//  //pWindowTarget->Release();
	//  pSwap->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
	//  wrl::ComPtr<ID3D11Resource> pBackBuffer;
	//  pSwap->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	//  
	//  pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL,
	//  	&pWindowTarget);
	//  pBackBuffer->Release();
	//  
	//D3D11_VIEWPORT vp;
	
	////  
	////  //for (const auto& viewport : ImGui::GetCurrentContext()->Viewports)
	////  //{
	////  //	ImGui::ScaleWindowsInViewport(viewport, relative_scale_reset);
	////  //}
	////  
	////  //auto testSize = ImGui::GetWindowSize();
	//pContext->RSSetViewports(1, &vpWindow);
}



DirectX::XMMATRIX Graphics::GetProjection() const 
{
	return projection;					// Get projection matrix
}

void Graphics::SetCamera(DirectX::FXMMATRIX cam) 
{
	camera = cam;						// Set camera matrix (view matrix)
}

DirectX::XMMATRIX Graphics::GetCamera() const 
{
	return camera;						// Get camera matrix.
}
void Graphics::CleanupRenderTarget()
{
	if (pWindowTarget) { pWindowTarget->Release(); pWindowTarget = nullptr; }
}
void Graphics::CreateRenderTarget()
{
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	pSwap->GetBuffer(0,__uuidof(ID3D11Resource), &pBackBuffer);
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, pWindowTarget.GetAddressOf());
	pContext->OMSetRenderTargets(1u, pWindowTarget.GetAddressOf(), NULL);
	auto vec2 = ImGui::GetCurrentContext();

}
/// <summary>
/// initialize imgui
/// </summary>
/// <param name="hwnd"> Window representation </param>
/// <returns></returns>
void Graphics::initImgui(HWND& hwnd) 
{
	// Initialize Imgui.
	IMGUI_CHECKVERSION();
	imgCont = ImGui::CreateContext();
	impCont = ImPlot::CreateContext();
	

	GUIwrap::getInstance().EnableDocking();
	
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX11_Init(pDevice.Get(), pContext.Get());



	ImGui::StyleColorsDark();
}



