#include "../headers/counter.h"


// Externally declared globals
int NUM_THREADS;
int NUM_ITERATIONS;
int count;

// Parser strings
std::string barRequested;
std::string lockRequested;
std::string outputPath;

// Concurrency primitives
primitive::Bar* bar;
primitive::Lock* lk;


std::vector<int>* unsortedValues= nullptr; // NOT USED; CLEARS UP EXTERN ERROR FROM PARSER.H


int main(int argc, char* argv[])
{
    // Validate argument count
    counter::validateArgc(argc);

    // Parse the command line arguments
    counter::parseCMD(argc, argv);

    // Either a lock or a barrier can be requested; not both
    counter::validateBarOrLock();
    
    // Set Lock or Barrier
    if(barRequested != "")
    {
        // std::cout << "Getting Barrier Class\n";
        bar = counter::getBarClass();
    }
    else if(lockRequested != "")
    {
        // std::cout << "Getting Lock Class\n";
        lk = counter::getLockClass();
    }

    counter::fork();
    counter::threadMain(MAIN_THREAD);
    counter::join();
    counter::cleanup();

    algTimer.printRuntime();
    counter::writeCounter();

    return 0;
}