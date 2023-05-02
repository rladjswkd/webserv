#include "Client.hpp"
#include "RequestParser.hpp"
#include "RequestLexer.hpp"
#include <stdexcept>
#include <sstream>

void Client::appendEssentialPart(const Buffer &newRead)
{
	size_t  startPos = request.size() - 3;  // "3" means \r\n\r was in previous buffer and last \n is in current buffer.

	request.append(newRead);
	if (request.find(CRLFCRLF, startPos) == std::string::npos)
		checkMessageBodyFormat();
}

void Client::appendChunked(const Buffer &newRead)
{
	size_t					startPos = request.size() - 3;
	RequestLexer::Tokens	tokens;

	request.append(newRead);
	if (request.find(CRLFCRLF, startPos) != std::string::npos)
	{
		tokens = RequestLexer::bodyLineTokenize(request);
		RequestParser::bodyLineParsing(tokens, requestObj);
		request.clear();
		state = STATE_COMPLETE;
	}
}

void Client::appendContentLength(const Buffer &newRead)
{
	static size_t			contentLength = requestObj.getContentLength();
	RequestLexer::Tokens	tokens;

	request.append(newRead);
	if (request.size() >= contentLength)
	{
		tokens = RequestLexer::bodyLineTokenize(request);
		RequestParser::bodyLineParsing(tokens, requestObj);
		request.clear();
		state = STATE_COMPLETE;
	}
}

void Client::checkMessageBodyFormat()
{
	RequestLexer::Tokens	tokens = RequestLexer::startLineHeaderLineTokenize(request);

	requestObj = RequestParser::startLineHeaderLineParsing(tokens);
	if (requestObj.getChunked())
		state = STATE_REQUEST_CHUNKED;
	else if (requestObj.getContentLength())
		state = STATE_REQUEST_CONTENT_LENGTH;
	request.clear();
}

std::string Client::convertToString(const size_t &contentLength)
{
	std::stringstream	ss;

	ss << contentLength;
	return (ss.str());
}

void Client::appendCGI(const Buffer &newRead)
{
	static size_t	contentLength = 0;
	static Buffer	body;

	if (newRead.size() == 0)	// this means that client has closed its socket.
	{
		responseObj.setBody(body);
		responseObj.setContentLength(convertToString(contentLength));
		contentLength = 0;
		state = STATE_COMPLETE;
		return;
	}
	body.append(newRead);
	contentLength += newRead.size();
}

// Client::Client(SocketAddr connectedServer) : connectedServer(connectedServer)
// { }

void Client::appendMessage(const Buffer &newRead)
{
	switch (state)
	{
		case STATE_REQUEST_FIELD_LINE:
			return (appendEssentialPart(newRead));
		case STATE_REQUEST_CHUNKED:
			return (appendChunked(newRead));
		case STATE_REQUEST_CONTENT_LENGTH:
			return (appendContentLength(newRead));
		case STATE_RESPONSE_CGI:
			return (appendCGI(newRead));
	}
}

bool Client::isComplete()
{
	return (state == STATE_COMPLETE);
}

void Client::setCGIState()
{
	state = STATE_RESPONSE_CGI;
}

// void Client::setRequestObj(Request requestObj)
// {
// 	this->requestObj = requestObj;
// }

const Request &Client::getRequestObject()
{
	return (requestObj);
}

void Client::setResponseObject(Response responseObj)
{
	this->responseObj = responseObj;
}

const Response &Client::getResponseObject()
{
	return (responseObj);
}

const char *Client::getResponseMessage()
{
	return (response);
}

void Client::setResponseMessage(const Buffer &response)
{
	this->response = response.c_str();
}

const char	Client::updateResponsePointer(const ssize_t &sent)
{
	response += sent;
	return (*response);
}

//TODO: requestObj, responseObj도 초기화해야 하나? 이 둘은 이 클래스에서 다루진 않고 외부 클래스가 반환하는 값을 저장할 뿐이므로 초기화는 필요없어 보인다.
void Client::reset()
{
	state = STATE_REQUEST_FIELD_LINE;
	request.clear();
	response = EMPTY_STRING;
}
