#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <thread>
#include <vector>

#include "parse.h"
#include "primitives.h"
#include "timer.h"


#define DEFAULT_NUM_THREADS     4
#define MAIN_THREAD             threadsRequested


/* Quality-of-life type definitions */
typedef std::vector<int> Bucket;
typedef std::pair<int, int> Range;


/* Global Concurrency Variables */
static int threadsRequested;
static std::vector<std::thread*> workers;
extern primitive::Bar* bar;
extern primitive::Lock* lk;

/* Global Timing Variables */
static Timer algTimer;


/* Forward Function Declarations */
namespace mythread
{
    void fork(void);
    void join(void);
    void cleanup(void);
    void threadMain(int threadID);
}


namespace mythread
{
    void fork(void)
    {
        // resize the worker vector
        workers.resize(threadsRequested - 1);

        // Create barriers for worker threads and the main thead
        // bar = new std::barrier<>(numThreads + 1);

        for(int threadIdx = 0; threadIdx < threadsRequested - 1; threadIdx++)
        {
            // std::cout << "Created Thread: " << threadIdx << std::endl;
            workers[threadIdx] = new std::thread(threadMain, threadIdx);
        }
    }


    void join(void)
    {
        for(int i = 0; i < threadsRequested - 1; i++)
        {
            // std::cout << "Joined Thread: " << i << std::endl;
            workers[i]->join();
        }
    }


    void cleanup(void)
    {
        // delete bar;

        for(int i = 0; i < threadsRequested - 1; i++)
        {
            // std::cout << "Deleted Thread: " << i << std::endl;
            delete workers[i];
        }

        workers.clear();
    }


    void threadMain(int threadID)
    {
        // Synchronize worker and main threads
        bar->wait();

        if(threadID == MAIN_THREAD)
        {
           algTimer.setStartTime();
        }

        // Distribute ranges for threads to work on
        // Range sortRange = sort::getRangeToSort(threadID);

        // Sort the range
        // sort::quickSort(sortedValues, sortRange.first, sortRange.second);

        bar->wait();

        // Contents of the ranges have been sorted, sort the ranges themselves
        if(threadID == MAIN_THREAD)
        {
            // Sort the now-sorted ranges
            // sort::quickSort(sortedValues, 0, sortedValues->size() - 1);
            algTimer.setEndTime();
        }
    }
}


#endif  /* MAIN_H */