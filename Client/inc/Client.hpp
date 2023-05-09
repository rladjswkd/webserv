#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <utility>
# include <sys/types.h>
# include "Request.hpp"
# include "Response.hpp"

# define CRLFCRLF							"\r\n\r\n"

# define STATE_REQUEST_FIELD_LINE			1
# define STATE_REQUEST_CHUNKED				2
# define STATE_REQUEST_CONTENT_LENGTH		4
# define STATE_RESPONSE_CGI					8
# define STATE_COMPLETE						16
# define STATE_DISCONNECTED					32

# define CONTENT_LENGTH_EXCEPTION_MESSAGE	"TOO LONG CLIENT MESSAGE BODY!"

class Client
{
public:
	typedef std::string				Message, Host, Port;
	typedef const char *			ResponsePointer;
	typedef std::pair<Host, Port>	SocketAddr;

private:
	// SocketAddr		connectedServer;
	unsigned short	state;
	Message			message;
	ResponsePointer	response;
	Request			requestObj;
	Response		responseObj;

private:
	void		appendEssentialPart(const Message &newRead);
	void		appendChunked(const Message &newRead);
	void		appendContentLength(const Message &newRead);
	void		checkMessageBodyFormat();
	std::string	convertToString(const size_t &contentLength);

public:
	Client();
	const char		*getResponseMessage();
	void			setResponseMessage(const Message &response);
	char			updateResponsePointer(const ssize_t &sent);
	void			reset();
	void			appendCGI(const Message &newRead);
	void			appendMessage(const Message &newRead);
	bool			isComplete();
	void			setCGIState();
	void			setDisconnectedState();
	
	const Request	&getRequestObject();
	void			setResponseObject(Response responseObj);
	const Response	&getResponseObject();

	bool			isKeepAlive();
};
#endif