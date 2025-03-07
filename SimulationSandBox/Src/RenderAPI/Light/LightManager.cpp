#include "RenderAPI/Light/LightManager.h"
#include "RenderAPI/RenderManager.h"
#include "RenderAPI/RenderQueue.h"
#include "Core/SystemManager/SubsystemManager.h"
#include "RenderAPI/Light/DefineLight.h"

#include <iostream>
#include <stdexcept>

#include "imgui/imgui.h"

LightManager::LightManager()
{
    ID3D11Device* device = SubsystemManager::Get<RenderManager>()->GetDevice();

    // --- Create Structured Buffer ---
    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.ByteWidth = sizeof(Simulation::SpotLight) * mMaxLights; // Total size
    bufferDesc.Usage = D3D11_USAGE_DYNAMIC; // Allows CPU updates
    bufferDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
    bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE; // CPU can update
    bufferDesc.MiscFlags = D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
    bufferDesc.StructureByteStride = sizeof(Simulation::SpotLight); // Per element size

    HRESULT hr = device->CreateBuffer(&bufferDesc, nullptr, mLightBuffer.GetAddressOf());
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create Light Buffer");
    }

    // --- Create Shader Resource View (SRV) ---
    D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
    srvDesc.Format = DXGI_FORMAT_UNKNOWN; // Structured buffers use UNKNOWN format
    srvDesc.ViewDimension = D3D11_SRV_DIMENSION_BUFFER;
    srvDesc.Buffer.NumElements = mMaxLights;

    hr = device->CreateShaderResourceView(mLightBuffer.Get(), &srvDesc, mLightBufferSRV.GetAddressOf());
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create Light Buffer SRV");
    }

    D3D11_BUFFER_DESC lightCountBufferDesc = {};
    lightCountBufferDesc.Usage = D3D11_USAGE_DYNAMIC;  // Allow CPU updates
    lightCountBufferDesc.ByteWidth = sizeof(LightCountBufferType);
    lightCountBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    lightCountBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    lightCountBufferDesc.StructureByteStride = 0;

    hr = SubsystemManager::Get<RenderManager>()->GetDevice()->CreateBuffer(
        &lightCountBufferDesc, nullptr, &mLightCountBuffer
    );

    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to create LightCount constant buffer!");
    }
}

void LightManager::AddLight(const SIMULATION_LIGHT_CREATE_DESC* desc)
{
    auto light_instance = RegistryLight::Create(desc->LightType);
    light_instance->InitObject(desc);

    if (mState == State::Loaded)
    {
        RenderQueue::RegisterObject(light_instance.get());
        std::cout << "Added Object on Scene!\n";
    }
    mLights.emplace_back(std::move(light_instance));
}

void LightManager::LoadFromJson(const nlohmann::json& jsonData)
{
    for (const auto& lightJson : jsonData)
    {
	    Simulation::Light lightDatta;
        lightDatta.Ambient = {
            lightJson["ambient"][0],
            lightJson["ambient"][1],
            lightJson["ambient"][2],
            lightJson["ambient"][3]
        };
        lightDatta.Diffuse = {
            lightJson["diffuse"][0],
            lightJson["diffuse"][1],
            lightJson["diffuse"][2],
            lightJson["diffuse"][3]
        };
        lightDatta.Specular = {
            lightJson["specular"][0],
            lightJson["specular"][1],
            lightJson["specular"][2],
            lightJson["specular"][3]
        };
        lightDatta.Position = {
            lightJson["position"][0],
            lightJson["position"][1],
            lightJson["position"][2]
        };
        lightDatta.Range = lightJson["range"];
        lightDatta.Direction = {
            lightJson["direction"][0],
            lightJson["direction"][1],
            lightJson["direction"][2]
        };
        lightDatta.Spot = lightJson["spot"];
        lightDatta.Att = {
            lightJson["attenuation"][0],
            lightJson["attenuation"][1],
            lightJson["attenuation"][2]
        };

        SIMULATION_LIGHT_CREATE_DESC desc{};
        desc.LightType = lightJson["light_type"];
        desc.LightName = lightJson["light_name"];
        desc.LightData = lightDatta;
        AddLight(&desc);
    }
}

nlohmann::json LightManager::SaveToJson() const
{
    nlohmann::json jsonData;

    for (const auto& light : mLights)
    {
        nlohmann::json lightJson;

        Simulation::Light lightData = light->GetLightData();
        lightJson["light_type"] = light->GetObjectType();
        lightJson["light_name"] = light->GetObjectName();

        lightJson["ambient"] = { lightData.Ambient.x, lightData.Ambient.y, lightData.Ambient.z, lightData.Ambient.w };
        lightJson["diffuse"] = { lightData.Diffuse.x, lightData.Diffuse.y, lightData.Diffuse.z, lightData.Diffuse.w };
        lightJson["specular"] = { lightData.Specular.x, lightData.Specular.y, lightData.Specular.z, lightData.Specular.w };
        lightJson["position"] = { lightData.Position.x, lightData.Position.y, lightData.Position.z };
        lightJson["range"] = lightData.Range;
        lightJson["direction"] = { lightData.Direction.x, lightData.Direction.y, lightData.Direction.z };
        lightJson["spot"] = lightData.Spot;
        lightJson["attenuation"] = { lightData.Att.x, lightData.Att.y, lightData.Att.z };

        jsonData.push_back(lightJson);
    }
    return jsonData;
}

void LightManager::Load()
{
    if (mState == State::Loaded) return;
    mState = State::Loaded;

    std::cout << "Loaded Lights: " << mLights.size() << "\n";
    for (auto& light: mLights)
    {
        RenderQueue::RegisterObject(light.get());
    }
}

void LightManager::UnLoad()
{
    if (mState == State::UnLoaded) return;
    mState = State::UnLoaded;

    std::cout << "Unloaded Lights: " << mLights.size() << "\n";
    for (auto& light : mLights)
    {
        RenderQueue::UnRegisterObject(light.get());
    }
}

void LightManager::Update(const Simulation::WorldSpace& space)
{
    if (mLights.empty()) return;

    for (auto& light : mLights)
    {
        if (light != nullptr)
			light->UpdateBuffer(space);
    }

    UpdateLightCountBuffer();

    ID3D11DeviceContext* deviceContext = SubsystemManager::Get<RenderManager>()->GetDeviceContext();
    // Convert stored pointers to actual light data
    std::vector<Simulation::Light> lightData;
    lightData.reserve(mLights.size());

    for (auto& light : mLights)
    {
        lightData.push_back(light->GetLightData());
    }

    // Map the buffer
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    HRESULT hr = deviceContext->Map(mLightBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to Map Light Buffer Data!\n");
        return;
    }

    memcpy(mappedResource.pData, lightData.data(), sizeof(Simulation::SpotLight) * lightData.size());
    deviceContext->Unmap(mLightBuffer.Get(), 0);
    deviceContext->PSSetShaderResources(0, 1, mLightBufferSRV.GetAddressOf());
    deviceContext->PSSetConstantBuffers(1, 1, mLightCountBuffer.GetAddressOf());
}

void LightManager::InitUpdateGUI()
{
    for (size_t i = 0; i < mLights.size(); i++)
    {
        ILightInterface* light = mLights[i].get();
        if (!light) continue;

        ImGui::PushID(static_cast<int>(i)); // Assign unique ID

        // Collapsible section for each light
        std::string name = light->GetObjectName() + "\t(" + light->GetObjectType() + ")";
        if (ImGui::CollapsingHeader(name.c_str()))
        {
            light->InitGUI();
        }
        ImGui::PopID();
    }
}

void LightManager::UpdateLightCountBuffer() const
{
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    ID3D11DeviceContext* deviceContext = SubsystemManager::Get<RenderManager>()->GetDeviceContext();

    HRESULT hr = deviceContext->Map(mLightCountBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(hr))
    {
        throw std::runtime_error("Failed to map LightCount buffer!");
    }

    LightCountBufferType* dataPtr = static_cast<LightCountBufferType*>(mappedResource.pData);
    dataPtr->numLights = static_cast<int>(mLights.size()); // Store the active light count
    dataPtr->padding[0] = 0.0f;  // Ensure padding is set correctly
    dataPtr->padding[1] = 0.0f;
    dataPtr->padding[2] = 0.0f;
    deviceContext->Unmap(mLightCountBuffer.Get(), 0);
}
