#pragma once
#include <nlohmann/json.hpp>


class IPhysicsUI
{
public:
	virtual void OnUpdateGUI() = 0;
	virtual void LoadFromJson(const nlohmann::json& jsonData) = 0;
	virtual nlohmann::json SaveToJson() = 0;

	virtual ~IPhysicsUI() = default;
};
