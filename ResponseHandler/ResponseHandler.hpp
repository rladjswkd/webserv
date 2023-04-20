#ifndef RESPONSE_HANDLER_HPP
# define RESPONSE_HANDLER_HPP

#include <string>
#include <map>
#include <iostream>
#include <ctime>
#include <sstream>
#include "Response.hpp"
#include "StatusText.hpp"

# define HTTP_VERSION		"HTTP/1.1"
# define SP                 " "
# define CRLF               "\r\n"
# define TEXT_HTML          "text/html"
# define COOKIE_VALID_TIME 2

class ResponseHandler
{
    public:
        typedef std::string StartLineType, HeaderLineType, BodyType, DateType, ContentLengthType, CookieStringType, RedirectLocationType;
        typedef std::string ResponseMessageType, StatusCodeType, StatusTextType, ErrorPageLocationType;
        typedef std::map<std::string, std::string> StatusTextMapType, ErrorPageLocationMapType;

    private:
        static bool isErrorStatusCode();

        static StartLineType createStartLine();
        static StatusTextMapType initialStatusTextMap();

        static HeaderLineType createHeaderLine();
        static DateType getCurrentTime();
        static ContentLengthType getContentLength();
        static std::string sizet_to_string(size_t value);
        static CookieStringType getCookieString();
        static DateType getCookieTime();
        static bool isRedirectStatusCode();
        static RedirectLocationType createRedirectLocation();

        static BodyType createBody();

        static ResponseMessageType createErrorMessage();
        static ErrorPageLocationMapType initialErrorPageMap();
        static ErrorPageLocationType getErrorPageLocation(StatusCodeType statusCode);
        static BodyType getErrorPageBody(StatusCodeType statusCode);

        static ResponseMessageType pasteAll(StartLineType &startLine, HeaderLineType &headerLine, BodyType &body);
        

    private:        
		static Response	response;

    public:
        static ResponseMessageType createResponseMessage(Response &inputResponseMessage);
};

#endif