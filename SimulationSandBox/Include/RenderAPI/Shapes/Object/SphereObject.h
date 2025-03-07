#pragma once

#include "Core/Interface/IObjectInterface.h"


class SphereObject final: public IObjectInterface
{
public:
	~SphereObject() override = default;
	void SetupObject() override;

protected:
	std::vector<WORD> BuildIndices() override;
	std::vector<DirectX::XMFLOAT3> BuildPosition() override;
	std::vector<DirectX::XMFLOAT3> BuildNormal() override;
	std::vector<DirectX::XMFLOAT2> BuildTexCoords() override;

private:
	int  mLatitudeBands{ 20 };
	int  mLongitudeBands{ 20 };
	float mRadius{ 1.0f };
};
