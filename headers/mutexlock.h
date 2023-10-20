#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include "lock.h"
#include <mutex>

namespace primitive
{
    class MutexLock : public Lock
    {
        public:
            MutexLock();
            virtual ~MutexLock();

            void lock(void) override;
            void unlock(void) override;

        private:
            std::mutex* lk;
    };
}


#endif