#include "../headers/main.h"


std::vector<int>* sortedValues;
primitive::Bar* bar;
primitive::Lock* lk;


int main(int argc, char* argv[])
{
    // Validate Command Line Arguements
    Parse parser;
    parser.validateArgc(argc);
    parser.parseCMD(argc, argv);

    // Set global number of threads requested
    threadsRequested = parser.getNumThreads();

    // Parse input file
    parser.parseInputFile(sortedValues);

    // Select lock and barrier
    bar = parser.selectBarrierType();
    lk = parser.selectLockType();

    // Fork
    mythread::fork();

    // ThreadMain
    mythread::threadMain(MAIN_THREAD);

    // Join
    mythread::join();

    // Thread cleanup
    mythread::cleanup();

    // Calculate runtime
    algTimer.printRuntime();

    // Write the sorted values to output file
    parser.writeSortedValues();

    // Global cleanup
    delete bar;
    delete lk;
    delete sortedValues;
    sortedValues->clear();

    return 0;
}