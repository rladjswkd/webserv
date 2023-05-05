#ifndef RESPONSE_HANDLER_HPP
# define RESPONSE_HANDLER_HPP

#include <string>
#include <map>
#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include "Response.hpp"
#include "StatusText.hpp"

# define HTTP_VERSION		    "HTTP/1.1"
# define SP                     " "
# define CRLF                   "\r\n"
# define TEXT_HTML              "text/html"
# define COOKIE_VALID_TIME      2
# define ERROR_PAGE_LOCATION    "./ErrorPage/"
# define SESSION_ID             "session_id"

class ResponseHandler
{
    public:
        typedef std::string StartLineType, HeaderLineType, BodyType, DateType, ContentLengthType, CookieStringType, RedirectLocationType;
        typedef std::string ResponseMessageType, StatusCodeType, StatusTextType, ErrorPageLocationType;
        typedef std::string KeepAliveType;
        typedef std::map<std::string, std::string> StatusTextMapType, ErrorPageLocationMapType;

    private:
        static std::string ft_toLower(std::string str);
        static bool isErrorStatusCode();
        static ResponseMessageType createNormalMessage();
        static ResponseMessageType createErrorMessage();
        static BodyType getErrorPageBody();

        static StartLineType createStartLine();
        static StatusTextMapType initialStatusTextMap();

        static HeaderLineType createHeaderLine();
        static DateType getCurrentTime();
        static ContentLengthType getContentLength();
        static std::string sizet_to_string(size_t value);
        static CookieStringType getCookieString();
        static bool isRedirectStatusCode();
        static RedirectLocationType createRedirectLocation();
        static bool isKeepAlive();
        static KeepAliveType createKeepAlive();

        static BodyType createBody();        
        static ResponseMessageType pasteAll(StartLineType &startLine, HeaderLineType &headerLine, BodyType &body);

        static bool isSetCookieHeader(std::string str);
        static bool isContentTypeHeader(std::string str);
        static void cgiBodySetting(Response &inputResponse);
        
    private:        
		static Response	response;

    public:
        static void cgiMessageParsing(Response &inputResponse);
        static ResponseMessageType createResponseMessage(const Response &inputResponseMessage);
};

#endif