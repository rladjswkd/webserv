#ifndef RESPONSE_HANDLER_HPP
# define RESPONSE_HANDLER_HPP

#include <string>
#include <map>
#include "Response.hpp"

class ResponseHandler
{
    public:
        typedef std::string StartLineType, HeaderLineType, BodyType, DateType, CookieStringType;
        typedef std::string ResponseMessageType, StatusCodeType, StatusTextType, ErrorPageLocationType;
        typedef std::map<std::string, std::string> StatusTextMapType, ErrorPageLocationMapType;

    private:
        static StartLineType createStartLine();
        static StatusTextMapType initialStatusTextMap();
        static StatusTextType getStatusText(StatusCodeType statusCode);

        static HeaderLineType createHeaderLine();
        static DateType getCurrentDate();
        static CookieStringType setCookie();

        static ErrorPageLocationMapType initialErrorPageMap();
        static ErrorPageLocationType getErrorPageLocation(StatusCodeType statusCode);
        static BodyType getErrorPageBody(StatusCodeType statusCode);

        static void pasteAll();
        

    private:        
		static Response	response;
        static ResponseMessageType outputResponseMessage;    

    public:
        static ResponseMessageType createResponseMessage(Response &inputResponseMessage);
};

#endif