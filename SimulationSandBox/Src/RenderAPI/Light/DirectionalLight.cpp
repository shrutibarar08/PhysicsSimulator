#include "RenderAPI/Light/DirectionalLight.h"

#include "imgui/imgui.h"

DirectionalLight::DirectionalLight()
    : mAmbient(DirectX::XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f))
    , mDiffuse(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
    , mSpecular(DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f))
    , mDirection(DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f))  // Default Direction pointing downward
    , mAttenuate(DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f))    // Optional (you can adjust if necessary)
{
}

void DirectionalLight::SetAmbient(DirectX::XMFLOAT4 value) { mAmbient = value; }
void DirectionalLight::SetDiffuse(DirectX::XMFLOAT4 value) { mDiffuse = value; }
void DirectionalLight::SetSpecular(DirectX::XMFLOAT4 value) { mSpecular = value; }
void DirectionalLight::SetDirection(DirectX::XMFLOAT3 value) { mDirection = value; }
void DirectionalLight::SetAttenuate(DirectX::XMFLOAT3 value) { mAttenuate = value; }

const DirectX::XMFLOAT4& DirectionalLight::GetAmbient() const { return mAmbient; }
const DirectX::XMFLOAT4& DirectionalLight::GetDiffuse() const { return mDiffuse; }
const DirectX::XMFLOAT4& DirectionalLight::GetSpecular() const { return mSpecular; }
const DirectX::XMFLOAT3& DirectionalLight::GetDirection() const { return mDirection; }
const DirectX::XMFLOAT3& DirectionalLight::GetAttenuate() const { return mAttenuate; }

float DirectionalLight::GetRange() const { return mRange; }  // Optional, doesn't need to be used.

void DirectionalLight::RenderObject(ID3D11DeviceContext* deviceContext)
{
    
}

void DirectionalLight::UpdateBuffer(const Simulation::WorldSpace& space)
{
    mLight.Ambient = mAmbient;
    mLight.Diffuse = mDiffuse;
    mLight.Specular = mSpecular;
    mLight.Direction = mDirection;
    mLight.Att = mAttenuate;
    mLight.Range = mRange;
    mLight.Id = static_cast<int>(Simulation::LightType::DirectionalLight);
}

void DirectionalLight::InitGUI()
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

    // Direction
    ImGui::Text("Direction");
    ImGui::DragFloat3("##Direction", reinterpret_cast<float*>(&mDirection), 0.1f);
}

void DirectionalLight::InitLightConfig(const Simulation::Light& lightConfig)
{
    mAttenuate = lightConfig.Att;
    mAmbient = lightConfig.Ambient;
    mDiffuse = lightConfig.Diffuse;
    mDirection = lightConfig.Direction;
    mRange = lightConfig.Range;
    mSpecular = lightConfig.Specular;
}
