#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <thread>
#include <vector>

#include "parse.h"
#include "primitives.h"


#define DEFAULT_NUM_THREADS     4
#define MAIN_THREAD             numThreads


/* Quality-of-life type definitions */
typedef std::vector<int> Bucket;
typedef std::pair<int, int> Range;


/* Global Concurrency Variables */
static int numThreads;
static std::vector<std::thread*> workers;

/* Global Timing Variables */
static timespec                     startTime;
static timespec                     endTime;
static unsigned long long           epochNs;
static unsigned long long           epochSec;


/* Forward Function Declarations */
namespace mythread
{
    void fork(void);
    void join(void);
    void cleanup(void);
    void threadMain(int threadID);
}
namespace myglobals
{
    void init(void);
    void cleanup(void);
    void printRunTime(void);
}



namespace mythread
{
    void fork(void)
    {
        // resize the worker vector
        workers.resize(numThreads - 1);

        // Create barriers for worker threads and the main thead
        // bar = new std::barrier<>(numThreads + 1);

        for(int threadIdx = 0; threadIdx < numThreads - 1; threadIdx++)
        {
            // std::cout << "Created Thread: " << threadIdx << std::endl;
            workers[threadIdx] = new std::thread(threadMain, threadIdx);
        }
    }


    void join(void)
    {
        for(int i = 0; i < numThreads - 1; i++)
        {
            // std::cout << "Joined Thread: " << i << std::endl;
            workers[i]->join();
        }
    }


    void cleanup(void)
    {
        // delete bar;

        for(int i = 0; i < numThreads - 1; i++)
        {
            // std::cout << "Deleted Thread: " << i << std::endl;
            delete workers[i];
        }

        workers.clear();
    }


    void threadMain(int threadID)
    {
        // Synchronize worker and main threads
        // bar->arrive_and_wait();

        if(threadID == MAIN_THREAD)
        {
            clock_gettime(CLOCK_MONOTONIC, &startTime);
        }

        // Distribute ranges for threads to work on
        // Range sortRange = sort::getRangeToSort(threadID);

        // Sort the range
        // sort::quickSort(sortedValues, sortRange.first, sortRange.second);

        // bar->arrive_and_wait();

        // Contents of the ranges have been sorted, sort the ranges themselves
        if(threadID == MAIN_THREAD)
        {
            // Sort the now-sorted ranges
            // sort::quickSort(sortedValues, 0, sortedValues->size() - 1);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
        }
    }
}


namespace myglobals
{
    void init(void)
    {
        startTime.tv_nsec   = 0;
        startTime.tv_sec    = 0;
        endTime.tv_nsec     = 0;
        endTime.tv_sec      = 0;
    }

    void printRunTime(void)
    {
        // Calculate runtime
        epochNs = (endTime.tv_sec - startTime.tv_sec) * 1000000000 + (endTime.tv_nsec - startTime.tv_nsec);
        epochSec = static_cast<double>(epochNs) / 1000000000.0;

        // Print runtime
        std::cout << "BucketSort Complete:\n";
        std::cout << "Runtime: " << epochNs << "ns\n";
        std::cout << "Runtime: " << epochSec << "sec\n";
    }
}

#endif  /* MAIN_H */