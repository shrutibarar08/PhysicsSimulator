#include "RenderAPI/Shapes/ObjectManager.h"
#include "RenderAPI/RenderQueue.h"
#include "Core/RegistryManager/RegistryShape.h"

#include "imgui/imgui.h"
#include <iostream>


void ObjectManager::Load()
{
	if (mState == State::Loaded) return;
	mState = State::Loaded;

	std::cout << "Loading " << mObjects.size() << " Objects onto Simulation Queue!\n";

	//~ Registers (Add) all the Object on Render Queue so that the DirectX can Render it
	for (int i = 0; i < mObjects.size(); i++)
	{
		RenderQueue::RegisterObject(mObjects[i].get());
	}
}

void ObjectManager::UnLoad()
{
	if (mState == State::UnLoaded) return;
	mState = State::UnLoaded;

	std::cout << "Off Loading " << mObjects.size() << " Objects onto Simulation Queue!\n";

	//~ RenderQ bhai jitne object iss specific scene me hai usse render mt kro
	// kyuki scene change ho chuka hai unload hone ke baad
	for (int i = 0; i < mObjects.size(); i++)
	{
		RenderQueue::UnRegisterObject(mObjects[i].get());
	}
}

void ObjectManager::Update(const Simulation::WorldSpace& space)
{
	for (auto& object : mObjects)
	{
		object->UpdateBuffer(space);
	}
}

void ObjectManager::AddObjectOnScene(const SIMULATION_OBJECT_CREATE_DESC* objectCreateDesc)
{
	std::unique_ptr<IObjectInterface> object = RegistryShape::Create(objectCreateDesc->ObjectType);
	object->InitObject(objectCreateDesc);

	if (mState == State::Loaded)
	{
		RenderQueue::RegisterObject(object.get());
		std::cout << "Added Object on Scene!\n";
	}
	mObjects.emplace_back(std::move(object));
}

void ObjectManager::AddObjectOnScene(const nlohmann::json& jsonData)
{
	if (!jsonData.contains("object_type")) return;
	std::unique_ptr<IObjectInterface> object = RegistryShape::Create(jsonData["object_type"]);
	object->LoadFromJson(jsonData);

	if (mState == State::Loaded)
	{
		RenderQueue::RegisterObject(object.get());
		std::cout << "Added Object on Scene!\n";
	}
	mObjects.emplace_back(std::move(object));
}

void ObjectManager::LoadFromJson(const nlohmann::json& jsonData)
{
	for (const auto& objJson : jsonData)
	{
		AddObjectOnScene(objJson);
	}
}

nlohmann::json ObjectManager::SaveToJson() const
{
	nlohmann::json scenarioJson = nlohmann::json::array();

	for (const auto& object : mObjects)
	{
		scenarioJson.emplace_back(std::move(object->SaveToJson()));
	}

	return scenarioJson;
}


void ObjectManager::InitCreationalGUI()
{

}

void ObjectManager::InitUpdateGUI()
{
	for (size_t i = 0; i < mObjects.size(); i++)
	{
		IObjectInterface* object = mObjects[i].get();
		if (!object) continue;
		ImGui::PushID(static_cast<int>(i * 2));
		std::string name = object->GetObjectName() + "\t(" + object->GetObjectType() + ")";

		if (ImGui::CollapsingHeader(name.c_str()))
		{
			object->InitControlGUI();
		}
		ImGui::PopID();
	}
}
