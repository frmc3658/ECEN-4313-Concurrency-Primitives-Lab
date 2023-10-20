#ifndef TASLOCK_H
#define TASLOCK_H


#include "lock.h"

namespace primitive
{
    class TASLock : public Lock
    {
        public:
            TASLock();
            virtual ~TASLock();

            void lock(void) override;
            void unlock(void) override;    
    };
}


#endif  /* TASLOCK_H */