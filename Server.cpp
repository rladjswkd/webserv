#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cstring>
#include <errno.h>

void Server::generateServerSocket(SocketAddr socketAddr)
{
	static struct addrinfo	hints = createaddrHints();
	struct addrinfo			*result = NULL;
	FileDescriptor			fd = createSocket();
	int						bindError;

	if (getaddrinfo(extractNumericHost(socketAddr.first), socketAddr.second.c_str(), &hints, &result) != 0)
		throwException(result);
	bindError = bind(fd, result->ai_addr, sizeof(sockaddr_in));
	if (bindError < 0 && errno == EADDRINUSE)	// *:8080과 127.0.0.1:8080이 configuration file에 선언돼있다면 127.0.0.1:8080은 무시하고 넘어가기 위함.
		return;
	if (bindError < 0)
		throwException(result);
	if (listen(fd, SOMAXCONN) < 0)
		throwException(result);
	freeaddrinfo(result);
	socketSet.insert(fd);
}

Server::FileDescriptor Server::createSocket()
{
	// FileDescriptor	fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);
	FileDescriptor	fd = socket(AF_INET, SOCK_STREAM, 0);

	if (fd < 0)
		throw (std::runtime_error(std::strerror(errno)));
	return (fd);
}

addrinfo Server::createaddrHints()
{
	struct addrinfo	hints;

	std::memset(&hints, 0, sizeof(addrinfo));
	hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST | AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	return(hints);
}

void Server::throwException(addrinfo *info)
{
	freeaddrinfo(info);
	throw (std::runtime_error(std::strerror(errno)));
}

const char	*Server::extractNumericHost(const Host &host)
{
    if (host == LOCALHOST_STR)
		return (LOCALHOST_NUMERIC_STR);
	if (host == ASTERISK_STR)
		return (ASTERISK_NUMERIC_STR);
	return (host.c_str());
}

void Server::initServerSockets()
{
	for (Config::const_iterator cIt = config.begin(); cIt != config.end(); cIt++)
		generateServerSocket(cIt->first);
}

Server::Server(const Config config) : config(config)
{}

void Server::run()
{
	initServerSockets();
}
