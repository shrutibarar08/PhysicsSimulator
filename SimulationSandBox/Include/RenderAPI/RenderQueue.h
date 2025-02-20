#pragma once
#include "Core/RenderDefine.h"
#include "Core/ISubsystemInterface.h"
#include "Core/IObjectInterface.h"

#include <unordered_map>


class RenderQueue final: public ISubsystemInterface
{
public:
	RenderQueue() = default;
	~RenderQueue() override = default;

	void Initialize() override {}
	void Shutdown() override {}

	static void RegisterObject(IObjectInterface* object)
	{
		if (object->IsAssignID()) return;

		mCounter++;
		object->SetAssignID(mCounter);
		mObjects[mCounter] = object;
		mObjects[mCounter]->InitObject();
	}

	static void UnRegisterObject(IObjectInterface* object)
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
				pDeviceContext->DrawIndexed(object.second->GetIndexCounts(), 0u, 0u);
			}
		}
	}

	static void UpdateAllConstantBuffer(const Render::WorldSpace& space)
	{
		for (auto& object : mObjects)
		{
			if (object.second != nullptr)
			{
				object.second->UpdateSpace(space);
			}
		}
	}

private:
	inline static std::unordered_map<int, IObjectInterface*> mObjects;
	inline static int mCounter = 0;
};
