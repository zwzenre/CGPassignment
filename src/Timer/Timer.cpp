#include "Header/Timer.h"
#include <iostream>
#include <profileapi.h>

void Timer::Init(int fps) {
    QueryPerformanceFrequency(&timerFreq);
    QueryPerformanceCounter(&timeNow);
    QueryPerformanceCounter(&timePrevious);

    requestedFPS = fps;
    intervalsPerFrame = (float)(timerFreq.QuadPart / requestedFPS);
    totalTime = 0.0f;
    deltaTime = 0.0f;
}

int Timer::FramesToUpdate() {
    int framesToUpdate = 0;
    QueryPerformanceCounter(&timeNow);

    intervalsSinceLastUpdate = (float)(timeNow.QuadPart - timePrevious.QuadPart);

    framesToUpdate = (int)(intervalsSinceLastUpdate / intervalsPerFrame);

    if (intervalsSinceLastUpdate > 0) {
        deltaTime = intervalsSinceLastUpdate / (float)timerFreq.QuadPart;
    }

    if (framesToUpdate != 0) {
        timePrevious = timeNow;
    }

    return framesToUpdate;
}

std::string Timer::GetTimer() {
    int minutes = totalTime / 60;
    int seconds = totalTime % 60;

    std::string minStr = (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
    std::string secStr = (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds);

    return minStr + ":" + secStr;
}

void Timer::SetTotalTime(int time) {
    totalTime = time;
}

float Timer::GetDeltaTime() {
    return deltaTime;
}

float Timer::SecondsPerFrame() const {
    return 1.0f / requestedFPS;
}