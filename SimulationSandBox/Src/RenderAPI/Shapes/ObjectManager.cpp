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

void ObjectManager::LoadFromJson(const nlohmann::json& jsonData)
{
	for (const auto& objJson : jsonData)
	{
		Simulation::Transform transform;
		transform.Translation = {
			objJson["transform"]["translation"][0],
			objJson["transform"]["translation"][1],
			objJson["transform"]["translation"][2]
		};
		transform.Rotation = {
			objJson["transform"]["rotation"][0],
			objJson["transform"]["rotation"][1],
			objJson["transform"]["rotation"][2]
		};
		transform.Scale = {
			objJson["transform"]["scale"][0],
			objJson["transform"]["scale"][1],
			objJson["transform"]["scale"][2]
		};

		SIMULATION_OBJECT_CREATE_DESC desc{};
		desc.Transform = transform;
		desc.ObjectName = objJson["object_name"];
		desc.ObjectType = objJson["object_type"];
		desc.TexturePath = objJson["texture_path"];

		AddObjectOnScene(&desc);
	}
}

nlohmann::json ObjectManager::SaveToJson() const
{
	nlohmann::json scenarioJson = nlohmann::json::array();

	for (const auto& object : mObjects)
	{
		nlohmann::json objJson;
		objJson["object_type"] = object->GetObjectType();
		objJson["object_name"] = object->GetObjectName();
		objJson["texture_path"] = object->GetTexturePath();

		Simulation::Transform transform = object->GetTransform();
		objJson["transform"] = {
			{ "translation", { transform.Translation.x, transform.Translation.y, transform.Translation.z } },
			{ "rotation", { transform.Rotation.x, transform.Rotation.y, transform.Rotation.z } },
			{ "scale", { transform.Scale.x, transform.Scale.y, transform.Scale.z } }
		};
		scenarioJson.push_back(objJson);
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
