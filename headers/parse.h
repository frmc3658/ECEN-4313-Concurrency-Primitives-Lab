#ifndef PARSE_H
#define PARSE_H


#include <fstream>
#include <getopt.h>
#include <iostream>
#include <string>
#include <vector>


struct cmdLine;
extern std::vector<int>* sortedValues;

class Parse
{
    public:
        Parse();
        ~Parse();
    
        void validateArgc(int argc);
        void parseCMD(int argc, char* argv[], cmdLine& cmd);
        void parseInputFile(std::vector<int>*& storage, cmdLine &cmd);
        void writeSortedValues(cmdLine& cmd);
        void printName(void);
        void printHelpFile(void);
    
    private:

};


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


const option longOptions[] = {
    /* name         has_arg             flag        val */
    {"lock",        required_argument,  nullptr,    'l'},
    {"bar",         required_argument,  nullptr,    'b'},
    {"help",        no_argument,        nullptr,    'h'},
    {"name",        no_argument,        nullptr,    'n'},
    {nullptr,       no_argument,        nullptr,    0}
};


#endif  /* PARSE_H */