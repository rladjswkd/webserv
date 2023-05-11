#include "Server.hpp"
#include "ConfigParser.hpp"
#include <stdlib.h>
#include <string>
#include "Config.hpp"
#include <utility>
#include <iostream>

#define DEFAULT_CONFIG_FILE "./Files/DefaultConfig"

// main.cpp 대신 이 파일의 이름을 main.cpp로 바꿔 make하면 테스트 결과 확인 가능
// Java의 Junit같은 유닛테스트를 위한 프레임워크나 라이브러리가 있는지 확인해보자.
int main(int argc, char **argv)
{
    std::string filePath = DEFAULT_CONFIG_FILE;
    
    if (argc > 2)
        return (EXIT_FAILURE);
    if (argc == 2)
        filePath = std::string(argv[1]);
    Config          config = ConfigParser::parse(ConfigLexer::tokenize(filePath.c_str()));
    ConfigServer    configServer = config.getServer(std::make_pair("*", "8080"), "");
    if (configServer.findLocation("/temp")->first == "/temp" 
        && configServer.findLocation("/tempp")->first == "/temp"
        && configServer.findLocation("/temppp")->first == "/temppp"
        && configServer.findLocation("/temppppasdf")->first == "/temppp"
        && configServer.findLocation("/tem")->first == "/"
        && configServer.findLocation("/temp/")->first == "/temp/"
        && configServer.findLocation("/temp/to")->first == "/temp/"
        && configServer.findLocation("/temp/tomp")->first == "/temp/"
        && configServer.findLocation("/temp/tomp/")->first == "/temp/tomp/"
    )
        std::cout << "SUCCESS!" << std::endl;
    else
        std::cout << "FAIL!" << std::endl;
    return (0);
}
