#pragma once
#include "IColliderUI.h"
#include "Collider/PlaneCollider.h"


class PlaneColliderUI final: public IColliderUI
{
public:
	PlaneColliderUI() = default;
	ICollider* Collider() override;
	void OnUpdateGUI() override;
	void LoadFromJson(const nlohmann::json& jsonData) override;
	nlohmann::json SaveToJson() override;

private:
	PlaneCollider mPlaneCollider{};
};
