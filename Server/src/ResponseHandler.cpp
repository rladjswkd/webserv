#include "ResponseHandler.hpp"

Response ResponseHandler::response;

std::string ResponseHandler::ft_toLower(std::string str)
{
  for (unsigned int i = 0; i < str.length(); i++) {
    str[i] = std::tolower(str[i]);
  }
	return str;
}

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

ResponseHandler::CookieStringType ResponseHandler::getCookieString()
{
    ResponseHandler::CookieStringType cookieString = "";
    cookieString = response.getCookie();
    if (cookieString.size() > 0)
        cookieString += CRLF;
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

bool ResponseHandler::isKeepAlive()
{
    return response.isKeepAlive();
}

ResponseHandler::KeepAliveType ResponseHandler::createKeepAlive()
{
    std::string connection;

    if (isKeepAlive())
        connection = "Connection: Keep-Alive";
    else
        connection = "Connection: close";
    connection += CRLF;
    return connection;
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
    headerLine += response.getContentType();
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

    //Connection(keepAlive)
    headerLine += createKeepAlive();
    
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
    // body = getErrorPageBody();
    // response.setBody(body); //content Length가 header쪽에서 response body를 보고 바껴서 body와 header 순서를 바꿔줌.
    body = response.getBody();
    headerLine = createHeaderLine();
    errorMessage = pasteAll(startLine, headerLine, body);
    return errorMessage;
}

ResponseHandler::ResponseMessageType ResponseHandler::createResponseMessage(const Response &inputResponseMessage)
{
    ResponseMessageType outputResponseMessage;
    
    response = inputResponseMessage;
    // if (!isErrorStatusCode())
    outputResponseMessage = createNormalMessage();
    // else
    //     outputResponseMessage = createErrorMessage();
    return outputResponseMessage; 
}


bool ResponseHandler::isSetCookieHeader(std::string str)
{
    return (ft_toLower(str).find("set-cookie: ") == 0);
}

bool ResponseHandler::isContentTypeHeader(std::string str)
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
        inputResponse.setContentType(temp);
      ss.get();
    }
    cgiBodySetting(inputResponse);
}
