#include "RenderAPI.h"
#include "WindowSystem.h"
#include <stdexcept>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <d3d11.h>

RenderAPI::~RenderAPI()
{
    if (d3d11DeviceContext)
    {
        d3d11DeviceContext->ClearState(); 
    }
}

void RenderAPI::Init() noexcept
{
    if (m_instance == nullptr)
    {
        m_instance = std::make_unique<RenderAPI>();

        int height = WindowSystem::GetHeight();
        int width = WindowSystem::GetWidth();

        m_instance->InitDeviceAndSwapChain(width, height);
        m_instance->InitRenderTarget();
        m_instance->InitViewport(width, height);
        m_instance->InitGeometry();

        ID3DBlob* vsBlob = nullptr;
        ID3DBlob* psBlob = nullptr;
        ID3DBlob* errorBlob = nullptr;

    }
}

RenderAPI* RenderAPI::Get()
{
    if (m_instance == nullptr)
    {
        throw std::runtime_error("Called RenderAPI without Initializing it!");
    }
    return m_instance.get();
}

void RenderAPI::Render()
{
    float clearColor[4] = { 1.0, 0.7529, 0.7961 };
    m_instance->PreDraw(clearColor);
    m_instance->Draw();
    m_instance->PostDraw();
}

void RenderAPI::InitGeometry()
{
    Vertex vertices[] = {
        { {-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} }, // Red
    { {1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },  // Green
    { {1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },   // Blue
    { {-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },  // Yellow
    { {-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },  // Purple
    { {1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },   // Cyan
    { {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f} },    // White
    { {-1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f} }    // Black
    };
    D3D11_BUFFER_DESC vbd;
    vbd.Usage = D3D11_USAGE_IMMUTABLE;
    vbd.ByteWidth = sizeof(Vertex) * 8;
    vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vbd.CPUAccessFlags = 0;
    vbd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA vinitData;
    vinitData.pSysMem = vertices;
    #define HR(x) { HRESULT hr = (x); if(FAILED(hr)) throw std::runtime_error("DirectX function failed!"); }
    HR(d3d11Device->CreateBuffer(&vbd, &vinitData, &mBoxVB));


    UINT indices[] = {
        0, 1, 2, 0, 2, 3,
        4, 6, 5, 4, 7, 6,
        4, 5, 1, 4, 1, 0,
        3, 2, 6, 3, 6, 7,
        1, 5, 6, 1, 6, 2,
        4, 0, 3, 4, 3, 7
    };

    D3D11_BUFFER_DESC ibd;
    ibd.Usage = D3D11_USAGE_IMMUTABLE;
    ibd.ByteWidth = sizeof(UINT) * 36;
    ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
    ibd.CPUAccessFlags = 0;
    ibd.MiscFlags = 0;

    D3D11_SUBRESOURCE_DATA iinitData;
    iinitData.pSysMem = indices;
    HR(d3d11Device->CreateBuffer(&ibd, &iinitData, &mBoxIB));

}

void RenderAPI::InitDeviceAndSwapChain(int width, int height)
{
    DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferDesc.Width = width;
    swapChainDesc.BufferDesc.Height = height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
    swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

    swapChainDesc.OutputWindow = WindowSystem::Get()->GetHandleWindow();
    swapChainDesc.SampleDesc.Count = 1;
    swapChainDesc.SampleDesc.Quality = 0;
    swapChainDesc.Windowed = TRUE;

    UINT createDeviceFlags = 0;
#if defined(_DEBUG)
    createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG; 
#endif

    D3D_FEATURE_LEVEL featureLevels[] = {
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0
    };

    HRESULT hr = D3D11CreateDeviceAndSwapChain(
        nullptr,                     
        D3D_DRIVER_TYPE_HARDWARE,    
        nullptr,                     
        createDeviceFlags,          
        featureLevels,               
        ARRAYSIZE(featureLevels),   
        D3D11_SDK_VERSION,           
        &swapChainDesc,              
        dxgiSwapChain.GetAddressOf(),
        d3d11Device.GetAddressOf(),  
        &featureLevel,               
        d3d11DeviceContext.GetAddressOf() 
    );

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create device and swap chain");
    }
}

void RenderAPI::InitRenderTarget()
{
    //back buffer from the swap chain
    Microsoft::WRL::ComPtr<ID3D11Texture2D> backBuffer;
    HRESULT hr = dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to get back buffer");
    }

    //render target view from the back buffer
    hr = d3d11Device->CreateRenderTargetView(backBuffer.Get(), nullptr, renderTargetView.GetAddressOf());
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to create render target view");
    }

    d3d11DeviceContext->OMSetRenderTargets(1, renderTargetView.GetAddressOf(), nullptr);
}

void RenderAPI::InitViewport(int width, int height) {
    D3D11_VIEWPORT viewport;
    viewport.Width = static_cast<FLOAT>(width);
    viewport.Height = static_cast<FLOAT>(height);
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;
    viewport.TopLeftX = 0.0f;
    viewport.TopLeftY = 0.0f;

    d3d11DeviceContext->RSSetViewports(1, &viewport);
}

void RenderAPI::PreDraw(float clearColor[4]) {
    // Clear the render target view with the specified color
    d3d11DeviceContext->ClearRenderTargetView(renderTargetView.Get(), clearColor);
}

void RenderAPI::Draw() {
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    md3dImmediateContext->IASetVertexBuffers(0, 1, &mBoxVB, &stride, &offset);
    md3dImmediateContext->IASetIndexBuffer(mBoxIB, DXGI_FORMAT_R32_UINT, 0);
    md3dImmediateContext->IASetInputLayout(mInputLayout.Get());
    md3dImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // Set shaders
    md3dImmediateContext->VSSetShader(vertexShader.Get(), nullptr, 0);
    md3dImmediateContext->PSSetShader(pixelShader.Get(), nullptr, 0);

    // Draw the cube
    md3dImmediateContext->DrawIndexed(36, 0, 0);
}

void RenderAPI::PostDraw() {
    //Present the back buffer to the screen
    HRESULT hr = dxgiSwapChain->Present(1, 0); 
    if (FAILED(hr)) {
        throw std::runtime_error("Failed to present the swap chain buffer");
    }
}

void RenderAPI::InitShaders()
{
    Microsoft::WRL::ComPtr<ID3DBlob> vertexShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> pixelShaderBlob;
    Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;

    HRESULT hr = D3DCompileFromFile(L"vertexshader.hlsl", nullptr, nullptr, "VS", "vs_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0,
        vertexShaderBlob.GetAddressOf(), errorBlob.GetAddressOf());

    hr = d3d11Device->CreateVertexShader(vertexShaderBlob->GetBufferPointer(),
        vertexShaderBlob->GetBufferSize(),
        nullptr, &vertexShader);

    D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
    };
    hr = D3DCompileFromFile(L"pixelshader.hlsl", nullptr, nullptr, "PS", "ps_5_0",
        D3DCOMPILE_ENABLE_STRICTNESS, 0,
        pixelShaderBlob.GetAddressOf(), errorBlob.GetAddressOf());
    
}
