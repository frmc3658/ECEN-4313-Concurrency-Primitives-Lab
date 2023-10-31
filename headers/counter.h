#include <getopt.h>
#include <fstream>
#include <iostream>
#include <string>
#include <thread>
#include <vector>

#include "primitives.h"
#include "timer.h"

#define MAIN_THREAD     NUM_THREADS - 1

extern primitive::Bar* bar;
extern primitive::Lock* lk;
extern std::string barRequested;
extern std::string lockRequested;
extern std::string outputPath;
extern int NUM_THREADS;
extern int NUM_ITERATIONS;
extern int count;

static std::vector<std::thread*> workers;
static Timer algTimer;

 /* Command Line: long options */
const option longOptions[] = {
    /* name         has_arg             flag        val */
    {"bar",         required_argument,  nullptr,    'b'},
    {"help",        no_argument,        nullptr,    'h'},
    {"lock",        required_argument,  nullptr,    'l'},
    {"name",        no_argument,        nullptr,    'a'},
    {nullptr,       no_argument,        nullptr,    0}
};


namespace counter
{
    void validateArgc(int argc)
    {
        if(argc < 2)
        {
            std::cout << "Invalid number of arguments\n";
            exit(EXIT_FAILURE);
        }
    }


    void parseCMD(int argc, char* argv[])
    {
        while(true)
        {
            int optIndex = 0;
            int option = getopt_long(argc, argv, "i:o:t:n:", longOptions, &optIndex);

            if(option == -1){ break; }

            switch(option)
            {
                case 'b':
                    // Barrier Requested
                    barRequested = (std::string)optarg;
                    // printf("Barrier Requested: %s\n", barRequested.c_str());
                    break;
                case 'l':
                    // Lock Requested
                    lockRequested = (std::string)optarg;
                    // printf("Lock Requested: %s\n", lockRequested.c_str());
                    break;
                case 'o':
                    // Output File Path
                    outputPath = (std::string)optarg;
                    // printf("Output Path: %s\n", outputPath.c_str());
                    break;
                case 't':
                    // Number of Threads Requested
                    NUM_THREADS = std::stoi(optarg);
                    // printf("Num Threads Requested: %d\n", NUM_THREADS);
                    break;
                case 'n':
                    // Number of Iterations Requested
                    NUM_ITERATIONS = std::stoi(optarg);
                    // printf("Num Iterations Requested: %d\n", NUM_ITERATIONS);
                    break;
                case 'a':
                    // Print Name
                    std::cout << "Frank McDermott\n";
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


    void validateBarOrLock(void)
    {
        if((barRequested != "") && (lockRequested != ""))
        {
            std::cout << "Invalid input: Both Lock and Barrier requested\n";
            exit(EXIT_FAILURE);
        }
    }


    primitive::Lock* getLockClass(void)
    {
        if(lockRequested == "mcs")
        {
            // std::cout << "mcs lock requested" << std::endl;
            lk = new primitive::MCSLock();
        }
        else if(lockRequested == "petersonrel")
        {
            // Not yet implemented
            // std::cout << "petersonrel lock requested" << std::endl;
            lk = nullptr;
        }
        else if(lockRequested == "petersonseq")
        {
            // Not yet implemented
            // std::cout << "petersonseq lock requested" << std::endl;
            lk = nullptr;
        }
        else if(lockRequested == "pthread")
        {
            // std::cout << "pthread lock requested" << std::endl;
            lk = new primitive::MutexLock();
        }
        else if(lockRequested == "tas")
        {
            // std::cout << "TAS lock requested" << std::endl;
            lk = new primitive::TASLock();
        }
        else if(lockRequested == "ticket")
        {
            // std::cout << "Ticket lock requested" << std::endl;
            lk = new primitive::TicketLock();
        }
        else if(lockRequested == "ttas")
        {
            // std::cout << "TTAS lock requested" << std::endl;
            lk = new primitive::TTASLock();
        }
        else
        {
            // std::cout << "Using default lock" << std::endl;
            lk = new primitive::MutexLock();
        }

        return lk;
    }


    primitive::Bar* getBarClass(void)
    {
        if(barRequested == "pthread")
        {
            // std::cout << "pthread barrier requested" << std::endl;
            bar = new primitive::StdBar(NUM_THREADS);
        }
        else if(barRequested == "sense")
        {
            // std::cout << "sense barrier requested" << std::endl;
            bar = new primitive::SenseBar(NUM_THREADS);
        }
        else if(barRequested == "senserel")
        {
            // Not yet implemented
            // std::cout << "senserel barrier requested" << std::endl;
            bar = nullptr;
        }
        else
        {
            // std::cout << "Using default barrier" << std::endl;
            bar = new primitive::StdBar(NUM_THREADS);
        }

        return bar;
    }


    void thread_main_lock(void)
    {
        for(int i = 0; i < NUM_ITERATIONS; i++)
        {
            lk->lock();
            count++;
            lk->unlock();
        }
    }


    void thread_main_barrier(int threadID)
    {
        for(int i = 0; i < NUM_ITERATIONS * NUM_THREADS; i++)
        {
            if(i % NUM_THREADS == threadID)
            {
                count++;
            }

            bar->wait();
        }
    }


    void threadMain(int threadID)
    {
        if(threadID == 0)
        {
            algTimer.setStartTime();
            // printf("Thread %d: Set the start time\n", threadID);
        }

        // Should have a barrier here

        if(barRequested != "")
        {
            // printf("Calling thread_main_barrier");
            thread_main_barrier(threadID);
        }
        else if(lockRequested != "") 
        {
            // printf("Calling thread_main_lock");
            thread_main_lock();
        }

        // Should have a barrier here

        // Contents of the ranges have been sorted, sort the ranges themselves
        if(threadID == 0)
        {
            algTimer.setEndTime();
            // printf("Thread %d: Set the end time\n", threadID);
        }
    }


    void fork(void)
    {
        // resize the worker vector
        workers.resize(NUM_THREADS - 1);

        for(int threadIdx = 0; threadIdx < NUM_THREADS - 1; threadIdx++)
        {
            // std::cout << "Created Thread: " << threadIdx << std::endl;
            workers[threadIdx] = new std::thread(threadMain, threadIdx);
        }
    }


    void join(void)
    {
        for(int i = 0; i < NUM_THREADS - 1; i++)
        {
            // std::cout << "Joined Thread: " << i << std::endl;
            workers[i]->join();
        }
    }


    void cleanup(void)
    {
        delete bar;
        delete lk;

        for(int i = 0; i < NUM_THREADS - 1; i++)
        {
            // std::cout << "Deleted Thread: " << i << std::endl;
            delete workers[i];
        }
    }


    void writeCounter(void)
    {
        std::ofstream outFile(outputPath);

        try
        {
            if(!outFile)
            {
                throw std::runtime_error("ERROR: Output File Error\n");
            }
            else
            {
                std::cout << "Writing Sorted Values to: " << outputPath << std::endl;

                outFile << count << std::endl;

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
};

