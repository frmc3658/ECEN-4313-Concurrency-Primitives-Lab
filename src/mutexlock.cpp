#include "../headers/mutexlock.h"


namespace primitive
{
    MutexLock::MutexLock()
    {
        
    }


    MutexLock::~MutexLock()
    {
        delete lk;
    }


    void MutexLock::lock(void)
    {
        lk->lock();
    }


    void MutexLock::unlock(void)
    {
        lk->unlock();
    }

}