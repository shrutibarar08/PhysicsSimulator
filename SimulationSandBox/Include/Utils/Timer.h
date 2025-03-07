#pragma once

#include "Core/Interface/IFrame.h"
#include "Core/Interface/ISubsystemInterface.h"

class Timer final: public ISubsystemInterface, public IFrameInterface
{
public:
	Timer();
	~Timer() override{}

	//~ Total Time elapsed in secs.
	float TotalTime() const;
	float DeltaTime() const;

	void Start(); // Call when unpaused
	void Stop(); // Call when pause

	void RecordingSetup() override;
	void RecordingExecute(float deltaTime) override;

private:
	void Reset(); // before message loop
	void Tick(); // Call every frame.

private:
	double mSecondsPerCount;
	double mDeltaTime;

	__int64 mBaseTime;
	__int64 mPausedTime;
	__int64 mStopTime;
	__int64 mPrevTime;
	__int64 mCurrTime;

	bool mbStopped;
};
