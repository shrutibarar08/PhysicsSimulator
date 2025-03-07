#pragma once

#include "Core/Interface/ILightInterface.h"

class DirectionalLight final : public ILightInterface
{
public:
    DirectionalLight();
    ~DirectionalLight() override = default;

    //~ Setters
    void SetAmbient(DirectX::XMFLOAT4 value);
    void SetDiffuse(DirectX::XMFLOAT4 value);
    void SetSpecular(DirectX::XMFLOAT4 value);
    void SetDirection(DirectX::XMFLOAT3 value);
    void SetAttenuate(DirectX::XMFLOAT3 value);

    //~ Getters
    const DirectX::XMFLOAT4& GetAmbient() const;
    const DirectX::XMFLOAT4& GetDiffuse() const;
    const DirectX::XMFLOAT4& GetSpecular() const;
    const DirectX::XMFLOAT3& GetDirection() const;
    const DirectX::XMFLOAT3& GetAttenuate() const;

    float GetRange() const; // Optional for directional lights (may not be used)

    void SetupObject() override {}
    void RenderObject(ID3D11DeviceContext* deviceContext) override;
    void UpdateBuffer(const Simulation::WorldSpace& space) override;

    void InitGUI() override;
    void InitLightConfig(const Simulation::Light& lightConfig) override;

private:
    DirectX::XMFLOAT4 mAmbient;
    DirectX::XMFLOAT4 mDiffuse;
    DirectX::XMFLOAT4 mSpecular;
    DirectX::XMFLOAT3 mDirection;
    DirectX::XMFLOAT3 mAttenuate;
    float mRange{ 1.f };
};
