#pragma once

#include "RenderAPI/CameraComponent.h"
#include "RenderAPI/Light/LightManager.h"
#include "RenderAPI/Shapes/ObjectManager.h"


class Scenario
{
public:
	virtual ~Scenario() = default;

	virtual void OnLoad();
	virtual void OnUpdate(float deltaTime);
	virtual void OnUnload();

	CameraComponent& GetCameraComponent();

	ObjectManager* GetObjectManager();
	LightManager* GetLightManager();

	void LoadFromJson(const nlohmann::json& jsonData);
	nlohmann::json SaveToJson() const;

	bool IsActive() const;

private:
	ObjectManager mObjectManager{};
	CameraComponent mCameraComponent{};
	LightManager mLightManager{};

	enum class State
	{
		Loaded,
		UnLoaded,
	} mState = State::UnLoaded;
};
