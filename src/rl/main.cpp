#include "CWaggle.h"
#include "RLExperiment.hpp"

int main(int argc, char ** argv)
{
    if (argc != 2)
    {
        std::cout << "Usage: cwaggle_rl configFileName\n";
    }
    else
    {
        std::cout << "Using config file: " << argv[1] << "\n";
        RLExperiments::MainRLExperiment(argv[1]);
    }
    
    return 0;
}