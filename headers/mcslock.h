#ifndef MCSLOCK_H
#define MCSLOCK_H

#include "lock.h"

namespace primitive
{
    class MCSLock : public Lock
    {
        private:
            struct Node
            {
                std::atomic<Node*> next;
                std::atomic<bool> wait;
            };
            
            std::atomic<Node*> tail;

        public:
            MCSLock();
            virtual ~MCSLock();

            // lock/unlock are not mean to be used
            // They are overriden only to make MCSLock
            // instantiable.
            void lock(void) override{}
            void unlock(void) override{}

            // acquire/release are the primary interfaces 
            // for the MCSLock class
            void acquire(Node* myNode);
            void release(Node* myNode);
    };
}

#endif /* MCSLOCK_H */