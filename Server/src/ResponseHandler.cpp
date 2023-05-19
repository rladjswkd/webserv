#include "ResponseHandler.hpp"
#include <algorithm>

std::string ResponseHandler::ft_toLower(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

bool ResponseHandler::isErrorStatusCode(const Response &response)
{
    Response::StatusCodeType statusCode = response.getStatusCode();

    return (statusCode.find("4") == 0 || statusCode.find("5") == 0);
}

ResponseHandler::StartLineType ResponseHandler::createStartLine(const Response &response)
{
    StartLineType startLine = "";
    StatusTextMapType statusTextMap = StatusText().getStatusTextMap();
    StatusCodeType  statusCode = response.getStatusCode();

    startLine += HTTP_VERSION;
    startLine += SP;
    startLine += statusCode;
    startLine += SP;
    startLine += statusTextMap[statusCode];
    startLine += CRLF;

    return startLine;
}

ResponseHandler::DateType ResponseHandler::getCurrentTime() {
    DateType currentTime;
    time_t now = time(0);

    char buffer[80];
    struct tm* timeinfo = gmtime(&now);
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    currentTime = std::string(buffer);

    return currentTime;
}

std::string ResponseHandler::sizet_to_string(const size_t value) 
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

ResponseHandler::ContentLengthType ResponseHandler::getContentLength(const Response &response)
{
    return sizet_to_string(response.getBody().length());    
}

ResponseHandler::CookieStringType ResponseHandler::getCookieString(const Response &response)
{
    ResponseHandler::CookieStringType cookieString = "";
    cookieString = response.getCookie();
    if (cookieString.size() > 0)
        cookieString += CRLF;
    return cookieString;
}

bool ResponseHandler::isRedirectStatusCode(const Response &response)
{
    return (response.getStatusCode().find("3") == 0);
}

ResponseHandler::RedirectLocationType ResponseHandler::createRedirectLocation(const Response &response)
{
    ResponseHandler::RedirectLocationType redirectLocation = "";

    redirectLocation += "Location: ";
    redirectLocation += response.getLocation();
    redirectLocation += CRLF;
    return redirectLocation;
}

bool ResponseHandler::isKeepAlive(const Response &response)
{
    return response.isKeepAlive();
}

ResponseHandler::KeepAliveType ResponseHandler::createKeepAlive(const Response &response)
{
    std::string connection;

    if (isKeepAlive(response))
        connection = "Connection: Keep-Alive";
    else
        connection = "Connection: close";
    connection += CRLF;
    return connection;
}

ResponseHandler::HeaderLineType ResponseHandler::createHeaderLine(const Response &response)
{
    HeaderLineType headerLine = "";
    DateType currentTime = getCurrentTime();
    
    //date
    headerLine += "Date: ";
    headerLine += currentTime;
    headerLine += CRLF;

    //content_type
    headerLine += "Content-Type: ";
    headerLine += response.getContentType();
    headerLine += CRLF;

    //content-length
    headerLine += "Content-Length: ";
    headerLine += getContentLength(response);
    headerLine += CRLF;
    
    //cookie
    headerLine += getCookieString(response);
    
    //redirection
    if (isRedirectStatusCode(response))
        headerLine += createRedirectLocation(response);

    //Connection(keepAlive)
    headerLine += createKeepAlive(response);
    
    //last CRLF
    headerLine += CRLF;
    return headerLine;
}

ResponseHandler::ResponseMessageType ResponseHandler::createNormalMessage(const Response &response)
{
    ResponseMessageType responseMessage, startLine, headerLine, body;

    startLine = createStartLine(response);
    headerLine = createHeaderLine(response);
    body = response.getBody();
    responseMessage = startLine + headerLine + body;
    return responseMessage;
}

ResponseHandler::BodyType ResponseHandler::getErrorPageBody(const Response &response)
{
    std::string fileLocation = ERROR_PAGE_LOCATION + response.getStatusCode() + ".html";
    std::ifstream file(fileLocation.c_str());
    std::stringstream buffer;
    buffer << file.rdbuf(); 
    return buffer.str();
}

ResponseHandler::ResponseMessageType ResponseHandler::createResponseMessage(const Response &response)
{
    return (createNormalMessage(response));
}

bool ResponseHandler::isSetCookieHeader(std::string &str)
{
    return (ft_toLower(str).find("set-cookie: ") == 0);
}

bool ResponseHandler::isContentTypeHeader(std::string &str)
{
    return (ft_toLower(str).find("content-type: ") == 0);
}

void ResponseHandler::cgiBodySetting(Response &inputResponse)
{
    std::string::size_type pos;
    std::string cgiBody = inputResponse.getBody();

    pos = cgiBody.find("\r\n\r\n");
    if (pos != std::string::npos)
        inputResponse.setBody(cgiBody.substr(pos + 4, cgiBody.size()));
}

void ResponseHandler::cgiMessageParsing(Response &inputResponse)
{
    std::string cgiBody = inputResponse.getBody();
    std::stringstream ss(cgiBody);
    std::string temp, currentCookie;
 
    while (getline(ss, temp, '\r')) {
      if (isSetCookieHeader(temp))
      {
        currentCookie = inputResponse.getCookie();
        if (currentCookie.size() > 0)
            currentCookie += CRLF;
        currentCookie += temp;
        inputResponse.setCookie(currentCookie);
      }
      else if(isContentTypeHeader(temp))
        inputResponse.setContentType(temp.substr(14, temp.size()));
      ss.get();
    }
    cgiBodySetting(inputResponse);
}
