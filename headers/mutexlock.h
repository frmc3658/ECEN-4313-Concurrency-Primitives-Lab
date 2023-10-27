#ifndef MUTEXLOCK_H
#define MUTEXLOCK_H

#include "lock.h"
#include <mutex>

namespace primitive
{
    class MutexLock : public Lock
    {
        private:
            std::mutex* m_lk;

        public:
            MutexLock();
            virtual ~MutexLock();

            void lock(void) override;
            void unlock(void) override;
    };
}


#endif