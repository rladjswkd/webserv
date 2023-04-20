#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
    public:
        typedef std::map<std::string, std::string>  CookieType;
        typedef std::string StatusCodeType;

    private:
        std::string statusCode;
        std::string contentLength;
        std::string contentType;
        std::string body;
        std::string date;
        std::string location;
        CookieType cookie;

    public:
        std::string getStatusCode();
        std::string getContentLength();
        std::string getContentType();
        std::string getBody();
        std::string getDate();
        std::string getLocation();
        CookieType  getCookie();

        void setStatusCode(std::string statusCode);
        void setContentLength(std::string contentLength);
        void setContentType(std::string contentType);
        void setBody(std::string body);
        void setDate(std::string date);
        void setLocation(std::string location);
        void setCookie(std::string key, std::string value); 
    
};

#endif