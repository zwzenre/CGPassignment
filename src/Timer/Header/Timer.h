#pragma once
#include <Windows.h>
#include <string>

class Timer {
public:
    void Init(int fps);
    int FramesToUpdate();
    std::string GetTimer();
    void SetTotalTime(int time);
    float GetDeltaTime();
    float SecondsPerFrame() const;
    void Start();
    void Stop();
    float GetElapsedTime() const;

private:
    LARGE_INTEGER timerFreq;
    LARGE_INTEGER timeNow;
    LARGE_INTEGER timePrevious;
    int requestedFPS;
    float intervalsPerFrame;
    float intervalsSinceLastUpdate;
    float deltaTime;

    LARGE_INTEGER startTime;
    LARGE_INTEGER stopTime;
    bool running = false;
    int totalTime = 0;
};