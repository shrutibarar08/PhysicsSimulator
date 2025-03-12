#include "PhysicsAPI/PhysicsManager.h"
#include <process.h>
#include <thread>

#include <iostream>

void PhysicsManager::Initialize()
{
	Start();
}

void PhysicsManager::Shutdown()
{
	WaitForTermination();
}

unsigned __stdcall PhysicsManager::threadFunc(void* param)
{
	if (param)
	{
		return ((PhysicsManager*)param)->Run();
	}
	return 1;
}

HANDLE PhysicsManager::Start()
{
	unsigned threadID = 0;
	_hThread = (HANDLE)_beginthreadex(
		NULL,
		1024 * 1024,
		threadFunc,
		this,
		0,
		&threadID
	);
	return _hThread;
}

int PhysicsManager::Run()
{
	while (!_terminate)
	{
		Update();
		std::this_thread::sleep_for(std::chrono::milliseconds(25));
	}
	return 1;
}

void PhysicsManager::Update()
{
	std::cout << "Other Thread is running fine!\n";
}

void PhysicsManager::WaitForTermination()
{
	_terminate = true;
	// wait for it to stop
	WaitForSingleObject(_hThread, INFINITE);
	// close thread handle
	CloseHandle(_hThread);
	_hThread = 0;
}
