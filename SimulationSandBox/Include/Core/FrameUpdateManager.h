#pragma once

#include "IFrame.h"
#include <unordered_map>

/*
 * Update Subsystem Frames based on Dependencies
 */
class FrameUpdateManager
{
public:
	static void ScheduleCallback(IFrameInterface* subsystem);
	static void RecordSetup();
	static void RecordStart();
	static void RecordEnd();
	static void RecordExecute();
private:
	static inline std::unordered_map<IFrameInterface*, bool> mSubsystemWithFrames;
};
