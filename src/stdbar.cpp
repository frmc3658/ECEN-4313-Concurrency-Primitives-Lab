#include "../headers/stdbar.h"

namespace primitive
{
    StdBar::StdBar()
    {

    }


    StdBar::~StdBar()
    {
        delete bar;
    }


    void StdBar::init(int numThreads)
    {
        bar = new std::barrier<>(numThreads);
    }

    void StdBar::wait(void)
    {
        bar->arrive_and_wait();
    }
}