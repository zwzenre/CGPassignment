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
}

int Timer::FramesToUpdate() {
    int framesToUpdate = 0;
    QueryPerformanceCounter(&timeNow);

    // Get delta time
    intervalsSinceLastUpdate = (float)timeNow.QuadPart - (float)timePrevious.QuadPart;

    framesToUpdate = (int)(intervalsSinceLastUpdate / intervalsPerFrame);

    if (framesToUpdate != 0)
        QueryPerformanceCounter(&timePrevious);

    return framesToUpdate;
}

std::string Timer::GetTimer() {
    int minutes = totalTime / 60;
    int seconds = totalTime % 60;

//    interval += 0.5;
//
//    if ((int)interval % 2 == 0) totalTime++;

    std::string minStr = (minutes < 10) ? "0" + std::to_string(minutes) : std::to_string(minutes);
    std::string secStr = (seconds < 10) ? "0" + std::to_string(seconds) : std::to_string(seconds);

    return minStr + ":" + secStr;
}

void Timer::SetTotalTime(int time) {
    totalTime = time;
}
