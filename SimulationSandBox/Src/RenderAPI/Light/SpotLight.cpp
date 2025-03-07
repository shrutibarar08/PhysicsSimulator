#include "RenderAPI/Light/SpotLight.h"

#include <iostream>

#include "imgui/imgui.h"
#include "RenderAPI/RenderManager.h"
#include "RenderAPI/RenderQueue.h"

// TODO: Create Light Manager for this.

SpotLight::SpotLight()
    : mAmbient(0.2f, 0.2f, 0.2f, 1.0f),
    mDiffuse(1.0f, 1.0f, 1.0f, 1.0f),
    mSpecular(1.0f, 1.0f, 1.0f, 1.0f),
    mPosition(0.0f, 5.0f, -5.0f),
    mDirection(0.0f, -0.5f, 1.0f),
    mAttenuate(1.0f, 0.1f, 0.01f),
    mRange(125.0f),
    mSpot(30.0f)
{}

// Setters
void SpotLight::SetAmbient(const DirectX::XMFLOAT4 value)   { mAmbient = value; }
void SpotLight::SetDiffuse(const DirectX::XMFLOAT4 value)   { mDiffuse = value; }
void SpotLight::SetSpecular(const DirectX::XMFLOAT4 value)  { mSpecular = value; }
void SpotLight::SetPosition(const DirectX::XMFLOAT3 value)  { mPosition = value; }
void SpotLight::SetDirection(const DirectX::XMFLOAT3 value) { mDirection = value; }
void SpotLight::SetAttenuate(const DirectX::XMFLOAT3 value) { mAttenuate = value; }
void SpotLight::SetRange(float value)                       { mRange = value; }
void SpotLight::SetSpot(float value)                        { mSpot = value; }

// Getters
const DirectX::XMFLOAT4& SpotLight::GetAmbient() const   { return mAmbient; }
const DirectX::XMFLOAT4& SpotLight::GetDiffuse() const   { return mDiffuse; }
const DirectX::XMFLOAT4& SpotLight::GetSpecular() const  { return mSpecular; }
const DirectX::XMFLOAT3& SpotLight::GetPosition() const  { return mPosition; }
const DirectX::XMFLOAT3& SpotLight::GetDirection() const { return mDirection; }
const DirectX::XMFLOAT3& SpotLight::GetAttenuate() const { return mAttenuate; }
float SpotLight::GetRange() const { return mRange; }
float SpotLight::GetSpot() const  { return mSpot; }

void SpotLight::RenderObject(ID3D11DeviceContext* deviceContext)
{

}

void SpotLight::UpdateBuffer(const Simulation::WorldSpace& space)
{
    mLight.Ambient = mAmbient;
    mLight.Diffuse = mDiffuse;
    mLight.Specular = mSpecular;
    mLight.Position = mPosition;
    mLight.Direction = mDirection;
    mLight.Att = mAttenuate;
    mLight.Range = mRange;
    mLight.Spot = mSpot;
    mLight.Id = static_cast<int>(Simulation::LightType::Spotlight);
}

void SpotLight::InitGUI()
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

    // Attenuation
    ImGui::Text("Attenuation");
    ImGui::DragFloat3("##Attenuation", reinterpret_cast<float*>(&mAttenuate), 0.01f, 0.0f, 1.0f);
}

void SpotLight::InitLightConfig(const Simulation::Light& lightConfig)
{
    mPosition = lightConfig.Position;
    mAmbient = lightConfig.Ambient;
    mAttenuate = lightConfig.Att;
    mDiffuse = lightConfig.Diffuse;
    mDirection = lightConfig.Direction;
    mRange = lightConfig.Range;
    mSpecular = lightConfig.Specular;
    mSpot = lightConfig.Spot;
}
