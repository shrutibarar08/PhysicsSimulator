#pragma once

#include "Core/IFrame.h"
#include "Core/ISubsystemInterface.h"
#include "Core/RenderDefine.h"

#include <dxgi.h>
#include <string>
#include <vector>



class RenderManager final: public ISubsystemInterface, public IFrameInterface
{
	//~ DEPENDS UPON WINDOWS MANAGER
public:
	RenderManager() = default;
	~RenderManager() override;

	RenderManager(const RenderManager&) = delete;
	RenderManager& operator=(const RenderManager&) = delete;
	RenderManager(RenderManager&&) = delete;
	RenderManager& operator=(RenderManager&&) = delete;

	//~ Subsystem Interface
	void Initialize() override;
	void Shutdown() override;

	//~ IFrame Interface
	void RecordingSetup() override;
	void RecordingBegin() override;
	void RecordingExecute() override;
	void RecordingEnd() override;

	//~ Builders
	void BuildVertexShader(const std::wstring& path, Microsoft::WRL::ComPtr<ID3D11VertexShader>& pVertexShader) const;
	void BuildPixelShader(const std::wstring& path, Microsoft::WRL::ComPtr<ID3D11PixelShader>& pPixelShader) const;
	void BuildInputLayout(const std::wstring& VertBlobPath, const D3D11_INPUT_ELEMENT_DESC* pInputElements, UINT inputElementSize, Microsoft::WRL::ComPtr<ID3D11InputLayout>& pInputLayout) const;
	void BuildVertexBuffer(const std::vector<Render::Vertex>& vertices, Microsoft::WRL::ComPtr<ID3D11Buffer>& pBuffer) const;
	void BuildIndexBuffer(const std::vector<WORD>& indices, Microsoft::WRL::ComPtr<ID3D11Buffer>& pBuffer) const;
	void BuildConstantBuffer(Microsoft::WRL::ComPtr<ID3D11Buffer>& pBuffer) const;
	void BuildRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, Microsoft::WRL::ComPtr<ID3D11RasterizerState>& pRasterizerState) const;

private:
	void InitDeviceAndSwapChain();
	void InitRenderTargetAndDepthStencil();
	void InitViewport() const;

	static void GetBlob(const std::wstring& path, Microsoft::WRL::ComPtr<ID3DBlob>& blob);

private:
	Microsoft::WRL::ComPtr<ID3D11Device>			md3dDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>		md3dDeviceContext;
	Microsoft::WRL::ComPtr<IDXGISwapChain>			mdxgiSwapChain;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	mRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	mDepthStencilView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mRenderTargetBuffer;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>			mDepthStencilBuffer;

	Render::WorldSpace mWorldPosition;
};
