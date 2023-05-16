#include "Server.hpp"
#include "ConfigParser.hpp"
#include <stdlib.h>
#include <string>
#include <iostream>

#define DEFAULT_CONFIG_FILE "./Files/DefaultConfig"

int main(int argc, char **argv)
{
    std::string filePath = DEFAULT_CONFIG_FILE;
    
    if (argc > 2)
        return (EXIT_FAILURE);
    if (argc == 2)
        filePath = std::string(argv[1]);
    try
    {
        Server(ConfigParser::parse(ConfigLexer::tokenize(filePath.c_str()))).run();
    }
    catch (const std::exception &ex)
    {
        std::cerr << ex.what() << std::endl;
    }
    return (0);
}