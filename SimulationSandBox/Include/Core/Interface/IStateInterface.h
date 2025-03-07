#pragma once

#include <nlohmann/json.hpp>
#include "RenderAPI/DefineRender.h"


class IStateManager
{
public:
	IStateManager() = default;
	virtual ~IStateManager() = default;

	virtual void Load() {}
	virtual void UnLoad() {}
	virtual void Update(const Simulation::WorldSpace& space) {}

	virtual void LoadFromJson(const nlohmann::json& jsonData) {}
	virtual nlohmann::json SaveToJson() const { return nlohmann::json::array(); }

	virtual void InitCreationalGUI() {}
	virtual void InitUpdateGUI() {}

protected:
	enum class State: uint8_t
	{
		Loaded,
		UnLoaded,
	} mState = State::UnLoaded;
};
