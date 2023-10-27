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
extern std::vector<primitive::Lock*> locks;

/* external sorting variables */
extern BucketSort bucket;
extern std::vector<Bucket> buckets;


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


        for(int i = 0; i < numThreads - 1; i++)
        {
            // std::cout << "Deleted Lock: " << i << std::endl;
            delete locks[i];
        }
    }


    void threadMain(int threadID)
    {
        // Synchronize worker and main threads
        printf("Thread %d: Reached Barrier 1\n", threadID);
        bar->wait();

        if(threadID == MAIN_THREAD)
        {
            algTimer.setStartTime();
            printf("Thread %d: Set the start time\n", threadID);
        }

        printf("Thread %d: Reached Barrier 2\n", threadID);
        bar->wait();

        // Distribute ranges for threads to work on
        // Range sortRange = bucket.getSortRange(threadID);

        // Sort the range
        bucket.sort(sortedValues, locks, buckets, numThreads);

        printf("Thread %d: Reached Barrier 3\n", threadID);
        bar->wait();

        // Contents of the ranges have been sorted, sort the ranges themselves
        if(threadID == MAIN_THREAD)
        {
            // Sort the now-sorted ranges
            // for(int i = 0; i < threadsRequested; i++)
            // {
            //     locks[i]->lock();
            //     std::sort(buckets[i].begin(), buckets[i].end());
            //     locks[i]->unlock();

            //     locks[0]->lock();

            //     for(int i = 1; i < threadsRequested; i++)
            //     {
            //         buckets[0].insert( buckets[0].end(), buckets[i].begin(), buckets[i].end() );
            //     }
                
            //     locks[0]->unlock();

            //     for(size_t j = 0; j < buckets[0].size(); j++)
            //     {
            //         sortedValues->push_back(buckets[0][j]);
            //     }
            // }

            algTimer.setEndTime();
            printf("Thread %d: Set the end time\n", threadID);
        }

        printf("Thread %d Reached Barrier 4\n", threadID);
        bar->wait();
    }
}


#endif  /* MAIN_H */