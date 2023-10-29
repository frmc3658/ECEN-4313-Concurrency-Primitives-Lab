#include "../headers/stdbar.h"

namespace primitive
{
    StdBar::StdBar()
    {
        // printf("Created Barrier: pthread\n");
    }


    StdBar::StdBar(int numThreads)
    {
        init(numThreads);
    }


    StdBar::~StdBar()
    {
        if(m_bar != nullptr)
        {
            // printf("Freed Barrier: pthread\n");
            delete m_bar;
        }
    }


    void StdBar::init(int numThreads)
    {
        // printf("Created Barrier: pthread\n");
        m_bar = new std::barrier<>(numThreads);
    }

    void StdBar::wait(void)
    {
        m_bar->arrive_and_wait();
    }
}