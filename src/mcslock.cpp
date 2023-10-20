#include "../headers/mcslock.h"


namespace primitive
{
    MCSLock::MCSLock()
    {
        tail = nullptr;
    }


    MCSLock::~MCSLock()
    {
        Node* currentTail = tail.load(SEQCST);

        if(currentTail != nullptr)
        {
            delete currentTail;
        }
    }


    void MCSLock::acquire(Node* myNode)
    {
        Node* oldTail = tail.load(SEQCST);
        myNode->next.store(nullptr, RELAXED);

        // cas() function won't link properly
        // TODO: Fix linker error
        // while(!cas(tail, oldTail, myNode, SEQCST))
        while(tail.compare_exchange_strong(oldTail, myNode, SEQCST))
        {
            oldTail = tail.load(SEQCST);
        }

        if(oldTail != nullptr)
        {
            // acquire the lock
            myNode->wait.store(true, RELAXED);
            oldTail->next.store(myNode, SEQCST);
        }
        else
        {
            // wait for the lock
            while(myNode->wait.load(SEQCST)){/* spin */}
        }
    }


    void MCSLock::release(Node* myNode)
    {
        // cas() function won't link properly
        // TODO: Fix linker error
        // Node* nullNode = nullptr;
        // if(cas(tail, myNode, nullNode, SEQCST))
        if(tail.compare_exchange_strong(myNode, nullptr, SEQCST))
        {
            /* no one waiting; lock released */
        }
        else
        {
            // Hand the lock over to the next waiting thread
            while(myNode->next.load(SEQCST) == nullptr){/* spin */}

            myNode->next.load()->wait.store(false, SEQCST);
        }
    }
}