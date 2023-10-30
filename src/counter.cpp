#include "../headers/counter.h"

int numThreads;
primitive::Bar* bar;
primitive::Lock* lk;
std::vector<int>* unsortedValues; // Not used; satisfies extern from parse.h

int main(int argc, char* argv[])
{
    Parse parser;
    parser.validateArgc(argc);
    parser.parseCMD(argc, argv);


    std::string barRequested = parser.getBarRequest();
    std::string lockRequested = parser.getLockRequest();

    // Either a lock or a barrier can be requested; not both
    if((barRequested != "") && (lockRequested != ""))
    {
        std::cout << "Invalid input: Both Lock and Barrier requested\n";
        return -2;
    }

    numThreads = parser.getNumThreads();
    
    if(barRequested != "")
    {
        bar = parser.selectBarrierType();
    }
    else if(lockRequested != "")
    {
        lk = parser.selectLockType();
    }

    return 0;
}