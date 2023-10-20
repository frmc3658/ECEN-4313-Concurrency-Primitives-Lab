#include "../headers/main.h"


std::vector<int>* sortedValues;


int main(int argc, char* argv[])
{
    // Validate Command Line Arguements
    Parse parser;
    parser.validateArgc(argc);
    parser.parseCMD(argc, argv);

    threadsRequested = parser.getNumThreads();

    // Parse input file
    parser.parseInputFile(sortedValues);

    // Fork

    // ThreadMain

    // Join

    // Thread cleanup

    // Calculate runtime
    algTimer.printRuntime();

    // Write the sorted values to output file

    // Cleanup
    delete sortedValues;
    sortedValues->clear();

    return 0;
}