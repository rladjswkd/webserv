#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>
# include <utility>
# include <sys/types.h>
# include "Request.hpp"
# include "Response.hpp"

# define CRLFCRLF							"\r\n\r\n"
# define EMPTY_STRING						""
# define STATE_REQUEST_HEAD			1
# define STATE_REQUEST_CHUNKED				2
# define STATE_REQUEST_CONTENT_LENGTH		4
# define STATE_RESPONSE_CGI					8
# define STATE_COMPLETE						16
# define STATE_DISCONNECTED					32
# define STATE_CGI_ERROR			64

# define CONTENT_LENGTH_EXCEPTION_MESSAGE	"TOO LONG CLIENT MESSAGE BODY!"

class Client
{
private:
	class MessageBuffer
	{
	public:
		std::string	buffer;
		size_t		sent;

		MessageBuffer(const std::string &response);
		void				append(const std::string &str);
		size_t				find(const std::string &str, size_t pos);
		void				clear();
		void				erase(size_t pos, size_t n);
		size_t				size() const;
		const std::string	&toString();
	};

public:
	typedef std::string				Message, Host, Port;
	typedef std::pair<Host, Port>	SocketAddr;

private:
	unsigned short	state;
	MessageBuffer	message;
	Request			requestObj;
	Response		responseObj;
	pid_t				pid;

private:
	void		appendEssentialPart(const Message &newRead);
	void		appendChunked(const Message &newRead);
	void		appendContentLength(const Message &newRead);
	void		checkMessageBodyFormat(const size_t &headerLineEndPos);
	std::string	convertToString(const size_t &contentLength);

public:
	Client();

	void				setResponseMessageBuffer(const Message &response);
	const char			*getResponseMessageBuffer();
	size_t				getResponseLengthToSend();
	size_t				updateMessageBuffer(const size_t sent);

	void				appendCGI(const Message &newRead);
	void				appendMessage(const Message &newRead);

	void				setResponseObject(Response responseObj);
	const Request		&getRequestObject();
	const Response		&getResponseObject();
	
	void				setCGIState();
	void				setDisconnectedState();
	void				setCGIErrorState();
	bool				isComplete();
	bool				isDisconnected();
	bool				isCGIError();

	bool				isKeepAlive();
	void				reset();

	void				setCGIPID(pid_t pid);
	pid_t				getCGIPID();
};
#endif