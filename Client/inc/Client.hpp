#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <utility>
# include <sys/types.h>
# include "Request.hpp"
# include "Response.hpp"

# define CRLFCRLF							"\r\n\r\n"
# define EMPTY_STRING						""

# define STATE_REQUEST_FIELD_LINE			1
# define STATE_REQUEST_CHUNKED				2
# define STATE_REQUEST_CONTENT_LENGTH		4
# define STATE_RESPONSE_CGI					8
# define STATE_COMPLETE						16

# define CONTENT_LENGTH_EXCEPTION_MESSAGE	"TOO LONG CLIENT MESSAGE BODY!"

class Client
{
public:
	typedef std::string				RequestString, Buffer, Host, Port;
	typedef const char *			ResponseString;
	typedef std::pair<Host, Port>	SocketAddr;

private:
	// SocketAddr		connectedServer;
	unsigned short	state;
	RequestString	request;
	ResponseString	response;
	Request			requestObj;
	Response		responseObj;

private:
	void		appendEssentialPart(const Buffer &newRead);
	void		appendChunked(const Buffer &newRead);
	void		appendContentLength(const Buffer &newRead);
	void		checkMessageBodyFormat();
	std::string	convertToString(const size_t &contentLength);

public:
	// Client(SocketAddr connectedServer);
	Client();
	const char		*getResponseMessage();
	void			setResponseMessage(const Buffer &response);
	char			updateResponsePointer(const ssize_t &sent);
	void			reset();
	void			appendCGI(const Buffer &newRead);
	void			appendMessage(const Buffer &newRead);
	bool			isComplete();
	void			setCGIState();
	// void			setRequestObj(Request requestObj);
	const Request	&getRequestObject();
	void			setResponseObject(Response responseObj);
	const Response	&getResponseObject();

	bool			isKeepAlive();
};
#endif