#include "RequestParser.hpp"
#include "Request.hpp"
#include <iostream>
#include <stdio.h>

void printRequest(Request request)
{
	if (request.getErrorCode().size() > 0)
		return ;
	printf("method : \"%d\"\n", request.getMethod());
	printf("uri : \"%s\"\n", request.getUriPath().c_str());
	printf("version : \"%s\"\n", request.getHttpVersion().c_str());
	printf("host : \"%s\"\n", request.getHost().c_str());
	printf("port : \"%d\"\n", request.getPort());
	printf("chunked : \"%d\"\n", request.getChunked());
	printf("contentlength : \"%d\"\n", request.getContentLength());
	printf("Cookie1 : %s = \"%s\"\n", (*request.getCookie().begin()).first.c_str(), (*request.getCookie().begin()).second.c_str());
	printf("Cookie2 : %s = \"%s\"\n", ((*(++request.getCookie().begin()))).first.c_str(), (*(++request.getCookie().begin())).second.c_str());
	printf("multitype-form-data : \"%s\"\n", request.getMultipartFormDataId().c_str());
	printf("body : \"%s\"\n", request.getBody().c_str());
	printf("other headers============================\n");

	Request::HeaderFieldsMap headerField = request.getHeaderFields();
	Request::HeaderFieldsMap::iterator it = headerField.begin();
	for(; it != headerField.end(); ++it)
	{
		printf("%s : \"%s\"\n",(*it).first.c_str(), (*it).second.c_str());
	}
}

int main()
{
	// normal
	// std::string test = "GET /index.html HTTP/1.1\r\nHost: github.co.kr:2000\r\nContent-Length: 30\r\nTransfer-Encoding: gzip, chunked\r\nCookie: session_id=abc123; user_id=12345\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nReferer: http://github.co.kr/\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n[Full request URI: http://github.co.kr/index.html][HTTP request 1/1][Response in frame: 24808]";
	
	// chunked test
	// std::string test = "GET /index.html HTTP/1.1\r\nHost: github.co.kr:2000\r\nContent-Length: 30\r\nTransfer-Encoding: gzip, chunked\r\nCookie: session_id=abc123; user_id=12345\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nReferer: http://github.co.kr/\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\nB\r\nLorem ipsum\r\n10C\r\ndolor sit amet,\n consectetur adipiscing elit. Suspendisse aliquet urna eu mi faucibus, nec eleifend est lobortis. Fusce pulvinar massa metus, sed eleifend nisl venenatis vitae. Aliquam vel elit quis nulla dapibus lacinia. Fusce sed erat vel tellus tristique fringilla.\r\n0\r\n\r\n";

	// multipart test	
	std::string test = "GET /index.html HTTP/1.1\r\nHost: github.co.kr:2000\r\nContent-Length: 30\r\nTransfer-Encoding: gzip, chunked\r\nContent-Type: multipart/form-data; boundary=Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu\r\nCookie: session_id=abc123; user_id=12345\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nReferer: http://github.co.kr/\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n--Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu\r\nContent-Disposition: form-data; name=files; filename=test.txt\r\nContent-Type: application/octet-stream\r\n\r\naaaa\n\r\n--Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu\r\nContent-Disposition: form-data; name=files; filename=test1.txt\r\nContent-Type: application/octet-stream\r\n\r\n1111 number check \n\r\n--Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu\r\nContent-Disposition: form-data; name=files; filename=test1.txt\r\nContent-Type: application/octet-stream\r\n\r\n  TestLine here.\r\n--Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu--\r\n";


	// std::string test = "ERRORTEST";
	RequestLexer::Tokens tokens = RequestLexer::httpTokenize(test);
	Request request = RequestParser::httpParser(tokens);
	printRequest(request);
	return (0);
}

//c++ RequestParserTester.cpp RequestParser.cpp RequestLexer.cpp Request.cpp