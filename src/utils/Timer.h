

#pragma once 

#ifdef __cplusplus
#include "Timestamp.h"
class Timer 
{
    private:
        Timestamp _startTime;
        // Timestamp _lastCount;

    public:
        Timer() : _startTime( Timestamp::now() ){};
        double passedTimeMicroSecond() {
            Timestamp now = Timestamp::now();
            double passedTime = timeDifference(now, _startTime);
            // _startTime = Timestamp::now();
            return passedTime;
        }
};
#endif

#ifdef __cplusplus
extern "C" {
#endif

void* startTimer();
double getPassedTime(void *timer, bool isDelete);

#ifdef __cplusplus
}
#endif