#pragma once

#include <Windows.h>
#include "Core/Interface/ISubsystemInterface.h"


class PhysicsManager final: public ISubsystemInterface
{
public:
	void Initialize() override;
	void Shutdown() override;

private:
	static unsigned __stdcall threadFunc(void* param);
	HANDLE Start();
	int Run();
	void Update();
	void WaitForTermination();

private:
	HANDLE _hThread;
	bool _terminate{ false };
};
