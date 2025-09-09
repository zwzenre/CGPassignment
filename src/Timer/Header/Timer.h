#pragma once
#include <Windows.h>
#include <string>

class Timer {
public:
    void Init(int);
    int FramesToUpdate();
    std::string GetTimer();
    void SetTotalTime(int time);
    float GetDeltaTime();

private:
    LARGE_INTEGER timerFreq;
    LARGE_INTEGER timeNow;
    LARGE_INTEGER timePrevious;
    int requestedFPS;
    float intervalsPerFrame;
    float intervalsSinceLastUpdate;

    int totalTime = 0;
//    float interval = 0;
};