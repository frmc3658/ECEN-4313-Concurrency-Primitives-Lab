#ifndef PARSE_H
#define PARSE_H


#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>


extern std::vector<int>* sortedValues;

class Parse
{
    private:
        struct cmdLine
        {
            std::string     barRequest;
            std::ifstream   inputFile;
            std::string     inputPath;
            std::string     lockRequest;
            int             numThreads;
            std::ofstream   outputFile;
            std::string     outputPath;
        }cmd;

    public:
        Parse();
        ~Parse();
    
        void validateArgc(int argc);
        void parseCMD(int argc, char* argv[]);
        void parseInputFile(std::vector<int>*& storage);
        void writeSortedValues(void);
        void printName(void);
        void printHelpFile(void);
    
        inline void setBarRequest(std::string req) { cmd.barRequest = req; }
        inline void setInputPath(std::string path) { cmd.inputPath = path; }
        inline void setLockRequest(std::string req) { cmd.lockRequest = req; }
        inline void setNumThreads(int num_threads) { cmd.numThreads = num_threads; }    
        inline void setOutputPath(std::string path) { cmd.outputPath = path; }
};


const option longOptions[] = {
    /* name         has_arg             flag        val */
    {"bar",         required_argument,  nullptr,    'b'},
    {"help",        no_argument,        nullptr,    'h'},
    {"lock",        required_argument,  nullptr,    'l'},
    {"name",        no_argument,        nullptr,    'n'},
    {nullptr,       no_argument,        nullptr,    0}
};


#endif  /* PARSE_H */