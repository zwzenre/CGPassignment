#pragma once
#include <Windows.h>

class Timer {
public:
    void Init(int);
    int FramesToUpdate();
    float GetDeltaTime();

private:
    LARGE_INTEGER timerFreq;
    LARGE_INTEGER timeNow;
    LARGE_INTEGER timePrevious;
    int requestedFPS;
    float intervalsPerFrame;
    float intervalsSinceLastUpdate;
};