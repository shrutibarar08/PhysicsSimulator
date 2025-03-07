#include "RenderAPI/Light/PointLight.h"

#include <iostream>

#include "imgui/imgui.h"


PointLight::PointLight()
    : mAmbient(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f))
    , mDiffuse(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
    , mSpecular(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
    , mPosition(DirectX::XMFLOAT3(10.0f, 10.0f, 0.0f))  
    , mAttenuate(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f))
{}

void PointLight::SetAmbient(DirectX::XMFLOAT4 value) { mAmbient = value; }
void PointLight::SetDiffuse(DirectX::XMFLOAT4 value) { mDiffuse = value; }
void PointLight::SetSpecular(DirectX::XMFLOAT4 value) { mSpecular = value; }
void PointLight::SetPosition(DirectX::XMFLOAT3 value) { mPosition = value; }
void PointLight::SetAttenuate(DirectX::XMFLOAT3 value) { mAttenuate = value; }
void PointLight::SetRange(float value) { mRange = value; }

const DirectX::XMFLOAT4& PointLight::GetAmbient() const { return mAmbient; }
const DirectX::XMFLOAT4& PointLight::GetDiffuse() const { return mDiffuse; }
const DirectX::XMFLOAT4& PointLight::GetSpecular() const { return mSpecular; }
const DirectX::XMFLOAT3& PointLight::GetPosition() const { return mPosition; }
const DirectX::XMFLOAT3& PointLight::GetAttenuate() const { return mAttenuate; }

float PointLight::GetRange() const { return mRange; }

void PointLight::RenderObject(ID3D11DeviceContext* deviceContext)
{
    
}

void PointLight::UpdateBuffer(const Simulation::WorldSpace& space)
{
    mLight.Ambient = mAmbient;
    mLight.Diffuse = mDiffuse;
    mLight.Specular = mSpecular;
    mLight.Position = mPosition;
    mLight.Att = mAttenuate;
    mLight.Range = mRange;
    mLight.Id = static_cast<int>(Simulation::LightType::PointLight);
}

void PointLight::InitGUI()
{
    ILightInterface::InitGUI();

    // Ambient Light Control
    ImGui::Text("Ambient");
    ImGui::ColorEdit4("##Ambient", reinterpret_cast<float*>(&mAmbient));

    // Diffuse Light Control
    ImGui::Text("Diffuse");
    ImGui::ColorEdit4("##Diffuse", reinterpret_cast<float*>(&mDiffuse));

    // Specular Light Control
    ImGui::Text("Specular");
    ImGui::ColorEdit4("##Specular", reinterpret_cast<float*>(&mSpecular));

    // Position
    ImGui::Text("Position");
    ImGui::DragFloat3("##Position", reinterpret_cast<float*>(&mPosition), 0.1f);

    // Range
    ImGui::Text("Range");
    ImGui::DragFloat("##Range", &mRange, 0.1f, 0.0f, 100.0f);

    // Attenuation
    ImGui::Text("Attenuation");
    ImGui::DragFloat3("##Attenuation", reinterpret_cast<float*>(&mAttenuate), 0.01f, 0.0f, 1.0f);
}

void PointLight::InitLightConfig(const Simulation::Light& lightConfig)
{
    mPosition = lightConfig.Position;
    mRange = lightConfig.Range;
    mSpecular = lightConfig.Specular;
    mAmbient = lightConfig.Ambient;
    mDiffuse = lightConfig.Diffuse;
    mAttenuate = lightConfig.Att;
}
