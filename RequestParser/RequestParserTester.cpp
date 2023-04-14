#include "RequestParser.hpp"
#include "Request.hpp"
#include <iostream>
#include <stdio.h>

void printRequest(Request request)
{
	printf("method : %d\n", request.getMethod());
	printf("uri : %s\n", request.getUriPath().c_str());
	printf("version : %s\n", request.getHttpVersion().c_str());
	printf("host : %s\n", request.getHost().c_str());
	printf("port : %d\n", request.getPort());
	printf("chunked : %d\n", request.getChunked());
	printf("contentlength : %d\n", request.getContentLength());
	printf("Cookie1 : %s = %s\n", (*request.getCookie().begin()).first.c_str(), (*request.getCookie().begin()).second.c_str());
	printf("Cookie2 : %s = %s\n", ((*(++request.getCookie().begin()))).first.c_str(), (*(++request.getCookie().begin())).second.c_str());
	printf("body : %s\n", request.getBody().c_str());
	
}

int main()
{
	std::string test = "GET /index.html HTTP/1.1\r\nHost: github.co.kr:2000\r\nContent-Length: 30\r\nTransfer-Encoding: gzip, chunked\r\nCookie: session_id=abc123; user_id=12345\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nReferer: http://github.co.kr/\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n[Full request URI: http://github.co.kr/index.html][HTTP request 1/1][Response in frame: 24808]";
	// std::string test = "ERRORTEST";
	RequestLexer::Tokens tokens = RequestLexer::httpTokenize(test);
	Request request = RequestParser::httpParser(tokens);
	printRequest(request);
	return (0);
}

//c++ RequestParserTester.cpp RequestParser.cpp RequestLexer.cpp Request.cpp