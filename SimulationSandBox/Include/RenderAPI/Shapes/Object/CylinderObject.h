#pragma once

#include "Core/Interface/IObjectInterface.h"
#include <vector>
#include <DirectXMath.h>

class CylinderObject final : public IObjectInterface
{
public:
    ~CylinderObject() override = default;
    void SetupObject() override;

protected:
    std::vector<WORD> BuildIndices() override;
    std::vector<DirectX::XMFLOAT3> BuildPosition() override;
    std::vector<DirectX::XMFLOAT3> BuildNormal() override;
    std::vector<DirectX::XMFLOAT2> BuildTexCoords() override;

private:
    int mSegmentCount = 20;  // Number of segments for circular faces
    float mRadius = 1.0f;
    float mHeight = 2.0f;
};
