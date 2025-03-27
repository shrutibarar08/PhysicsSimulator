#pragma once

#include "Core/Interface/IObjectInterface.h"
#include <vector>
#include <DirectXMath.h>

class PyramidObject final : public IObjectInterface
{
public:
    ~PyramidObject() override = default;
    void SetupObject() override;

protected:
    std::vector<WORD> BuildIndices() override;
    std::vector<DirectX::XMFLOAT3> BuildPosition() override;
    std::vector<DirectX::XMFLOAT3> BuildNormal() override;
    std::vector<DirectX::XMFLOAT2> BuildTexCoords() override;
    void InitPrimitiveControlGUI() override;

private:
    float mBaseSize = 1.0f;
    float mHeight = 1.0f;
};
