#ifndef TICKETLOCK_H
#define TICKETLOCK_H

#include "lock.h"

namespace primitive
{
    class TicketLock : public Lock
    {
        private:
            std::atomic<int> nextNum;
            std::atomic<int> nowServing;

        public:
            TicketLock();
            virtual ~TicketLock();

            void lock(void) override;
            void unlock(void) override;        
    };
}

#endif  /* TICKETLOCK_H */