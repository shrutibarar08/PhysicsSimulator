#include "Core/IObjectInterface.h"
#include "Core/SubsystemManager.h"

#include "RenderAPI/RenderManager.h"

void IObjectInterface::RenderObject(ID3D11DeviceContext* pDeviceContext)
{
	//~ Set Input Assembler
	UINT strides = sizeof(Render::Vertex);
	UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(0u, 1u, mVertexBuffer.GetAddressOf(), &strides, &offset);
	pDeviceContext->IASetIndexBuffer(mIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
	pDeviceContext->IASetInputLayout(mInputLayout.Get());
	pDeviceContext->IASetPrimitiveTopology(mTopology);

	//~ Set Shaders
	UpdateVertexConstantBuffer(pDeviceContext);
	pDeviceContext->VSSetShader(mVertexShader.Get(), nullptr, 0u);
	pDeviceContext->VSSetConstantBuffers(0u, 1u, mVertexConstantBuffer.GetAddressOf());
	pDeviceContext->PSSetShader(mPixelShader.Get(), nullptr, 0u);

	//~ Set Other States
	pDeviceContext->RSSetState(mRasterizerState.Get());
}

void IObjectInterface::SetTransform(const Render::Transform& transform)
{
	mTransform = transform;
}

Render::Transform IObjectInterface::GetTransform() const
{
	return mTransform;
}

void IObjectInterface::InitObject()
{
	ConstructConstantBuffer();
}

void IObjectInterface::UpdateSpace(const Render::WorldSpace& worldSpace)
{
	mWorldSpace = worldSpace;
}

UINT IObjectInterface::GetIndexCounts() const
{
	return mIndexCounts;
}

void IObjectInterface::UnAssignID()
{
	mAssignedID = -1;
}

void IObjectInterface::SetAssignID(int id)
{
	mAssignedID = id;
}

bool IObjectInterface::IsAssignID() const
{
	return mAssignedID != -1;
}

int IObjectInterface::GetAssignID() const
{
	return mAssignedID;
}

void IObjectInterface::ConstructVertexShader(const std::wstring& path)
{
	SubsystemManager::Get<RenderManager>()->BuildVertexShader(path, mVertexShader);
}

void IObjectInterface::ConstructPixelShader(const std::wstring& path)
{
	SubsystemManager::Get<RenderManager>()->BuildPixelShader(path, mPixelShader);
}

void IObjectInterface::ConstructInputLayout(const std::wstring& vertexShaderPath,
	const D3D11_INPUT_ELEMENT_DESC* pInputElementDesc, UINT inputCount)
{
	SubsystemManager::Get<RenderManager>()->BuildInputLayout(vertexShaderPath,
		pInputElementDesc,
		inputCount,
		mInputLayout);
}

void IObjectInterface::ConstructVertices(const std::vector<Render::Vertex>& vertices)
{
	SubsystemManager::Get<RenderManager>()->BuildVertexBuffer(vertices, mVertexBuffer);
}

void IObjectInterface::ConstructIndices(const std::vector<WORD>& indices)
{
	mIndexCounts = static_cast<UINT>(indices.size());
	SubsystemManager::Get<RenderManager>()->BuildIndexBuffer(indices, mIndexBuffer);
}

void IObjectInterface::ConstructConstantBuffer()
{
	SubsystemManager::Get<RenderManager>()->BuildConstantBuffer(mVertexConstantBuffer);
}

void IObjectInterface::UpdateVertexConstantBuffer(ID3D11DeviceContext* pDeviceContext) const
{
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(mTransform.Scale.x, mTransform.Scale.y, mTransform.Scale.z);
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(mTransform.Rotation.x, mTransform.Rotation.y, mTransform.Rotation.z);
	DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslation(mTransform.Translation.x, mTransform.Translation.y, mTransform.Translation.z);

	DirectX::XMMATRIX worldMatrix = scaleMat * rotMat * transMat;

	Render::WorldSpace cb;
	cb.World = DirectX::XMMatrixTranspose(worldMatrix);
	cb.View = DirectX::XMMatrixTranspose(mWorldSpace.View);
	cb.Projection = DirectX::XMMatrixTranspose(mWorldSpace.Projection);
	pDeviceContext->UpdateSubresource(mVertexConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
}

void IObjectInterface::ConstructRasterizationState(const D3D11_RASTERIZER_DESC* pRasterizerDesc)
{
	SubsystemManager::Get<RenderManager>()->BuildRasterizerState(pRasterizerDesc, mRasterizerState);
}

void IObjectInterface::ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	mTopology = topology;
}
