#ifndef TIMER_H
#define TIMER_H


#include <ctime>
#include <iostream>


typedef unsigned long long epochTime;


class Timer
{
    private:
       timespec     startTime;
       timespec     endTime;
       epochTime    epochNs;
       epochTime    epochSec;

    public:
        Timer();
        ~Timer();

        void calculateRuntime(void);
        void printRuntime(void);

        inline void setStartTime(void) { clock_gettime(CLOCK_MONOTONIC, &startTime); }
        inline void setEndTime(void) { clock_gettime(CLOCK_MONOTONIC, &endTime); }
        inline epochTime getRuntimeSec(void) const { return epochSec; }
        inline epochTime getRuntimeNS(void) const { return epochNs; }

};

#endif  /* TIMER_H */