#pragma once

#include "IEntityInterface.h"


typedef struct SIMULATION_LIGHT_CREATE_DESC
{
	std::string LightName;
	std::string LightType;
	Simulation::Light LightData;
} SIMULATION_LIGHT_CREATE_DESC;


class ILightInterface: public IEntityInterface
{
public:
	ILightInterface() = default;
	virtual ~ILightInterface() override = default;

	ILightInterface(const ILightInterface&) = default;
	ILightInterface(ILightInterface&&) = default;
	ILightInterface& operator=(const ILightInterface&) = default;
	ILightInterface& operator=(ILightInterface&&) = default;

	void InitObject(const SIMULATION_LIGHT_CREATE_DESC* desc);
	void SetupObject() override{}
	void RenderObject(ID3D11DeviceContext* deviceContext) override{}
	void UpdateBuffer(const Simulation::WorldSpace& space) override{}

	const Simulation::Light& GetLightData() const
	{
		return mLight;
	}

	virtual void InitLightConfig(const Simulation::Light& lightConfig) = 0;
	virtual void InitGUI();

protected:
	Simulation::Light mLight{};

private:
	//~ UI
	std::string mChangeLightName;
};
