#pragma once

#include "Core/Interface/IObjectInterface.h"


class HyperboloidObject final : public IObjectInterface
{
public:
    ~HyperboloidObject() override = default;
    void SetupObject() override;

protected:
    std::vector<WORD> BuildIndices() override;
    std::vector<DirectX::XMFLOAT3> BuildPosition() override;
    std::vector<DirectX::XMFLOAT3> BuildNormal() override;
    std::vector<DirectX::XMFLOAT2> BuildTexCoords() override;

private:
    int radialSegments = 40;
    int heightSegments = 20;
    float height = 2.0f;
    float radiusBottom = 1.0f;
    float radiusTop = 0.5f;
};
