#include "Request.hpp"

Request::method_enum Request::getMethod() const {
    return method;
}

void Request::setMethod(method_enum method) {
    method = method;
}


std::string Request::getUriPath() const {
    return uriPath;
}

void Request::setUriPath(std::string uriPath) {
    uriPath = uriPath;
}


std::string Request::getQueryString() const {
    return queryString;
}

void Request::setQueryString(std::string queryString) {
    queryString = queryString;
}


std::string Request::getHttpVersion() const {
    return httpVersion;
}

void Request::setHttpVersion(std::string httpVersion) {
    httpVersion = httpVersion;
}


Request::HeaderFieldsMap Request::getHeaderFields() const {
    return headerFields;
}

void Request::setHeaderFields(HeaderFieldsMap headerFields) {
    headerFields = headerFields;
}


Request::HostType Request::getHost() const {
    return host;
}

void Request::setHost(HostType host) {
    host = host;
}

std::string Request::getTransferEncoding() const {
    return transferEncoding;
}

void Request::setTransferEncoding(std::string transferEncoding) {
    transferEncoding = transferEncoding;
}

uint32_t Request::getContentLength() const {
    return contentLength;
}

void Request::setContentLength(uint32_t contentLength) {
    contentLength = contentLength;
}

Request::CookieType Request::getCookie() const {
    return cookie;
}

void Request::setCookie(CookieType cookie) {
    cookie = cookie;
}

std::string Request::getBody() const {
    return body;
}

void Request::setBody(std::string body) {
    body = body;
}