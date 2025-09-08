#include "../Timer/Header/Timer.h"
#include <profileapi.h>

void Timer::Init(int fps)
{
    QueryPerformanceFrequency(&timeFreq);
    QueryPerformanceCounter(&timeNow);
    QueryPerformanceCounter(&timePrevious);

    requestedFPS = fps;

    intervalsPerFrame = (float)(timeFreq.QuadPart / requestedFPS);

}

int Timer::FramesToUpdate()
{
    int framesToUpdate = 0;
    QueryPerformanceCounter(&timeNow);

    //get the delta time
    intervalsSinceLastUpdate = (float)(timeNow.QuadPart - (float)timePrevious.QuadPart);

    framesToUpdate = (int)(intervalsSinceLastUpdate / intervalsPerFrame);

    if(framesToUpdate != 0)
    {
        QueryPerformanceCounter(&timePrevious);
    }


    return framesToUpdate;
}
