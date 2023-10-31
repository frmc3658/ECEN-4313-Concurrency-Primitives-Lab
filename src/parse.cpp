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


primitive::Bar* Parse::selectBarrierType(void)
{
    primitive::Bar* bar;

    if(barRequest == "pthread")
    {
        // std::cout << "pthread barrier requested" << std::endl;
        bar = new primitive::StdBar(numThreads);
    }
    else if(barRequest == "sense")
    {
        // std::cout << "sense barrier requested" << std::endl;
        bar = new primitive::SenseBar(numThreads);
    }
    else if(barRequest == "senserel")
    {
        // Not yet implemented
        // std::cout << "senserel barrier requested" << std::endl;
        bar = nullptr;
    }
    else
    {
        // std::cout << "Using default barrier" << std::endl;
        bar = new primitive::StdBar(numThreads);
    }

    return bar;
}


primitive::Lock* Parse::selectLockType(void)
{
    primitive::Lock* lk;

    if(lockRequest == "mcs")
    {
        // std::cout << "mcs lock requested" << std::endl;
        lk = new primitive::MCSLock();
    }
    else if(lockRequest == "petersonrel")
    {
        // Not yet implemented
        // std::cout << "petersonrel lock requested" << std::endl;
        lk = nullptr;
    }
    else if(lockRequest == "petersonseq")
    {
        // Not yet implemented
        // std::cout << "petersonseq lock requested" << std::endl;
        lk = nullptr;
    }
    else if(lockRequest == "pthread")
    {
        // std::cout << "pthread lock requested" << std::endl;
        lk = new primitive::MutexLock();
    }
    else if(lockRequest == "tas")
    {
        // std::cout << "TAS lock requested" << std::endl;
        lk = new primitive::TASLock();
    }
    else if(lockRequest == "ticket")
    {
        // std::cout << "Ticket lock requested" << std::endl;
        lk = new primitive::TicketLock();
    }
    else if(lockRequest == "ttas")
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


void Parse::parseCMD(int argc, char* argv[])
{
    while(true)
    {
        int optIndex = 0;
        int option = getopt_long(argc, argv, "i:o:t:", longOptions, &optIndex);

        if(option == -1){ break; }

        switch(option)
        {
            case 'b':
                setBarRequest((std::string)optarg);
                break;
            case 'h':
                printHelpFile();
                exit(EXIT_SUCCESS);
                break;
            case 'i':
                setInputPath((std::string)optarg);
                break;
            case 'l':
                setLockRequest((std::string)optarg);
                break;
            case 'n':
                printName();
                exit(EXIT_SUCCESS);
                break;
            case 'o':
                setOutputPath((std::string)optarg);
                break;
            case 't':
                setNumThreads(std::stoi(optarg));
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


void Parse::parseInputFile(std::vector<int>*& storage)
{
    storage = new std::vector<int>;
    std::ifstream file(inputPath);

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


void Parse::writeSortedValues(void)
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

            for(size_t i = 0; i < unsortedValues->size(); i++)
            {
                outFile << (*unsortedValues)[i] << std::endl;
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

