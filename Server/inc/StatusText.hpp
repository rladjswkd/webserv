#ifndef STATUSTEXT_HPP
# define STATUSTEXT_HPP


#include <map>
#include <string>

class StatusText
{
    public:
        typedef std::map<std::string, std::string> StatusTextMapType;

    private:
        StatusTextMapType statusTextMap;
    
    public:
        StatusText();
        StatusTextMapType getStatusTextMap() const;
};
#endif