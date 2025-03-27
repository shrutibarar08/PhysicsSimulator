#pragma once

#include "Core/Interface/IObjectInterface.h"
#include "Core/Interface/IStateInterface.h"
#include "RenderAPI/DefineRender.h"

#include <memory>
#include <nlohmann/json.hpp>


class ObjectManager: public IStateManager
{
public:
	void Load() override;
	void Update(const Simulation::WorldSpace& space) override;
	void UnLoad() override;

	void AddObjectOnScene(const SIMULATION_OBJECT_CREATE_DESC* objectCreateDesc);
	void AddObjectOnScene(const nlohmann::json& jsonData);

	void LoadFromJson(const nlohmann::json& jsonData) override;
	nlohmann::json SaveToJson() const override;

	//~ Gui
	void InitCreationalGUI() override;
	void InitUpdateGUI() override;

	~ObjectManager() override = default;

private:
	std::vector<std::unique_ptr<IObjectInterface>> mObjects;
};
