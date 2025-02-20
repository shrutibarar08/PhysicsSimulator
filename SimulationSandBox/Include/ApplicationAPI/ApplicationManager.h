#pragma once
#include "WindowsAPI/WindowsManager.h"
#include "RenderAPI/RenderManager.h"
#include "Scenarios/PlaneScenario.h"

class ApplicationManager
{
public:
	ApplicationManager(const std::wstring& AppName, int width, int height);
	virtual ~ApplicationManager() = default;

	virtual void BeginPlay();
	virtual void Tick();

	int Update();

private:
	PlaneScenario planeScenario_1{};
	PlaneScenario planeScenario_2{};
};

