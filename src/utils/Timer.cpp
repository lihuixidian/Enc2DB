
#include "Timer.h"


void *startTimer() {

    return new Timer;
}
double getPassedTime(void *timer, bool isDelete)
{
    double ret = ((Timer *)timer)->passedTimeMicroSecond();
    if(isDelete)
        delete (Timer*)timer;
    return ret;
}