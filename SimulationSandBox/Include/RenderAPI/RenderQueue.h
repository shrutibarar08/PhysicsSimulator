#pragma once
#include "DefineRender.h"
#include "Core/Interface/ISubsystemInterface.h"
#include "Core/Interface/IEntityInterface.h"

#include <unordered_map>

#include "Core/SystemManager/SubsystemManager.h"
#include "Utils/Timer.h"


class RenderQueue final: public ISubsystemInterface
{
public:
	RenderQueue() = default;
	~RenderQueue() override = default;

	void Initialize() override {}
	void Shutdown() override {}

	static void RegisterObject(IEntityInterface* object)
	{
		if (object == nullptr || object->IsAssignID()) return;

		mCounter++;
		object->SetAssignID(mCounter);
		mObjects[mCounter] = object;
		mObjects[mCounter]->SetupObject();
	}

	static void UnRegisterObject(IEntityInterface* object)
	{
		if (!object->IsAssignID()) return;

		mObjects.erase(object->GetAssignID());
		object->UnAssignID();
	}

	static void RenderObjects(ID3D11DeviceContext* pDeviceContext)
	{
		for (auto& object: mObjects)
		{
			if (object.second != nullptr)
			{
				object.second->RenderObject(pDeviceContext);
			}
		}
	}

	static void UpdateAllConstantBuffer(const Simulation::WorldSpace& space)
	{
		for (auto& object : mObjects)
		{
			if (object.second != nullptr)
			{
				object.second->UpdateBuffer(space);
			}
		}
	}

public:
	inline static std::unordered_map<int, IEntityInterface*> mObjects;
private:
	
	inline static int mCounter = 0;
};
