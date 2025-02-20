#pragma once

#include "Core/IObjectInterface.h"
#include "Core/RenderDefine.h"
#include "RenderAPI/RenderManager.h"

#include <memory>


class IScenarioBase
{
public:
	virtual ~IScenarioBase() = default;

	virtual void OnLoad();
	virtual void OnUpdate(){};	// TODO: To be implemented.
	virtual void OnUnload();

	template<class T>
	void AddObjectOnScene(const Render::Transform& transform)
	{
		static_assert(std::is_base_of_v<IObjectInterface, T>, "T must inherit from IObjectInterface");

		auto obj = std::make_unique<T>();
		obj->SetTransform(transform);
		mObjects.emplace_back(std::move(obj));
	}

protected:
	std::vector<std::unique_ptr<IObjectInterface>> mObjects;
};
