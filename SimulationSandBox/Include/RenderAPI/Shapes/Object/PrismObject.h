#pragma once

#include "Core/Interface/IObjectInterface.h"
#include <vector>
#include <DirectXMath.h>


class PrismObject final : public IObjectInterface
{
public:
    ~PrismObject() override = default;
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
    float mBaseWidth = 1.0f;
    float mHeight = 1.0f;
    float mDepth = 1.0f;
};
