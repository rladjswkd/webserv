#include "Client.hpp"
#include "RequestParser.hpp"
#include "RequestLexer.hpp"
#include <stdexcept>
#include <sstream>

Client::Client() : state(STATE_REQUEST_FIELD_LINE), message(EMPTY_STRING)
{ }

void Client::appendEssentialPart(const Message &newRead)
{
	size_t  startPos = message.size() - 3;  // "3" means \r\n\r was in previous buffer and last \n is in current buffer.
	size_t	headerLineEndPos;

	if (message.size() < startPos)
		startPos = 0;
	message.append(newRead);
	headerLineEndPos = message.find(CRLFCRLF, startPos);
	if (headerLineEndPos != std::string::npos)
		checkMessageBodyFormat(headerLineEndPos + 4);
}

void Client::appendChunked(const Message &newRead)
{
	message.append(newRead);
	if (message.find(CRLFCRLF, message.size() - 3) != std::string::npos)
	{
		RequestParser::bodyLineParsing(RequestLexer::bodyLineTokenize(message.toString()), requestObj);
		state = STATE_COMPLETE;
	}
}

void Client::appendContentLength(const Message &newRead)
{
	message.append(newRead);
	if (message.size() >= requestObj.getContentLength())
	{
		RequestParser::bodyLineParsing(RequestLexer::bodyLineTokenize(message.toString()), requestObj);
		state = STATE_COMPLETE;
	}
}

void Client::checkMessageBodyFormat(const size_t &headerLineEndPos)
{
	RequestLexer::Tokens tokens = RequestLexer::startLineHeaderLineTokenize(message.buffer);

	state = STATE_COMPLETE;
	RequestParser::startLineHeaderLineParsing(tokens, requestObj);
	message.erase(0, headerLineEndPos);
	if (requestObj.getChunked())
	{
		state = STATE_REQUEST_CHUNKED;
		return (appendChunked(EMPTY_STRING));
	}
	if (requestObj.getContentLength())
	{
		state = STATE_REQUEST_CONTENT_LENGTH;
		return (appendContentLength(EMPTY_STRING));
	}
}

std::string Client::convertToString(const size_t &contentLength)
{
	std::stringstream	ss;

	ss << contentLength;
	return (ss.str());
}

void Client::appendCGI(const Message &newRead)
{
	static size_t contentLength = 0;

	if (newRead.size() == 0) // this means that client has closed its socket.
	{
		responseObj.setBody(message.toString());
		responseObj.setContentLength(convertToString(contentLength));
		contentLength = 0;
		state = STATE_COMPLETE;
		return;
	}
	message.append(newRead);
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
	message.clear();
	state = STATE_RESPONSE_CGI;
}

void Client::setDisconnectedState()
{
	state = STATE_DISCONNECTED;
}

bool Client::isDisconnected()
{
	return (state == STATE_DISCONNECTED);
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

const char *Client::getResponseMessageBuffer()
{
	return (message.buffer.c_str() + message.sent);
}

size_t Client::getResponseLengthToSend()
{
	return (message.buffer.size() - message.sent);
}

size_t Client::updateMessageBuffer(const size_t sent)
{
	message.sent += sent;
	return (getResponseLengthToSend());
}

void Client::setResponseMessageBuffer(const Message &response)
{
	this->message = MessageBuffer(response);
}

void Client::reset()
{
	state = STATE_REQUEST_FIELD_LINE;
	message.clear();
	// response = 0;
	requestObj = Request();
	responseObj = Response();
}

Client::MessageBuffer::MessageBuffer(const std::string &response) : buffer(response), sent()
{
}

size_t Client::MessageBuffer::size() const
{
	return (buffer.size());
}

void Client::MessageBuffer::append(const std::string &str)
{
	buffer.append(str);
}

size_t Client::MessageBuffer::find(const std::string &str, size_t pos)
{
	return (buffer.find(str, pos));
}

void Client::MessageBuffer::clear()
{
	buffer.clear();
}

void Client::MessageBuffer::erase(size_t pos, size_t n)
{
	buffer.erase(pos, n);
}

const std::string &Client::MessageBuffer::toString()
{
	return (buffer);
}