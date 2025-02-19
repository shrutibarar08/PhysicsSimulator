#pragma once

#include <d3d11.h>
#include <dxgi.h>
#include <wrl/client.h>
#include <stdexcept>

class RenderAPI {
public:
    RenderAPI();
    ~RenderAPI();

    void InitDeviceAndSwapChain(HWND hwnd, int width, int height);
    void InitRenderTarget();
    void InitViewport(int width, int height);

    void PreDraw(float clearColor[4]); 
    void Draw();                       
    void PostDraw();                   

private:
    Microsoft::WRL::ComPtr<ID3D11Device> d3d11Device;
    Microsoft::WRL::ComPtr<ID3D11DeviceContext> d3d11DeviceContext;
    Microsoft::WRL::ComPtr<IDXGISwapChain> dxgiSwapChain;
    Microsoft::WRL::ComPtr<ID3D11RenderTargetView> renderTargetView;

    D3D_FEATURE_LEVEL featureLevel;
};
