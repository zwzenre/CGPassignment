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
    running = false;
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
        if (running) {
            totalTime += deltaTime;
        }
    }

    return framesToUpdate;
}

std::string Timer::GetTimer() {
    float elapsed = running ? GetElapsedTime() : (float)(stopTime.QuadPart - startTime.QuadPart) / timerFreq.QuadPart;

    int minutes = static_cast<int>(elapsed / 60);
    int seconds = static_cast<int>(elapsed) % 60;

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
    if (running) {
        LARGE_INTEGER currentTime;
        QueryPerformanceCounter(&currentTime);
        return (float)(currentTime.QuadPart - startTime.QuadPart) / timerFreq.QuadPart;
    } else {
        return (float)(stopTime.QuadPart - startTime.QuadPart) / timerFreq.QuadPart;
    }
}