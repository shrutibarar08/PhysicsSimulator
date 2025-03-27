#pragma once

#include "IColliderUI.h"
#include "Collider/SphereCollider.h"

class SphereColliderUI final: public IColliderUI
{
public:
	SphereColliderUI() = default;

	void OnUpdateGUI() override;
	void LoadFromJson(const nlohmann::json& jsonData) override;
	nlohmann::json SaveToJson() override;
	ICollider* Collider() override;

private:
	SphereCollider mSphereCollider{};
};
