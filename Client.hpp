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
	typedef std::string				RequestString, Message, Host, Port;
	typedef char *					ResponseString;
	typedef std::pair<Host, Port>	SocketAddr;

private:
	// SocketAddr		connectedServer;
	unsigned short	state;
	RequestString	request;
	ResponseString	response;
	Request			requestObj;
	Response		responseObj;

private:
	void		appendEssentialPart(const Message &newRead);
	void		appendChunked(const Message &newRead);
	void		appendContentLength(const Message &newRead);
	void		checkMessageBodyFormat();
	std::string	convertToString(const size_t &contentLength);

public:
	// Client(SocketAddr connectedServer);
	const char		*getResponseMessage();

	const char		updateResponsePointer(const ssize_t &sent);
	void			reset();
	void			appendCGI(const Message &newRead);
	void			appendMessage(const Message &newRead);
	
	void			setRequestObj(Request requestObj);
	const Request	&getRequestObj();
	void			setResponseObj(Response responseObj);
	const Response	&getResponseObj();
};
#endif