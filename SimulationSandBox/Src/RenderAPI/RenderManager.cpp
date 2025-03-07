#include "RenderAPI/RenderManager.h"

#include "Core/SystemManager/SubsystemManager.h"
#include "WindowsAPI/WindowsManager.h"

#include <stdexcept>
#include <d3dcompiler.h>
#include <iostream>

#include "RenderAPI/RenderQueue.h"
#include "imgui/imgui_impl_dx11.h"
#include "GuiAPI/GUIManager.h"


RenderManager::~RenderManager()
{
	Shutdown();
}

void RenderManager::Initialize()
{
	InitDeviceAndSwapChain();
	InitRenderTargetAndDepthStencil();
	InitViewport();
	ImGui_ImplDX11_Init(md3dDevice.Get(), md3dDeviceContext.Get());
}

void RenderManager::Shutdown()
{}

void RenderManager::RecordingSetup()
{
	IFrameInterface::RecordingSetup();
}

void RenderManager::RecordingBegin()
{
	md3dDeviceContext->ClearRenderTargetView(mRenderTargetView.Get(), mColor);
	md3dDeviceContext->ClearDepthStencilView(mDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
}

void RenderManager::RecordingExecute(float deltaTime)
{
	//~ Setup for render
	SubsystemManager::Get<RenderQueue>()->RenderObjects(md3dDeviceContext.Get());

	//~ Setup ImGui Simulation
	ImGui_ImplDX11_NewFrame();
	ImGui::NewFrame();
}

void RenderManager::RecordingEnd()
{
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	mdxgiSwapChain->Present(0u, 0u);

}

void RenderManager::BuildVertexShader(const std::wstring& path, Microsoft::WRL::ComPtr<ID3D11VertexShader>& pVertexShader) const
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GetBlob(path, pBlob);

	HRESULT hr = md3dDevice->CreateVertexShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr, pVertexShader.ReleaseAndGetAddressOf());

	if (FAILED(hr)) throw std::runtime_error("Failed to Create Vertex Shader");
	pBlob->Release();
}

void RenderManager::BuildPixelShader(const std::wstring& path, Microsoft::WRL::ComPtr<ID3D11PixelShader>& pPixelShader) const
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	GetBlob(path, pBlob);

	HRESULT hr = md3dDevice->CreatePixelShader(pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		nullptr, pPixelShader.ReleaseAndGetAddressOf());

	if (FAILED(hr)) throw std::runtime_error("Failed To Create Pixel Shader");
	pBlob->Release();
}

void RenderManager::BuildInputLayout(const std::wstring& VertBlobPath,
	const D3D11_INPUT_ELEMENT_DESC* pInputElements, UINT inputElementSize,
	Microsoft::WRL::ComPtr<ID3D11InputLayout>& pInputLayout) const
{
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob = nullptr;
	GetBlob(VertBlobPath, pBlob);

	HRESULT hr = md3dDevice->CreateInputLayout(pInputElements, inputElementSize,
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(), pInputLayout.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw std::runtime_error("Failed to Create Input Layout");

	pBlob->Release();
}

void RenderManager::BuildVertexBuffer(const std::vector<Simulation::Vertex>& vertices, Microsoft::WRL::ComPtr<ID3D11Buffer>& pBuffer) const
{
	D3D11_BUFFER_DESC ds{};
	ds.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	ds.ByteWidth = static_cast<UINT>(vertices.size() * sizeof(Simulation::Vertex));
	ds.CPUAccessFlags = 0;
	ds.Usage = D3D11_USAGE_DEFAULT;

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = vertices.data();

	HRESULT hr = md3dDevice->CreateBuffer(&ds, &data, pBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr)) { throw std::runtime_error("Failed To Create Vertex Buffer!"); }
}

void RenderManager::BuildIndexBuffer(const std::vector<WORD>& indices, Microsoft::WRL::ComPtr<ID3D11Buffer>& pBuffer) const
{
	D3D11_BUFFER_DESC ds{};
	ds.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ds.Usage = D3D11_USAGE_DEFAULT;
	ds.CPUAccessFlags = 0;
	ds.ByteWidth = static_cast<UINT>(sizeof(WORD) * indices.size());

	D3D11_SUBRESOURCE_DATA data{};
	data.pSysMem = indices.data();

	HRESULT hr = md3dDevice->CreateBuffer(&ds, &data, pBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr)) { throw std::runtime_error("Failed To Create Index Buffer!"); }
}

void RenderManager::BuildConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& pBuffer) const
{
	D3D11_BUFFER_DESC ds{};
	ds.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	ds.ByteWidth = sizeof(Simulation::WorldSpace);
	ds.CPUAccessFlags = 0;
	ds.Usage = D3D11_USAGE_DEFAULT;

	HRESULT hr = md3dDevice->CreateBuffer(&ds, nullptr, pBuffer.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw std::runtime_error("Failed To Create Constant Buffer!");
}

void RenderManager::BuildRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterizerState) const
{
	HRESULT hr = md3dDevice->CreateRasterizerState(pRasterizerDesc, pRasterizerState.ReleaseAndGetAddressOf());
	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to Create Raster State!");
	}
}

void RenderManager::SetBackgroundColor(float color[])
{
	for (int i = 0; i < 4; i++)
	{
		mColor[i] = color[i];
	}
}

ID3D11Device* RenderManager::GetDevice() const
{
	return md3dDevice.Get();
}

ID3D11DeviceContext* RenderManager::GetDeviceContext() const
{
	return md3dDeviceContext.Get();
}

void RenderManager::InitDeviceAndSwapChain()
{
#ifdef _BABYIA
	UINT debugFlag = D3D11_CREATE_DEVICE_DEBUG;
	UINT swapDebugFlag = 0u;
#else
	UINT debugFlag = 0u;
	UINT swapDebugFlag = 0u;
#endif

	DXGI_SWAP_CHAIN_DESC sd{};
	sd.BufferCount = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.Width = SubsystemManager::Get<WindowsManager>()->GetWidth();
	sd.BufferDesc.Height = SubsystemManager::Get<WindowsManager>()->GetHeight();
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = SubsystemManager::Get<WindowsManager>()->GetHandleWindow();
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;

	sd.Windowed = TRUE;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = swapDebugFlag;

	const HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr,
		D3D_DRIVER_TYPE_HARDWARE, nullptr,
		debugFlag, nullptr, 0u, D3D11_SDK_VERSION,
		&sd, &mdxgiSwapChain, &md3dDevice, nullptr,
		&md3dDeviceContext);

	if (FAILED(hr))
	{
		throw std::runtime_error("Failed to Create Device and SwapChain!");
	}
}

void RenderManager::InitRenderTargetAndDepthStencil()
{

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pBackBuffer;
	HRESULT hr = mdxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed To get back buffer", L"Error", MB_ICONERROR);
		throw std::runtime_error("Failed to get back buffer from swap chain.");
	}

	// Create render target view (RTV)
	hr = md3dDevice->CreateRenderTargetView(pBackBuffer.Get(), nullptr, &mRenderTargetView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed To Create RTV", L"Error", MB_ICONERROR);
		throw std::runtime_error("Failed to create render target view.");
	}

	// Describe depth stencil buffer
	D3D11_TEXTURE2D_DESC depthStencilDesc{};
	depthStencilDesc.Width = SubsystemManager::Get<WindowsManager>()->GetWidth();
	depthStencilDesc.Height = SubsystemManager::Get<WindowsManager>()->GetHeight();
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count = 1;
	depthStencilDesc.SampleDesc.Quality = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.MiscFlags = 0;

	// Create depth stencil buffer
	hr = md3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, &mDepthStencilBuffer);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed To Create DSB", L"Error", MB_ICONERROR);
		throw std::runtime_error("Failed to create depth stencil buffer.");
	}

	// Create depth stencil view (DSV)
	hr = md3dDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, &mDepthStencilView);
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed To Create DSV", L"Error", MB_ICONERROR);
		throw std::runtime_error("Failed to create depth stencil view.");
	}

	D3D11_DEPTH_STENCIL_DESC dsDesc{};
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;

	dsDesc.StencilEnable = FALSE;  // Change to TRUE if using stencil
	hr = md3dDevice->CreateDepthStencilState(&dsDesc, &mDepthStencilState);

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Failed To Create DSS", L"Error", MB_ICONERROR);
		throw std::runtime_error("Failed to create depth stencil state.");
	}
	md3dDeviceContext->OMSetDepthStencilState(mDepthStencilState.Get(), 0);

	// Bind RTV and DSV to output merger (OM) stage
	md3dDeviceContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
}

void RenderManager::InitViewport() const
{
	D3D11_VIEWPORT vp{};
	vp.Width = static_cast<FLOAT>(SubsystemManager::Get<WindowsManager>()->GetWidth());
	vp.Height = static_cast<FLOAT>(SubsystemManager::Get<WindowsManager>()->GetHeight());
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	vp.TopLeftX = 0.0f;
	vp.TopLeftY = 0.0f;

	md3dDeviceContext->RSSetViewports(1, &vp);
}

void RenderManager::GetBlob(const std::wstring& path, Microsoft::WRL::ComPtr<ID3DBlob>& blob)
{
	HRESULT hr = D3DReadFileToBlob(path.c_str(), blob.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw std::runtime_error("Failed to ReadBlob");
}
