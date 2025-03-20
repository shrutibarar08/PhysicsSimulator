#include "Utils/Timer.h"
#include <chrono>

Timer::Timer()
    : mDeltaTime(std::chrono::duration<double>(0.0)),
    mbStopped(false),
    mPausedDuration(std::chrono::duration<double>(0.0))
{
    mBaseTime = std::chrono::high_resolution_clock::now();
    mPrevTime = mBaseTime;
}

float Timer::TotalTime() const
{
    if (mbStopped) 
    {
        return std::chrono::duration<float>(mStopTime - mBaseTime - mPausedDuration).count();
    }
    return std::chrono::duration<float>(mCurrTime - mBaseTime - mPausedDuration).count();
}

float Timer::DeltaTime() const
{
    return static_cast<float>(mDeltaTime.count());
}

void Timer::Reset()
{
    auto currTime = std::chrono::high_resolution_clock::now();
    mBaseTime = currTime;
    mPrevTime = currTime;
    mStopTime = {};
    mPausedDuration = std::chrono::duration<double>(0.0);
    mbStopped = false;
}

void Timer::Start()
{
    if (mbStopped) 
    {
        auto startTime = std::chrono::high_resolution_clock::now();
        mPausedDuration += startTime - mStopTime;
        mPrevTime = startTime;
        mbStopped = false;
    }
}

void Timer::Stop()
{
    if (!mbStopped) 
    {
        mStopTime = std::chrono::high_resolution_clock::now();
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
        mDeltaTime = std::chrono::duration<double>(0.0);
        return;
    }

    mCurrTime = std::chrono::high_resolution_clock::now();
    mDeltaTime = mCurrTime - mPrevTime;
    mPrevTime = mCurrTime;

    if (mDeltaTime.count() < 0.0) 
    {
        mDeltaTime = std::chrono::duration<double>(0.0);
    }
}

void Timer::RecordingExecute(float deltaTime)
{
    Tick();
}
