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
    void InitPrimitiveControlGUI() override;

private:
    int mRadialSegments{ 40 };
    int mHeightSegments{ 20 };
    float mHeight{ 2.0f };
    float mRadiusBottom{ 1.0f };
    float mRadiusTop{ 0.5f };
};
