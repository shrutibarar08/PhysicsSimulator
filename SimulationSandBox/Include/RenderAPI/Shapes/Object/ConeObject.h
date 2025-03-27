#pragma once

#include "Core/Interface/IObjectInterface.h"

class ConeObject final : public IObjectInterface
{
public:
    ~ConeObject() override = default;
    void SetupObject() override;

protected:
    std::vector<WORD> BuildIndices() override;
    std::vector<DirectX::XMFLOAT3> BuildPosition() override;
    std::vector<DirectX::XMFLOAT3> BuildNormal() override;
    std::vector<DirectX::XMFLOAT2> BuildTexCoords() override;
    void InitPrimitiveControlGUI() override;
    void LoadParamFromJson(const nlohmann::json& json) override;
    nlohmann::json SaveParamToJson() override;

private:
    int mSlices{ 20 };
    float mRadius{ 1.0f };
    float mHeight{ 2.0f };
};
