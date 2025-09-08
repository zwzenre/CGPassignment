#pragma once
#include <Windows.h>

class Timer
{
public:
    void Init(int);
    int FramesToUpdate();

private:
    LARGE_INTEGER timeFreq;
    LARGE_INTEGER timeNow;
    LARGE_INTEGER timePrevious;
    int requestedFPS;
    float intervalsPerFrame;
    float intervalsSinceLastUpdate;

};

