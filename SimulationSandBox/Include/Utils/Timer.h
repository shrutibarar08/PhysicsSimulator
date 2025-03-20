#pragma once

#include "Core/Interface/IFrame.h"
#include "Core/Interface/ISubsystemInterface.h"
#include <chrono>

class Timer final : public ISubsystemInterface, public IFrameInterface
{
public:
    Timer();
    ~Timer() override = default;

    // Total time elapsed in seconds.
    float TotalTime() const;
    float DeltaTime() const;

    void Start(); // Call when unpaused
    void Stop();  // Call when paused

    void RecordingSetup() override;
    void RecordingExecute(float deltaTime) override;

private:
    void Reset(); // Before message loop
    void Tick();  // Call every frame
private:
    std::chrono::high_resolution_clock::time_point mBaseTime;
    std::chrono::high_resolution_clock::time_point mStopTime;
    std::chrono::high_resolution_clock::time_point mPrevTime;
    std::chrono::high_resolution_clock::time_point mCurrTime;

    std::chrono::duration<double> mDeltaTime;
    std::chrono::duration<double> mPausedDuration;
    bool mbStopped;
};
