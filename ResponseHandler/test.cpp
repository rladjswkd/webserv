#include "StatusText.hpp"
#include <iostream>

#include <ctime>

std::string getDateTime() {
    std::string currentTime;
    time_t now = time(nullptr) + 7200;

    char buffer[80];
    struct tm* timeinfo = gmtime(&now);
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    currentTime = std::string(buffer);
    
    return currentTime;
}



int main()
{
    // StatusText map;
    
    // StatusText::StatusTextMapType statusMap = StatusText().getStatusTextMap();
    // StatusText::StatusTextMapType::iterator it = statusMap.begin();
    // // StatusText::StatusTextMapType::iterator it = map.getStatusTextMap().begin();

    // for(; it != statusMap.end(); ++it)
    // {
    //    std::cout << (*it).first << " : " << (*it).second <<std::endl; 
    // }

    // std::map<std::string, std::string> map;

    // map["a"] = "b";

    // std::cout << map["c"] << "test" << std::endl;

    std::cout << "getDateTime : " << getDateTime() << std::endl;


}