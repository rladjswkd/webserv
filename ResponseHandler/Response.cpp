#include "Response.hpp"

Response::Response()
: contentType("Content-Type: text/html"), cookie(""), keepAlive(true)
{

}

std::string Response::getStatusCode()
{
    return this->statusCode;
}

void Response::setStatusCode(std::string statusCode)
{
    this->statusCode = statusCode;
}

std::string Response::getContentLength()
{
    return this->contentLength;
}

void Response::setContentLength(std::string contentLength)
{
    this->contentLength = contentLength;
}

std::string Response::getContentType()
{
    return this->contentType;
}

void Response::setContentType(std::string contentType)
{
    this->contentType = contentType;
}

std::string Response::getBody()
{
    return this->body;
}

void Response::setBody(std::string body)
{
    this->body = body;
}

std::string Response::getDate()
{
    return this->date;
}

void Response::setDate(std::string date)
{
    this->date = date;
}

std::string Response::getLocation()
{
    return this->location;
}

void Response::setLocation(std::string location)
{
    this->location = location;
}

std::string Response::getCookie()
{
    return this->cookie;
}

void Response::setCookie(std::string cookie)
{
    this->cookie = cookie;
}

bool Response::isKeepAlive()
{
    return this->keepAlive;    
}

void Response::setKeepAlive(bool keepAlive)
{
    this->keepAlive = keepAlive;
}

