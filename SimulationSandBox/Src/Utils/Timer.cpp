#include "Utils/Timer.h"
#include <Windows.h>

Timer::Timer():
mSecondsPerCount(0.0), mDeltaTime(-1.0),
mBaseTime(0), mPausedTime(0),
mPrevTime(0), mCurrTime(0),
mbStopped(false), mStopTime(0)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	mSecondsPerCount = 1.0 / (double)countsPerSec;
}

float Timer::TotalTime() const
{

	if (mbStopped)
	{
		return (float)(((mStopTime - mPausedTime) -
			mBaseTime) * mSecondsPerCount);
	}

	else
	{
		return (float)(((mCurrTime - mPausedTime) -
			mBaseTime) * mSecondsPerCount);
	}
}

float Timer::DeltaTime() const
{
	return static_cast<float>(mDeltaTime);
}

void Timer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mBaseTime = currTime;
	mPrevTime = currTime;
	mStopTime = 0;
	mbStopped = false;
}

void Timer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (mbStopped)
	{
		mPausedTime += (startTime - mStopTime);
		mPrevTime = startTime;
		mStopTime = 0;
		mbStopped = false;
	}
}

void Timer::Stop()
{
	if (!mbStopped)
	{
		__int64 currTime;
		QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
		mStopTime = currTime;
		mbStopped = true;
	}
}

void Timer::RecordingSetup()
{
	Reset();
}

void Timer::Tick()
{
	if (mbStopped)
	{
		mDeltaTime = 0.0;
		return;
	}
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
	mCurrTime = currTime;
	mDeltaTime = (mCurrTime - mPrevTime) * mSecondsPerCount;
	mPrevTime = mCurrTime;
	if (mDeltaTime < 0.0)
	{
		mDeltaTime = 0.0;
	}
}

void Timer::RecordingExecute()
{
	Tick();
}
