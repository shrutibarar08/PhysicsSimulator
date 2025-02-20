#pragma once

#include "Core/IObjectInterface.h"


class CubeObject final: public IObjectInterface
{
public:
	CubeObject() = default;
	~CubeObject() override = default;
	void InitObject() override;
};
