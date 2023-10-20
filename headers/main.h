#ifndef MAIN_H
#define MAIN_H

#include <fstream>
#include <getopt.h>
#include <iostream>
#include <queue>
#include <set>
#include <thread>
#include <vector>

#include "lock.h"


#define DEFAULT_NUM_THREADS     4
#define MAIN_THREAD             numThreads


/* Quality-of-life type definitions */
typedef std::vector<int> Bucket;
typedef std::pair<int, int> Range;

/* Valid long options */
static const option longOptions[] = {
    /* name         has_arg             flag        val */
    {"lock",        required_argument,  nullptr,    'l'},
    {"bar",         required_argument,  nullptr,    'b'},
    {"help",        no_argument,        nullptr,    'h'},
    {"name",        no_argument,        nullptr,    'n'},
    {nullptr,       no_argument,        nullptr,    0}
};

/* Valid lock argumnets */
const std::string lockTypes[] = {
    "tas",
    "ttas",
    "ticket",
    "mcs",
    "pthread",
    "petersonseq",
    "petersonrel"
};

/* Valid barrier arguments */
const std::string barrierTypes[] = {
    "sense",
    "pthread"
};

/* Command-Line Parsing Struct */
struct cmdLine
{
    int numThreads;
    std::string lockRequest;
    std::string barRequest;
    std::string inputPath;
    std::string outputPath;
    std::ifstream inputFile;
    std::ofstream outputFile;
};


/* Global Command Line Variables */
static std::vector<int>* sortedValues;
static cmdLine cmd;

/* Global Concurrency Variables */
static int numThreads;
static std::vector<std::thread*> workers;

/* Global Timing Variables */
static timespec                     startTime;
static timespec                     endTime;
static unsigned long long           epochNs;
static unsigned long long           epochSec;


/* Forward Function Declarations */
namespace mythread
{
    void fork(void);
    void join(void);
    void cleanup(void);
    void threadMain(int threadID);
}
namespace parse
{
    void validateArgc(int argc);
    void parseCMD(int argc, char* argv[], cmdLine& cmd);
    void parseInputFile(std::vector<int>*& storage, cmdLine &cmd);
    void writeSortedValues(cmdLine& cmd);
    void printName(void);
    void printHelpFile(void);
}
namespace myglobals
{
    void init(void);
    void cleanup(void);
    void setNumThreads(cmdLine& cmd);
    void printRunTime(void);
}



namespace mythread
{
    void fork(void)
    {
        // resize the worker vector
        workers.resize(numThreads - 1);

        // Create barriers for worker threads and the main thead
        // bar = new std::barrier<>(numThreads + 1);

        for(int threadIdx = 0; threadIdx < numThreads - 1; threadIdx++)
        {
            // std::cout << "Created Thread: " << threadIdx << std::endl;
            workers[threadIdx] = new std::thread(threadMain, threadIdx);
        }
    }


    void join(void)
    {
        for(int i = 0; i < numThreads - 1; i++)
        {
            // std::cout << "Joined Thread: " << i << std::endl;
            workers[i]->join();
        }
    }


    void cleanup(void)
    {
        // delete bar;

        for(int i = 0; i < numThreads - 1; i++)
        {
            // std::cout << "Deleted Thread: " << i << std::endl;
            delete workers[i];
        }

        workers.clear();
    }


    void threadMain(int threadID)
    {
        // Synchronize worker and main threads
        // bar->arrive_and_wait();

        if(threadID == MAIN_THREAD)
        {
            clock_gettime(CLOCK_MONOTONIC, &startTime);
        }

        // Distribute ranges for threads to work on
        // Range sortRange = sort::getRangeToSort(threadID);

        // Sort the range
        // sort::quickSort(sortedValues, sortRange.first, sortRange.second);

        // bar->arrive_and_wait();

        // Contents of the ranges have been sorted, sort the ranges themselves
        if(threadID == MAIN_THREAD)
        {
            // Sort the now-sorted ranges
            // sort::quickSort(sortedValues, 0, sortedValues->size() - 1);
            clock_gettime(CLOCK_MONOTONIC, &endTime);
        }
    }
}


namespace parse
{
    void validateArgc(int argc)
    {
        try
        {
            if(argc < 2)
            {
                throw std::runtime_error("ERROR: Not enough Arguments\n");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what()<< std::endl;
            exit(EXIT_FAILURE);
        }
    }


    void parseCMD(int argc, char* argv[], cmdLine& cmd)
    {
        while(true)
        {
            int optIndex = 0;
            int option = getopt_long(argc, argv, "i:o:t:", longOptions, &optIndex);

            if(option == -1){ break; }

            switch(option)
            {
                case 'i':
                    cmd.inputPath = (std::string)optarg;
                    break;
                case 'o':
                    cmd.outputPath = (std::string)optarg;
                    break;
                case 'b':
                    cmd.barRequest = (std::string)optarg;
                    break;
                case 'l':
                    cmd.lockRequest = (std::string)optarg;
                    break;
                case 't':
                    cmd.numThreads = std::stoi(optarg);
                    break;
                case 'h':
                    parse::printHelpFile();
                    exit(EXIT_SUCCESS);
                    break;
                case 'n':
                    parse::printName();
                    exit(EXIT_SUCCESS);
                    break;
                case '?':
                    try
                    {
                        throw std::runtime_error("Error Unknown Option\n");
                    }
                    catch(const std::exception& e)
                    {
                        std::cerr << e.what() << '\n';
                        exit(EXIT_FAILURE);
                    }
            }
        }
    }


    void parseInputFile(std::vector<int>*& storage, cmdLine &cmd)
    {
        storage = new std::vector<int>;
        std::string path = cmd.inputPath;
        std::ifstream file(path);

        try
        {
            if(!file)
            {
                throw std::runtime_error("ERROR: Input File Error\n");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            exit(EXIT_FAILURE);
        }
        
        std::string line;
    
        while(std::getline(file, line))
        {
            storage->push_back(std::stoi(line));
        }

        file.close();
    }


    void writeSortedValues(cmdLine& cmd)
    {
        std::ofstream outFile(cmd.outputPath);

        try
        {
            if(!outFile)
            {
                throw std::runtime_error("ERROR: Output File Error\n");
            }
            else
            {
                std::cout << "Writing Sorted Values to: " << cmd.outputPath << std::endl;

                for(size_t i = 0; i < sortedValues->size(); i++)
                {
                    outFile << (*sortedValues)[i] << std::endl;
                }

                std::cout << "Writing Complete!\n";
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            exit(EXIT_FAILURE);
        }
        
        outFile.close();
    }


    void printName(void)
    {
        std::cout << "Frank McDermott\n";
    }


    void printHelpFile(void)
    {       
        std::ifstream file("help/help.txt");

        try
        {
            if(!file)
            {
                throw std::runtime_error("ERROR: Help File Error\n");
            }
        }
        catch(const std::exception& e)
        {
            std::cerr << e.what() << "\n";
            exit(EXIT_FAILURE);
        }
        
        std::string line;
    
        while(std::getline(file, line))
        {
            std::cout << line << std::endl;
        }

        file.close();
    }
}


namespace myglobals
{
    void init(void)
    {
        cmd.numThreads      = DEFAULT_NUM_THREADS;
        startTime.tv_nsec   = 0;
        startTime.tv_sec    = 0;
        endTime.tv_nsec     = 0;
        endTime.tv_sec      = 0;
    }

    void cleanup(void)
    {
        delete sortedValues;
        sortedValues->clear();
    }

    void setNumThreads(cmdLine& cmd)
    {
        numThreads = cmd.numThreads;
    }

    void printRunTime(void)
    {
        // Calculate runtime
        epochNs = (endTime.tv_sec - startTime.tv_sec) * 1000000000 + (endTime.tv_nsec - startTime.tv_nsec);
        epochSec = static_cast<double>(epochNs) / 1000000000.0;

        // Print runtime
        std::cout << "BucketSort Complete:\n";
        std::cout << "Runtime: " << epochNs << "ns\n";
        std::cout << "Runtime: " << epochSec << "sec\n";
    }
}

#endif  /* MAIN_H */