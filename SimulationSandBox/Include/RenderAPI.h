#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <wrl.h>
#include <memory>
#include <DirectXMath.h>


struct Vertex
{
	DirectX::XMFLOAT3 Pos;
	DirectX::XMFLOAT4 Color;
};

class RenderAPI
{
public:
    RenderAPI() = default;
    ~RenderAPI();

    static void Init() noexcept;
    static RenderAPI* Get();
    static void Render();

private:

    void InitGeometry();

    void PreDraw(float clearColor[4]);
    void Draw();
    void PostDraw();
    void InitShaders();

    void InitDeviceAndSwapChain(int width, int height);
    void InitRenderTarget();
    void InitViewport(int width, int height);

private:

    ID3D11Buffer* mBoxVB;
    ID3D11Buffer* mBoxIB;
    Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11DeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> dxgiSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;
    Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
    Microsoft::WRL::ComPtr<ID3D11VertexShader> vertexShader;
    Microsoft::WRL::ComPtr<ID3D11PixelShader> pixelShader;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> md3dImmediateContext;

    D3D_FEATURE_LEVEL featureLevel;

    inline static std::unique_ptr<RenderAPI> m_instance = nullptr;
};
