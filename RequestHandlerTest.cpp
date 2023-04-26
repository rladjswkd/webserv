#include "RequestHandler.hpp"

#include "stdio.h"
#include <iostream>

int main()
{
    std::string res = RequestHandler::createDirectoryListing("..");
    std::cout << "res : " << res << std::endl;
}