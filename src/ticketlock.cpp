#include "../headers/ticketlock.h"


namespace primitive
{
    TicketLock::TicketLock()
    {
        nextNum     = 0;
        nowServing  = 0;
    }


    TicketLock::~TicketLock()
    {
        
    }


    void TicketLock::lock(void)
    {
        int myNum = fai(nextNum, 1, SEQCST);
        while(nowServing.load(SEQCST) != myNum){/* spin */}
    }


    void TicketLock::unlock(void)
    {
        fai(nowServing, 1, SEQCST);
    }
}