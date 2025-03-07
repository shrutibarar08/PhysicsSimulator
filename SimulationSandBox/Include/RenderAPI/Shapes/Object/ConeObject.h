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
};
