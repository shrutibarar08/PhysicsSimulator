#pragma once

#include "Core/Interface/ILightInterface.h"
#include "wrl.h"


class PointLight final : public ILightInterface
{
public:
    PointLight();
    ~PointLight() override = default;

    //~ Setters
    void SetAmbient(DirectX::XMFLOAT4 value);
    void SetDiffuse(DirectX::XMFLOAT4 value);
    void SetSpecular(DirectX::XMFLOAT4 value);
    void SetPosition(DirectX::XMFLOAT3 value);
    void SetAttenuate(DirectX::XMFLOAT3 value);
    void SetRange(float value);

    //~ Getters
    const DirectX::XMFLOAT4& GetAmbient() const;
    const DirectX::XMFLOAT4& GetDiffuse() const;
    const DirectX::XMFLOAT4& GetSpecular() const;
    const DirectX::XMFLOAT3& GetPosition() const;
    const DirectX::XMFLOAT3& GetAttenuate() const;

    float GetRange() const;

    void SetupObject() override {}
    void RenderObject(ID3D11DeviceContext* deviceContext) override;
    void UpdateBuffer(const Simulation::WorldSpace& space) override;

    void InitGUI() override;
    void InitLightConfig(const Simulation::Light& lightConfig) override;

private:
    DirectX::XMFLOAT4 mAmbient;
    DirectX::XMFLOAT4 mDiffuse;
    DirectX::XMFLOAT4 mSpecular;
    DirectX::XMFLOAT3 mPosition;
    DirectX::XMFLOAT3 mAttenuate;
    float mRange{ 10.f };  // Radius of the point light's influence
};
