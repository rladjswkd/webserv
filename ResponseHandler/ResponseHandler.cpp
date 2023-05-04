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
    time_t now = time(0);

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
    time_t now = time(0) + COOKIE_VALID_TIME * 60 * 60;

    char buffer[80];
    struct tm* timeinfo = gmtime(&now);
    strftime(buffer, 80, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);
    cookieTime = std::string(buffer);

    return cookieTime;
}

bool ResponseHandler::isSession(std::string key)
{
    if (key == SESSION_ID)
        return true;
    return false;
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
        if (!isSession((*it).first))
            cookieString += ("; expires=" + getCookieTime());
        cookieString += CRLF;
    }
    return cookieString;
}

bool ResponseHandler::isRedirectStatusCode()
{
    Response::StatusCodeType statusCode = response.getStatusCode();
    if (statusCode.find("3") == 0)
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

ResponseHandler::ResponseMessageType ResponseHandler::createNormalMessage()
{
    ResponseMessageType responseMessage, startLine, headerLine, body;

    startLine = createStartLine();
    headerLine = createHeaderLine();
    body = createBody();
    responseMessage = pasteAll(startLine, headerLine, body);
    return responseMessage;
}

ResponseHandler::BodyType ResponseHandler::getErrorPageBody()
{
    std::string fileLocation = ERROR_PAGE_LOCATION + response.getStatusCode() + ".html";
    std::ifstream file(fileLocation.c_str());
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

ResponseHandler::ResponseMessageType ResponseHandler::createErrorMessage()
{
    ResponseMessageType errorMessage, startLine, headerLine, body;

    startLine = createStartLine();
    body = getErrorPageBody();
    response.setBody(body); //content Length가 header쪽에서 response body를 보고 바껴서 body와 header 순서를 바꿔줌.
    headerLine = createHeaderLine();
    errorMessage = pasteAll(startLine, headerLine, body);
    return errorMessage;
}

ResponseHandler::ResponseMessageType ResponseHandler::createResponseMessage(const Response &inputResponseMessage)
{
    ResponseMessageType outputResponseMessage;
    
    response = inputResponseMessage;
    if (!isErrorStatusCode())
        outputResponseMessage = createNormalMessage();
    else
        outputResponseMessage = createErrorMessage();
    return outputResponseMessage; 
}