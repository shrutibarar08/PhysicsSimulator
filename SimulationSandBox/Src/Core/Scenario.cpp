#include "Scenarios/Scenario.h"

void Scenario::OnLoad()
{
	if (mState == State::Loaded) return;
	mState = State::Loaded;
	
	mObjectManager.Load();
	mLightManager.Load();
}

void Scenario::OnUpdate(float deltaTime)
{
	mCameraComponent.Update(deltaTime);
	mLightManager.Update(mCameraComponent.GetCameraSpace());
	mObjectManager.Update(mCameraComponent.GetCameraSpace());
}

void Scenario::OnUnload()
{
	if (mState == State::UnLoaded) return;
	mState = State::UnLoaded;

	mObjectManager.UnLoad();
	mLightManager.UnLoad();
}

CameraComponent& Scenario::GetCameraComponent()
{
	return mCameraComponent;
}

ObjectManager* Scenario::GetObjectManager()
{
	return &mObjectManager;
}

LightManager* Scenario::GetLightManager()
{
	return &mLightManager;
}

void Scenario::LoadFromJson(const nlohmann::json& jsonData)
{
	mObjectManager.LoadFromJson(jsonData["Objects"]);
	mLightManager.LoadFromJson(jsonData["Lights"]);
}

nlohmann::json Scenario::SaveToJson() const
{
	nlohmann::json data;
	data["Objects"] = mObjectManager.SaveToJson();
	data["Lights"] = mLightManager.SaveToJson();
	return data;
}

bool Scenario::IsActive() const
{
	return mState == State::Loaded;
}
