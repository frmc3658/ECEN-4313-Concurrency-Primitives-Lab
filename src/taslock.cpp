#include "../headers/taslock.h"

namespace primitive
{
    TASLock::TASLock()
    {
        flag = false;
    }


    TASLock::~TASLock()
    {

    }


    void TASLock::lock(void)
    {
        while(!this->tas(flag, SEQCST)){/* spin */}
    }


    void TASLock::unlock(void)
    {
        flag.store(false, SEQCST);
    }
}