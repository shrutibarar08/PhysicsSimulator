#pragma once

#include "IColliderUI.h"
#include "Collider/CubeCollider.h"

class CubeColliderUI final : public IColliderUI
{
public:
	CubeColliderUI() = default;

	void Update(float deltaTime) override;
	void ChildUpdateGUI() override;
	void LoadFromJson(const nlohmann::json& jsonData) override;
	nlohmann::json SaveToJson() override;
	ICollider* Collider() override;

private:
	CubeCollider mCubeCollider{};
};
