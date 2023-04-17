#include "RequestLexer.hpp"
#include "Request.hpp"
#include <iostream>
#include <stdio.h>

void printTokens(RequestLexer::Tokens tokens)
{
	RequestLexer::Tokens::iterator it = tokens.begin();
	int cnt = 0;
	for (;it != tokens.end(); ++it)
	{
		printf("token[%d](Lexeme, value)>> %d, \"%s\"\n", cnt, (*it).first, (*it).second.c_str());
		cnt++;
	}
}

int main()
{
	// std::string test = "GET /index.html HTTP/1.1\r\nHost: github.co.kr:2000\r\nContent-Length: 30\r\nTransfer-Encoding: gzip, chunked\r\nCookie: session_id=abc123; user_id=12345\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nReferer: http://github.co.kr/\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n[Full request URI: http://github.co.kr/index.html][HTTP request 1/1][Response in frame: 24808]";
	// multipart
	std::string test = "GET /index.html HTTP/1.1\r\nHost: github.co.kr:2000\r\nContent-Length: 30\r\nTransfer-Encoding: gzip, chunked\r\nContent-Type: multipart/form-data; boundary=Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu\r\nCookie: session_id=abc123; user_id=12345\r\nConnection: keep-alive\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/111.0.0.0 Safari/537.36\r\nAccept: image/avif,image/webp,image/apng,image/svg+xml,image/*,*/*;q=0.8\r\nReferer: http://github.co.kr/\r\nAccept-Encoding: gzip, deflate\r\nAccept-Language: ko-KR,ko;q=0.9,en-US;q=0.8,en;q=0.7\r\n\r\n--Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu\r\nContent-Disposition: form-data; name=files; filename=test.txt\r\nContent-Type: application/octet-stream\r\n\r\naaaa\r\n--Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu\r\nContent-Disposition: form-data; name=files; filename=test1.txt\r\nContent-Type: application/octet-stream\r\n\r\n1111\r\n--Uee--r1_eDOWu7FpA0LJdLwCMLJQapQGu--\r\n";
	// std::string test = "ERRORTEST";
	RequestLexer::Tokens tokens = RequestLexer::httpTokenize(test);
	printTokens(tokens);
	return (0);
}

//c++ RequestLexerTester.cpp RequestLexer.cpp Request.cpp