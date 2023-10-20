#include "../headers/sensebar.h"

namespace primitive
{
    SenseBar::SenseBar()
    {

    }


    SenseBar::SenseBar(int num_threads)
    {
        numThreads = num_threads;
    }


    SenseBar::~SenseBar()
    {

    }


    void SenseBar::init(int num_threads)
    {
        numThreads = num_threads;
    }


    void SenseBar::wait(void)
    {
        thread_local bool mySense = 0;

        // flip sense
        if(mySense == 0) { mySense = 1; }
        else { mySense = 0; }
        
        // fetch and increment count
        int count_copy = fai(count, 1, SEQCST);

        // If last thread to arrive
        if(count_copy == numThreads - 1)
        {
            count.store(0, RELAXED);
            sense.store(mySense, SEQCST);
        }
        // Not the last thread to arrive
        else
        {
            while(sense.load(SEQCST) != mySense) {/* spin */}
        }
    }
}