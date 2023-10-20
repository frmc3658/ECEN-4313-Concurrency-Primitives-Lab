#include "../headers/main.h"



int main(int argc, char* argv[])
{
    // Validate Command Line Arguements
    parse::validateArgc(argc);
    parse::parseCMD(argc, argv, cmd);

    // Set number of threads
    myglobals::setNumThreads(cmd);

    // Parse input file
    parse::parseInputFile(sortedValues, cmd);

    // Execute using fork/join parallelism
    mythread::fork();
    mythread::threadMain(MAIN_THREAD);
    mythread::join();
    mythread::cleanup();

    // Calculate runtime
    myglobals::printRunTime();

    // Write the sorted values to output file
    parse::writeSortedValues(cmd);

    myglobals::cleanup();

    return 0;
}