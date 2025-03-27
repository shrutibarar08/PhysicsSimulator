#pragma once

#include "Core/Interface/IObjectInterface.h"

class TorusObject final : public IObjectInterface
{
public:
    ~TorusObject() override = default;
    void SetupObject() override;

protected:
    std::vector<WORD> BuildIndices() override;
    std::vector<DirectX::XMFLOAT3> BuildPosition() override;
    std::vector<DirectX::XMFLOAT3> BuildNormal() override;
    std::vector<DirectX::XMFLOAT2> BuildTexCoords() override;
    void InitPrimitiveControlGUI() override;

private:
    int mRadialSegments = 40;
    int mTubularSegments = 20;
    float mRadius = 1.0f;
    float mTubeRadius = 0.3f;
};
