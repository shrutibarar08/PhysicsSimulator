#include "Core/FrameUpdateManager.h"


void FrameUpdateManager::ScheduleCallback(IFrameInterface* subsystem)
{
	if (mSubsystemWithFrames.find(subsystem) == mSubsystemWithFrames.end())
	{
		mSubsystemWithFrames[subsystem] = true;
	}
}

void FrameUpdateManager::RecordSetup()
{
	for (auto& i : mSubsystemWithFrames)
	{
		i.first->RecordingSetup();
	}
}

void FrameUpdateManager::RecordStart()
{
	for (auto& i: mSubsystemWithFrames)
	{
		i.first->RecordingBegin();
	}
}

void FrameUpdateManager::RecordEnd()
{
	for (auto& i : mSubsystemWithFrames)
	{
		i.first->RecordingEnd();
	}
}

void FrameUpdateManager::RecordExecute()
{
	for (auto i : mSubsystemWithFrames)
	{
		i.first->RecordingExecute();
	}
}
