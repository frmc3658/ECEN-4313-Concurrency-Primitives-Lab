#ifndef TTASLOCK_H
#define TTASLOCK_H

#include "lock.h"

namespace primitive
{
    class TTASLock : public Lock
    {
        public:
            TTASLock();
            virtual ~TTASLock();

            void lock(void) override;
            void unlock(void) override;
    };
}

#endif