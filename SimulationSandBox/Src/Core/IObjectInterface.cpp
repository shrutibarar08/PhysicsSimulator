#include "Core/Interface/IObjectInterface.h"

#include <iostream>
#include <stdexcept>

#include "Core/SystemManager/SubsystemManager.h"
#include "imgui/imgui.h"

#include "RenderAPI/RenderManager.h"
#include "WindowsAPI/WindowsHelper.h"

void IObjectInterface::RenderObject(ID3D11DeviceContext* pDeviceContext)
{
	//~ Set Input Assembler
	UINT strides = sizeof(Simulation::Vertex);
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
	pDeviceContext->PSSetConstantBuffers(0u, 1u, mVertexConstantBuffer.GetAddressOf());

	//~ Set Other States
	pDeviceContext->RSSetState(mRasterizerState.Get());

	pDeviceContext->PSSetShaderResources(1, 1, mTextureSRV.GetAddressOf());
	pDeviceContext->PSSetSamplers(0, 1, mSamplerState.GetAddressOf());

	//~ Draw Call
	pDeviceContext->DrawIndexed(GetIndexCounts(), 0, 0);
}

void IObjectInterface::SetTransform(const Simulation::Transform& transform)
{
	mTransform = transform;
	if (transform.Translation.x && transform.Translation.y && transform.Translation.z)
		mPhysicsObject.mParticle.Position = transform.Translation;
}

Simulation::Transform IObjectInterface::GetTransform()
{
	mTransform.Translation = mPhysicsObject.mParticle.Position;
	return mTransform;
}

void IObjectInterface::InitObject(const SIMULATION_OBJECT_CREATE_DESC* objectCreateDesc)
{
	SetTransform(objectCreateDesc->Transform);
	SetObjectType(objectCreateDesc->ObjectType);
	SetObjectName(objectCreateDesc->ObjectName);
	SetTexture(objectCreateDesc->TexturePath);
}

void IObjectInterface::SetupObject()
{
	//~ Builds Vertices
	auto pos = BuildPosition();
	auto norm = BuildNormal();
	auto texCoords = BuildTexCoords();
	mVertices.resize(pos.size());

	for (int i = 0; i < pos.size(); i++)
	{
		mVertices[i].position = pos[i];
		mVertices[i].normal = norm[i];
		mVertices[i].texCoordinate = texCoords[i];
	}

	//~ Builds Indices
	mIndices = BuildIndices();

	//~ Construct D3DObjects
	ConstructVertices();
	ConstructIndices();
	ConstructSamplerState();
	ConstructVertexShader();
	ConstructPixelShader();
	ConstructInputLayout();
	ConstructConstantBuffer();
	ConstructRasterizationState();
}

void IObjectInterface::UpdateBuffer(const Simulation::WorldSpace& worldSpace)
{
	mWorldSpace = worldSpace;
}

UINT IObjectInterface::GetIndexCounts() const
{
	return mIndexCounts;
}

void IObjectInterface::InitControlGUI()
{
	if (mChangeObjectName.empty()) mChangeObjectName.resize(150);

	// Input field for object name
	ImGui::InputText("Set Name", mChangeObjectName.data(), mChangeObjectName.size());

	// Apply button
	if (ImGui::Button("Apply"))
	{
		if (mChangeObjectName != mObjectName)
		{
			std::cout << "Object name changed to: " << mChangeObjectName << "\n";
			SetObjectName(mChangeObjectName);
		}
	}

	// --- Particle System Controls ---
	ImGui::Separator();
	ImGui::Text("Particle Controls");

	// Mass
	ImGui::DragFloat("Mass", &mPhysicsObject.mParticle.mMass, 0.1f, 0.001f, 10000.0f);
	mPhysicsObject.mParticle.SetMass(mPhysicsObject.mParticle.mMass);  // Ensure setter is called

	// Velocity
	ImGui::DragFloat3("Velocity", reinterpret_cast<float*>(&mPhysicsObject.mParticle.Velocity), 0.1f);
	mPhysicsObject.mParticle.SetVelocity(mPhysicsObject.mParticle.Velocity);

	// Acceleration
	ImGui::DragFloat3("Acceleration", reinterpret_cast<float*>(&mPhysicsObject.mParticle.Acceleration), 0.1f);
	mPhysicsObject.mParticle.SetAcceleration(mPhysicsObject.mParticle.Acceleration);

	// Accumulated Force
	ImGui::DragFloat3("Force", reinterpret_cast<float*>(&mPhysicsObject.mParticle.AccumulatedForce), 0.1f);

	// --- Transform Controls ---
	ImGui::Separator();
	ImGui::Text("Transform Controls");

	ImGui::Text("Translation");
	ImGui::DragFloat3("##Position", reinterpret_cast<float*>(&mPhysicsObject.mParticle.Position), 0.1f);

	ImGui::Text("Rotation");
	ImGui::DragFloat3("##Rotation", reinterpret_cast<float*>(&mTransform.Rotation), 0.1f);

	ImGui::Text("Scale");
	ImGui::DragFloat3("##Scale", reinterpret_cast<float*>(&mTransform.Scale), 0.1f, 0.1f, 100000.0f);

	// File path selection
	ImGui::InputText("Selected Path", muiFilePath, MAX_PATH, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();

	if (ImGui::Button("Browse Path"))
	{
		std::string selectedFile = OpenFileDialog();
		if (!selectedFile.empty())
		{
			strcpy_s(muiFilePath, selectedFile.c_str());
			SetTexture(selectedFile.c_str());
			std::cout << "Applied texture to Object: " << this << "\n";
		}
	}

	ImGui::Separator();

	mPhysicsObject.InitParticleEffectPopUp();
	mPhysicsObject.InitParticleUpdateGUI();

	mPhysicsObject.InitColliderPopUp();
	mPhysicsObject.InitColliderUpdateGUI();
}

void IObjectInterface::SetTexture(const std::string& path)
{
	if (path == "" || mTexturePath == path) return;

	const std::wstring wpath = std::wstring(path.begin(), path.end());
	HRESULT hr = DirectX::CreateWICTextureFromFile(
		SubsystemManager::Get<RenderManager>()->GetDevice(),
		SubsystemManager::Get<RenderManager>()->GetDeviceContext(),
		wpath.c_str(),
		nullptr,
		&mTextureSRV
	);

	if (FAILED(hr))
	{
		std::cout << "Failed to Build Texture Image!\n";
		return;
	}
	mTexturePath = path;
	std::cout << "Applied new Texture Image!: " << mTexturePath << "\n";
}

void IObjectInterface::ConstructVertexShader()
{
	std::wstring vertexPath = L"Shaders/SimpleVertexShader.cso";
	SubsystemManager::Get<RenderManager>()->BuildVertexShader(vertexPath,
		mVertexShader);
}

void IObjectInterface::ConstructPixelShader()
{
	std::wstring pixelPath = L"Shaders/SimplePixelShader.cso";
	SubsystemManager::Get<RenderManager>()->BuildPixelShader(pixelPath, mPixelShader);
}

void IObjectInterface::ConstructInputLayout()
{
	std::wstring vertexPath = L"Shaders/SimpleVertexShader.cso";

	D3D11_INPUT_ELEMENT_DESC inputDesc[]
	{
		// Position 
		{"POSITION", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u,
			0u, D3D11_INPUT_PER_VERTEX_DATA, 0u},

		// Normal
		{"NORMAL", 0u, DXGI_FORMAT_R32G32B32_FLOAT, 0u,
			12u, D3D11_INPUT_PER_VERTEX_DATA, 0u
		},

		// texture Coordinate
		{"TEXCOORD", 0u, DXGI_FORMAT_R32G32_FLOAT, 0u,
			24u, D3D11_INPUT_PER_VERTEX_DATA, 0u
		},
	};
	UINT inputCounts = ARRAYSIZE(inputDesc);

	SubsystemManager::Get<RenderManager>()->BuildInputLayout(vertexPath,
		inputDesc,
		inputCounts,
		mInputLayout);
}

void IObjectInterface::ConstructVertices()
{
	SubsystemManager::Get<RenderManager>()->BuildVertexBuffer(mVertices, mVertexBuffer);
}

void IObjectInterface::ConstructIndices()
{
	mIndexCounts = static_cast<UINT>(mIndices.size());
	SubsystemManager::Get<RenderManager>()->BuildIndexBuffer(mIndices, mIndexBuffer);
}

void IObjectInterface::ConstructConstantBuffer()
{
	SubsystemManager::Get<RenderManager>()->BuildConstantBuffer(mVertexConstantBuffer);
}

void IObjectInterface::UpdateVertexConstantBuffer(ID3D11DeviceContext* pDeviceContext) const
{
	DirectX::XMMATRIX scaleMat = DirectX::XMMatrixScaling(mTransform.Scale.x, mTransform.Scale.y, mTransform.Scale.z);
	DirectX::XMMATRIX rotMat = DirectX::XMMatrixRotationRollPitchYaw(mTransform.Rotation.x, mTransform.Rotation.y, mTransform.Rotation.z);
	DirectX::XMMATRIX transMat = DirectX::XMMatrixTranslation(
		mPhysicsObject.mParticle.Position.x,
		mPhysicsObject.mParticle.Position.y,
		mPhysicsObject.mParticle.Position.z);

	DirectX::XMMATRIX worldMatrix = scaleMat * rotMat * transMat;

	Simulation::WorldSpace cb;
	cb.World = DirectX::XMMatrixTranspose(worldMatrix);
	cb.View = DirectX::XMMatrixTranspose(mWorldSpace.View);
	cb.Projection = DirectX::XMMatrixTranspose(mWorldSpace.Projection);
	pDeviceContext->UpdateSubresource(mVertexConstantBuffer.Get(), 0, nullptr, &cb, 0, 0);
}

void IObjectInterface::ConstructRasterizationState()
{
	D3D11_RASTERIZER_DESC rasterDesc{};
	rasterDesc.FillMode = D3D11_FILL_SOLID;      // Simulation filled polygons (not wireframe)
	rasterDesc.CullMode = D3D11_CULL_NONE;       // Disable backface culling
	rasterDesc.FrontCounterClockwise = FALSE;    // Standard clockwise winding
	rasterDesc.DepthClipEnable = TRUE;           // Enable depth clipping

	SubsystemManager::Get<RenderManager>()->BuildRasterizerState(&rasterDesc, mRasterizerState);
}

void IObjectInterface::ConstructSamplerState()
{
	D3D11_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	SubsystemManager::Get<RenderManager>()->GetDevice()->CreateSamplerState(&samplerDesc, &mSamplerState);
}

void IObjectInterface::ConstructPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
{
	mTopology = topology;
}

std::string IObjectInterface::GetTexturePath() const
{
	return mTexturePath;
}
