#include "Header/Timer.h"
#include <iostream>
#include <profileapi.h>
#include <iomanip>
#include <sstream>

void Timer::Init(int fps) {
    QueryPerformanceFrequency(&timerFreq);
    QueryPerformanceCounter(&timeNow);
    QueryPerformanceCounter(&timePrevious);

    requestedFPS = fps;
    intervalsPerFrame = (float)(timerFreq.QuadPart / requestedFPS);
    deltaTime = 0.0f;
    running = false;
    startTime.QuadPart = 0;
    stopTime.QuadPart = 0;
}

int Timer::FramesToUpdate() {
    int framesToUpdate = 0;
    QueryPerformanceCounter(&timeNow);

    intervalsSinceLastUpdate = (float)(timeNow.QuadPart - timePrevious.QuadPart);

    deltaTime = intervalsSinceLastUpdate / (float)timerFreq.QuadPart;

    framesToUpdate = (int)(intervalsSinceLastUpdate / intervalsPerFrame);

    if (framesToUpdate != 0) {
        timePrevious.QuadPart += (LONGLONG)(framesToUpdate * intervalsPerFrame);
    }

    return framesToUpdate;
}


float Timer::GetDeltaTime() {
    return deltaTime;
}

float Timer::SecondsPerFrame() const {
    return 1.0f / requestedFPS;
}

void Timer::Start() {
    if (!running) {
        QueryPerformanceCounter(&startTime);
        timePrevious = startTime;
        running = true;
    }
}

void Timer::Stop() {
    if (running) {
        QueryPerformanceCounter(&stopTime);
        running = false;
    }
}