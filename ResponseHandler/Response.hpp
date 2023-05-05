#ifndef RESPONSE_HPP
# define RESPONSE_HPP

#include <string>
#include <map>

class Response
{
    public:
        typedef std::string StatusCodeType;
        Response();

    private:
        std::string statusCode;
        std::string contentLength;
        std::string contentType;
        std::string body;
        std::string date;
        std::string location;
        std::string cookie;
        bool        keepAlive;

    public:
        std::string getStatusCode();
        std::string getContentLength();
        std::string getContentType();
        std::string getBody();
        std::string getDate();
        std::string getLocation();
        std::string getCookie();
        bool        isKeepAlive();

        void setStatusCode(std::string statusCode);
        void setContentLength(std::string contentLength);
        void setContentType(std::string contentType);
        void setBody(std::string body);
        void setDate(std::string date);
        void setLocation(std::string location);
        void setCookie(std::string cookie); 
        void setKeepAlive(bool keepAlive);
};

#endif