#include "Response.hpp"

Response::Response()
: contentType("text/html"), cookie(""), keepAlive(true)
{

}

std::string Response::getStatusCode() const
{
    return this->statusCode;
}

void Response::setStatusCode(std::string statusCode)
{
    this->statusCode = statusCode;
}

std::string Response::getContentLength() const
{
    return this->contentLength;
}

void Response::setContentLength(std::string contentLength)
{
    this->contentLength = contentLength;
}

std::string Response::getContentType() const
{
    return this->contentType;
}

void Response::setContentType(std::string contentType)
{
    this->contentType = contentType;
}

std::string Response::getBody() const
{
    return this->body;
}

void Response::setBody(std::string body)
{
    this->body = body;
}

std::string Response::getDate() const
{
    return this->date;
}

void Response::setDate(std::string date)
{
    this->date = date;
}

std::string Response::getLocation() const
{
    return this->location;
}

void Response::setLocation(std::string location)
{
    this->location = location;
}

std::string Response::getCookie() const
{
    return this->cookie;
}

void Response::setCookie(std::string cookie)
{
    this->cookie = cookie;
}

bool Response::isKeepAlive() const
{
    return this->keepAlive;    
}

void Response::setKeepAlive(bool keepAlive)
{
    this->keepAlive = keepAlive;
}

