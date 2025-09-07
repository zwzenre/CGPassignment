#pragma once
#include <Windows.h>

class Timer {
public:
    Timer();
    ~Timer();

    void initialization();
    void Init(int fps);
    int FramesToUpdate();
    void tick();
    float getDeltaTimer();
    void reset();

private:
    LARGE_INTEGER timerFreq;
    LARGE_INTEGER timeNow;
    LARGE_INTEGER timePrevious;
    int requestedFPS;
    float intervalsPerFrame;
    float intervalsSinceLastUpdate;
    float deltaTime;
};