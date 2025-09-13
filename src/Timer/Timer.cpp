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

    framesToUpdate = (int)(intervalsSinceLastUpdate / intervalsPerFrame);

    if (intervalsSinceLastUpdate > 0) {
        deltaTime = intervalsSinceLastUpdate / (float)timerFreq.QuadPart;
    }

    if (framesToUpdate != 0) {
        timePrevious.QuadPart += (LONGLONG)(framesToUpdate * intervalsPerFrame);
    }

    return framesToUpdate;
}

std::string Timer::GetTimer() {
    float elapsed = running ? GetElapsedTime() : (float)(stopTime.QuadPart - startTime.QuadPart) / timerFreq.QuadPart;

    int minutes = static_cast<int>(elapsed / 60);
    int seconds = static_cast<int>(elapsed) % 60;

    std::stringstream ss;
    ss << std::setfill('0') << std::setw(2) << minutes << ":"
       << std::setfill('0') << std::setw(2) << seconds;

    return ss.str();
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

float Timer::GetElapsedTime() const {
    if (startTime.QuadPart == 0) return 0.0f;

    if (running) {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return (float)(currentTime.QuadPart - startTime.QuadPart) / timerFreq.QuadPart;
    } else {
        return (float)(stopTime.QuadPart - startTime.QuadPart) / timerFreq.QuadPart;
    }
}