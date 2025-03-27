#pragma once

#include "RenderAPI/DefineRender.h"
#include <string>
#include <vector>

#include "IEntityInterface.h"
#include "WICTextureLoader.h"
#include "DDSTextureLoader.h"


typedef struct SIMULATION_OBJECT_CREATE_DESC
{
	std::string ObjectName;
	std::string ObjectType;
	std::string TexturePath;
	Simulation::Transform Transform;
} SIMULATION_OBJECT_CREATE_DESC;


class IObjectInterface: public IEntityInterface
{
public:
	IObjectInterface()			= default;
	virtual ~IObjectInterface() override = default;

	IObjectInterface(const IObjectInterface&)			 = default;
	IObjectInterface& operator=(const IObjectInterface&) = default;
	IObjectInterface(IObjectInterface&&)				 = default;
	IObjectInterface& operator=(IObjectInterface&&)		 = default;

	void InitObject(const SIMULATION_OBJECT_CREATE_DESC* objectCreateDesc);

	virtual void SetupObject() override;
	void UpdateBuffer(const Simulation::WorldSpace& worldSpace) override;
	void RenderObject(ID3D11DeviceContext* pDeviceContext) override;

	//~ Getters and Setters
	void SetTransform(const Simulation::Transform& transform);
	Simulation::Transform GetTransform();

	UINT GetIndexCounts() const;

	void InitControlGUI();
	void SetTexture(const std::string& path);
	std::string GetTexturePath() const;

protected:
	void ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);

	virtual std::vector<WORD> BuildIndices() = 0;
	virtual std::vector<DirectX::XMFLOAT3> BuildPosition()	= 0;
	virtual std::vector<DirectX::XMFLOAT3> BuildNormal()	= 0;
	virtual std::vector<DirectX::XMFLOAT2> BuildTexCoords() = 0;
	virtual void InitPrimitiveControlGUI() = 0;

private:
	void ConstructVertices();
	void ConstructIndices();
	void ConstructVertexShader();
	void ConstructPixelShader();
	void ConstructInputLayout();
	void ConstructConstantBuffer();
	void ConstructRasterizationState();
	void ConstructSamplerState();

	void UpdateVertexConstantBuffer(ID3D11DeviceContext* pDeviceContext);

	void InitDataControlGUI();
	void InitTransformControlGUI();

protected:
	D3D11_PRIMITIVE_TOPOLOGY mTopology{ D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP };
	UINT mIndexCounts;

	Simulation::Transform mTransform{};
	Simulation::WorldSpace mWorldSpace;

	Microsoft::WRL::ComPtr<ID3D11InputLayout>		 mInputLayout;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			 mVertexConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>		 mVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>		 mPixelShader;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState>	 mRasterizerState;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			 mVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			 mIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mTextureSRV;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>		 mSamplerState;

	//~ Vertices and Indices
	std::vector<Simulation::Vertex> mVertices;
	std::vector<WORD> mIndices;

	//~ UI
	char muiFilePath[MAX_PATH]{ "" };
	std::string mTexturePath{ "not set" };
	std::string mChangeObjectName;
};
