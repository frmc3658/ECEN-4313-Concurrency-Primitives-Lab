#ifndef PARSE_H
#define PARSE_H


#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>

#include "primitives.h"

extern std::vector<int>* unsortedValues;

class Parse
{
    private:
        std::string     barRequest;
        std::string     inputPath;
        std::string     lockRequest;
        int             numThreads;
        std::string     outputPath;

    public:
        Parse();
        ~Parse();
    
        /* Interface functions */
        void validateArgc(int argc);
        void parseCMD(int argc, char* argv[]);
        primitive::Bar* selectBarrierType(void);
        primitive::Lock* selectLockType(void);
        void parseInputFile(std::vector<int>*& storage);
        void writeSortedValues(void);
        void printName(void);
        void printHelpFile(void);


        /* Accessor helper functions */
        int getNumThreads(void) const { return numThreads; }
        std::string getBarRequest(void) const { return barRequest; }
        std::string getLockRequest(void) const { return lockRequest; }
    
        /* Modifer helper functions */
        inline void setBarRequest(std::string req) { barRequest = req; }
        inline void setInputPath(std::string path) { inputPath = path; }
        inline void setLockRequest(std::string req) { lockRequest = req; }
        inline void setNumThreads(int num_threads) { numThreads = num_threads; }    
        inline void setOutputPath(std::string path) { outputPath = path; }
};


 /* Command Line: long options */
const option longOptions[] = {
    /* name         has_arg             flag        val */
    {"bar",         required_argument,  nullptr,    'b'},
    {"help",        no_argument,        nullptr,    'h'},
    {"lock",        required_argument,  nullptr,    'l'},
    {"name",        no_argument,        nullptr,    'n'},
    {nullptr,       no_argument,        nullptr,    0}
};


#endif  /* PARSE_H */