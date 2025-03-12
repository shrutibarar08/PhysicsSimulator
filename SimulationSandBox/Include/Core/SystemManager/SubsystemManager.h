#pragma once

#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

#include "Core/Interface/IFrame.h"
#include "Core/Interface/IObjectInterface.h"
#include "Core/SystemManager/FrameUpdateManager.h"
#include "RenderAPI/RenderQueue.h"

/// <summary>
/// Manages All Subsystem Life Cycle
/// </summary>
class SubsystemManager
{
public:
	template<typename T, bool thread = false, typename... Args>
	static void Register(Args&&... args)
	{
		const std::string name = typeid(T).name();
		auto ptr = std::make_unique<T>(std::forward<Args>(args)...);

		if (ISubsystemInterface* subsystem = dynamic_cast<ISubsystemInterface*>(ptr.get()))
		{
			mSubsystems[name] = std::move(ptr);
			mSubsystems[name]->Initialize();

			if (IFrameInterface* frame = dynamic_cast<IFrameInterface*>(mSubsystems[name].get()))
			{
				FrameUpdateManager::ScheduleCallback(frame);
			}
		}
	}
	
	template<typename T>
	static T* Get()
	{
		const std::string name = typeid(T).name();
		auto it = mSubsystems.find(name);

		return (it != mSubsystems.end()) ? static_cast<T*>(it->second.get()) : nullptr;
	}

	static void ShutdownAll()
	{
		for (auto& [name, subsystem]: mSubsystems)
		{
			subsystem->Shutdown();
		}
		mSubsystems.clear();
	}

private:
	static inline std::unordered_map<std::string, std::unique_ptr<ISubsystemInterface>> mSubsystems;
};

