#pragma once

#include "Core/Interface/ILightInterface.h"
#include "Core/Interface/IStateInterface.h"

#include <memory>
#include <wrl.h>
#include <nlohmann/json.hpp>


struct LightCountBufferType
{
	int numLights;
	float padding[3];
};

class LightManager final: public IStateManager
{
public:
	LightManager();
	~LightManager() override = default;

	void Load() override;
	void UnLoad() override;
	void Update(const Simulation::WorldSpace& space) override;
	void AddLight(const SIMULATION_LIGHT_CREATE_DESC* desc);
	void LoadFromJson(const nlohmann::json& jsonData) override;
	nlohmann::json SaveToJson() const override;
	void InitUpdateGUI() override;

private:
	void UpdateLightCountBuffer() const;

private:
	std::vector<std::unique_ptr<ILightInterface>>	 mLights;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			 mLightBuffer;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mLightBufferSRV;
	Microsoft::WRL::ComPtr<ID3D11Buffer>			 mLightCountBuffer;

	int mMaxLights{ 100 };
};
