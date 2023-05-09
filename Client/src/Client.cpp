#include "Client.hpp"
#include "RequestParser.hpp"
#include "RequestLexer.hpp"
#include <stdexcept>
#include <sstream>

Client::Client() : state(STATE_REQUEST_FIELD_LINE)
{ }

void Client::appendEssentialPart(const Message &newRead)
{
	size_t  startPos = message.size() - 3;  // "3" means \r\n\r was in previous buffer and last \n is in current buffer.

	if (message.size() < startPos)
		startPos = 0;
	message.append(newRead);
	if (message.find(CRLFCRLF, startPos) != std::string::npos)
		checkMessageBodyFormat();
}

void Client::appendChunked(const Message &newRead)
{
	size_t					startPos = message.size() - 3;
	RequestLexer::Tokens	tokens;

	message.append(newRead);
	if (message.find(CRLFCRLF, startPos) != std::string::npos)
	{
		tokens = RequestLexer::bodyLineTokenize(message);
		RequestParser::bodyLineParsing(tokens, requestObj);
		state = STATE_COMPLETE;
	}
}

void Client::appendContentLength(const Message &newRead)
{
	RequestLexer::Tokens	tokens;

	message.append(newRead);
	if (message.size() >= requestObj.getContentLength())
	{
		tokens = RequestLexer::bodyLineTokenize(message);
		RequestParser::bodyLineParsing(tokens, requestObj);
		state = STATE_COMPLETE;
	}
}

void Client::checkMessageBodyFormat()
{
	RequestLexer::Tokens	tokens = RequestLexer::startLineHeaderLineTokenize(message);

	requestObj = RequestParser::startLineHeaderLineParsing(tokens);
	if (requestObj.getChunked())
		state = STATE_REQUEST_CHUNKED;
	else if (requestObj.getContentLength())
		state = STATE_REQUEST_CONTENT_LENGTH;
	else
		state = STATE_COMPLETE;
}

std::string Client::convertToString(const size_t &contentLength)
{
	std::stringstream	ss;

	ss << contentLength;
	return (ss.str());
}

void Client::appendCGI(const Message &newRead)
{
	static size_t	contentLength = 0;
	static Message	body;

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

void Client::appendMessage(const Message &newRead)
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

void Client::setDisconnectedState()
{
	state = STATE_DISCONNECTED;
}

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

bool Client::isKeepAlive()
{
	return (responseObj.isKeepAlive());
}

const char *Client::getResponseMessage()
{
	return (response);
}

void Client::setResponseMessage(const Message &response)
{
	this->message = response;
	this->response = message.c_str();
}

char	Client::updateResponsePointer(const ssize_t &sent)
{
	response += sent;
	return (*response);
}

//TODO: requestObj, responseObj도 초기화해야 하나? 이 둘은 이 클래스에서 다루진 않고 외부 클래스가 반환하는 값을 저장할 뿐이므로 초기화는 필요없어 보인다.
void Client::reset()
{
	state = STATE_REQUEST_FIELD_LINE;
	message.clear();
	response = 0;
}
