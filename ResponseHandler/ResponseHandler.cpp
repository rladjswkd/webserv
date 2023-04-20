#include "ResponseHandler.hpp"

Response ResponseHandler::response;

bool ResponseHandler::isErrorStatusCode()
{
    Response::StatusCodeType statusCode = response.getStatusCode();
    if (statusCode.find("4") == 0 || statusCode.find("5") == 0)
        return true;
    return false;
}

ResponseHandler::StatusTextMapType ResponseHandler::initialStatusTextMap()
{
    return StatusText().getStatusTextMap();
}

ResponseHandler::StartLineType ResponseHandler::createStartLine()
{
    StartLineType startLine = "";
    StatusTextMapType statusTextMap = initialStatusTextMap();


    startLine += HTTP_VERSION;
    startLine += SP;
    if (statusTextMap.count(response.getStatusCode()) == 0)
        std::cout << "code : " << response.getStatusCode() << " doesn't exist in the statusTextMap." << std::endl;
    startLine += response.getStatusCode();
    startLine += SP;
    startLine += statusTextMap[response.getStatusCode()];
    startLine += CRLF;

    return startLine;
}

ResponseHandler::DateType ResponseHandler::getCurrentTime() {
    DateType currentTime;
    time_t now = time(nullptr);

    char buffer[80];
    struct tm* timeinfo = gmtime(&now);
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    currentTime = std::string(buffer);

    return currentTime;
}

std::string ResponseHandler::sizet_to_string(size_t value) 
{
    std::stringstream ss;
    ss << value;
    return ss.str();
}

ResponseHandler::ContentLengthType ResponseHandler::getContentLength()
{
    return sizet_to_string(response.getBody().length());
}

ResponseHandler::DateType ResponseHandler::getCookieTime() {
    DateType cookieTime;
    time_t now = time(nullptr) + COOKIE_VALID_TIME * 60 * 60;

    char buffer[80];
    struct tm* timeinfo = gmtime(&now);
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    cookieTime = std::string(buffer);

    return cookieTime;
}

ResponseHandler::CookieStringType ResponseHandler::getCookieString()
{
    ResponseHandler::CookieStringType cookieString;
    Response::CookieType cookieMap = response.getCookie();
    Response::CookieType::iterator it = cookieMap.begin();

    for(; it != cookieMap.end(); ++it)
    {
        cookieString += "Set-Cookie: ";
        cookieString += (*it).first;
        cookieString += "=";
        cookieString += (*it).second;
        cookieString += "; ";
        cookieString += getCookieTime();
        cookieString += CRLF;
    }
    return cookieString;
}

bool ResponseHandler::isRedirectStatusCode()
{
    Response::StatusCodeType statusCode = response.getStatusCode();
    if (statusCode.find("3"))
        return true;
    return false;
}

ResponseHandler::RedirectLocationType ResponseHandler::createRedirectLocation()
{
    ResponseHandler::RedirectLocationType redirectLocation = "";

    redirectLocation += "Location: ";
    redirectLocation += response.getLocation();
    redirectLocation += CRLF;
    return redirectLocation;
}

ResponseHandler::HeaderLineType ResponseHandler::createHeaderLine()
{
    HeaderLineType headerLine = "";
    DateType currentTime = getCurrentTime();
    
    //date
    headerLine += "Date: ";
    headerLine += currentTime;
    headerLine += CRLF;

    //content_type
    headerLine += "Content-Type: ";
    headerLine += TEXT_HTML;
    headerLine += CRLF;

    //content-length
    headerLine += "Content-Length: ";
    headerLine += getContentLength();
    headerLine += CRLF;
    
    //cookie
    headerLine += getCookieString();
    
    //redirection
    if (isRedirectStatusCode())
        headerLine += createRedirectLocation();
    
    //last CRLF
    headerLine += CRLF;
    return headerLine;
}

ResponseHandler::BodyType ResponseHandler::createBody()
{
    return response.getBody();
}

ResponseHandler::ResponseMessageType ResponseHandler::pasteAll(StartLineType &startLine, HeaderLineType &headerLine, BodyType &body)
{
    ResponseMessageType responseMessage;

    responseMessage += startLine;
    responseMessage += headerLine;
    responseMessage += body;
    return responseMessage;
}

ResponseHandler::ResponseMessageType ResponseHandler::createErrorMessage()
{

}

ResponseHandler::ResponseMessageType ResponseHandler::createResponseMessage(Response &inputResponseMessage)
{
    ResponseHandler::ResponseMessageType outputResponseMessage, startLine, headerLine, body;
    
    response = inputResponseMessage;
    if (!isErrorStatusCode())
    {
        startLine = createStartLine();
        headerLine = createHeaderLine();
        body = createBody();
        outputResponseMessage = pasteAll(startLine, headerLine, body);
    }
    else
        outputResponseMessage = createErrorMessage();

    return outputResponseMessage; 
}