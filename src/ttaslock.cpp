#include "../headers/ttaslock.h"

namespace primitive
{
    TTASLock::TTASLock()
    {
        flag = false;
    }


    TTASLock::~TTASLock()
    {
        
    }


    void TTASLock::lock(void)
    {
        while(flag.load(ACQUIRE) == true || this->tas(flag, SEQCST) == false){/* spin */}
    }


    void TTASLock::unlock(void)
    {
        flag.store(false, RELEASE);
    }

}