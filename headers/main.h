#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <thread>
#include <vector>

#include "bucketsort.h"
#include "parse.h"
#include "primitives.h"
#include "timer.h"


#define DEFAULT_NUM_THREADS     4
#define MAIN_THREAD             threadsRequested - 1


/* Quality-of-life type definitions */
typedef std::vector<int> Bucket;
typedef std::pair<int, int> Range;


/* Global Concurrency Variables */
static int threadsRequested;
static std::vector<std::thread*> workers;
extern primitive::Bar* bar;

/* Externally declared Concurrency Variables */
extern primitive::Lock* lk;

/* external sorting variables */
extern BucketSort bucket;

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
        delete bar;
        delete lk;

        for(int i = 0; i < threadsRequested - 1; i++)
        {
            // std::cout << "Deleted Thread: " << i << std::endl;
            delete workers[i];
        }
    }


    void threadMain(int threadID)
    {
        // Synchronize worker and main threads
        // printf("Thread %d: Reached Barrier 1\n", threadID);
        bar->wait();

        if(threadID == MAIN_THREAD)
        {
            algTimer.setStartTime();
            // printf("Thread %d: Set the start time\n", threadID);
        }

        // printf("Thread %d: Reached Barrier 2\n", threadID);
        bar->wait();

        // Sort the range
        bucket.sort(threadID);

        // printf("Thread %d: finished\n", threadID);
        bar->wait();

        // Contents of the ranges have been sorted, sort the ranges themselves
        if(threadID == MAIN_THREAD)
        {
            algTimer.setEndTime();
            // printf("Thread %d: Set the end time\n", threadID);
        }

        // printf("Thread %d Reached Barrier 4\n", threadID);
        bar->wait();
    }
}


#endif  /* MAIN_H */