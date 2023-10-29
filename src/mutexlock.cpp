#include "../headers/mutexlock.h"


namespace primitive
{
    MutexLock::MutexLock()
    {
        // printf("Created Lock: pthread\n");
        m_lk = new std::mutex;
    }


    MutexLock::~MutexLock()
    {
        if(m_lk != nullptr)
        {
            // printf("Freed Lock: pthread\n");
            delete m_lk;
        }
        else
        {
            // printf("Lock Not Freed: pthread\n");
        }
    }


    void MutexLock::lock(void)
    {
        m_lk->lock();
    }


    void MutexLock::unlock(void)
    {
        m_lk->unlock();
    }

}