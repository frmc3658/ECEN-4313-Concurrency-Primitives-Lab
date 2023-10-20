#include "../headers/parse.h"

Parse::Parse()
{

}


Parse::~Parse()
{

}


void Parse::validateArgc(int argc)
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


void Parse::parseCMD(int argc, char* argv[], cmdLine& cmd)
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
                printHelpFile();
                exit(EXIT_SUCCESS);
                break;
            case 'n':
                printName();
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


void Parse::parseInputFile(std::vector<int>*& storage, cmdLine &cmd)
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


void Parse::writeSortedValues(cmdLine& cmd)
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


void Parse::printName(void)
{
    std::cout << "Frank McDermott\n";
}


void Parse::printHelpFile(void)
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

