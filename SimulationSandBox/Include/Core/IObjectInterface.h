#pragma once

#include "Core/RenderDefine.h"
#include <string>
#include <vector>


class IObjectInterface
{
public:
	IObjectInterface()			= default;
	virtual ~IObjectInterface() = default;

	IObjectInterface(const IObjectInterface&)			 = default;
	IObjectInterface& operator=(const IObjectInterface&) = default;
	IObjectInterface(IObjectInterface&&)				 = default;
	IObjectInterface& operator=(IObjectInterface&&)		 = default;

	virtual void InitObject();
	void UpdateSpace(const Render::WorldSpace& worldSpace);
	void RenderObject(ID3D11DeviceContext* pDeviceContext);

	//~ Getters and Setters
	// TODO: Implement this
	void SetTransform(const Render::Transform& transform);
	Render::Transform GetTransform() const;

	UINT GetIndexCounts() const;

	void UnAssignID();
	void SetAssignID(int id);
	bool IsAssignID() const;
	int GetAssignID() const;

protected:
	void ConstructVertexShader(const std::wstring& path);
	void ConstructPixelShader(const std::wstring& path);
	void ConstructInputLayout(const std::wstring& vertexShaderPath, const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT inputCount);
	void ConstructVertices(const std::vector<Render::Vertex>& vertices);
	void ConstructIndices(const std::vector<WORD>& indices);
	void ConstructRasterizationState(const D3D11_RASTERIZER_DESC* pRasterizerDesc);
	void ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

private:
	void ConstructConstantBuffer();
	void UpdateVertexConstantBuffer(ID3D11DeviceContext* pDeviceContext) const;

protected:
	D3D11_PRIMITIVE_TOPOLOGY mTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP };
	UINT mIndexCounts;

	Render::Transform mTransform{};
	Render::WorldSpace mWorldSpace;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		mInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mVertexConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		mVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		mPixelShader;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	mRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			mIndexBuffer;

	//~ Identity
	int mAssignedID{ -1 };
};
