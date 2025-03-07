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

private:
    int radialSegments = 40;
    int tubularSegments = 20;
    float radius = 1.0f;
    float tubeRadius = 0.3f;
};
