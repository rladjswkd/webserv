#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <sys/epoll.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
//TODO: keep-alive timeout?
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
	servers.insert(std::make_pair(fd, socketAddr));
}

Server::FileDescriptor Server::createSocket()
{
	FileDescriptor	fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

	if (fd < 0)
		throw (std::runtime_error(std::strerror(errno))); // every server socket must be created.
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

Server::FileDescriptor Server::createEpollObject()
{
	FileDescriptor	fd = epoll_create(servers.size());

	if (fd < 0)	// fd < 0 means system limits has been reached or there is insufficient memory. so server can't run.
		throw (std::runtime_error(std::strerror(errno)));
	return (fd);
}

void Server::controlIOEvent(FileDescriptor &epoll, int option, const FileDescriptor &target, uint32_t eventType)
{
	struct epoll_event	event = {.events = eventType, .data.fd = target};

	if (epoll_ctl(epoll, option, target, &event) < 0)
		throw (std::runtime_error(std::strerror(errno))); // here fd < 0 means system limits has been reached or there is insufficient memory. so server can't run.
}

void Server::loopIOEvents(FileDescriptor &epoll)
{
	struct epoll_event	events[MAX_EVENT_COUNT];
	int					eventCount;
	
	eventCount = waitIOEventOccurrence(epoll, events);
	for (int eventId = 0; eventId < eventCount; eventId++)
		handleIOEvent(epoll, events[eventId]);
}

int Server::waitIOEventOccurrence(FileDescriptor &epoll, epoll_event *events)
{
	int	eventCount = epoll_wait(epoll, events, MAX_EVENT_COUNT, -1);

	if (eventCount < 0)
		std::cerr << EPOLL_WAIT_EXCEPTION_MESSAGE << std::endl; // this is not a systemic error.
	return (eventCount);
}

void Server::handleIOEvent(FileDescriptor &epoll, const epoll_event &event)
{
	int			eventFd = event.data.fd;
	uint32_t	eventType = event.events;

	if (servers.find(eventFd) != servers.end())
		return (acceptNewClient(epoll, eventFd));
	if (cgiClients.find(eventFd) != cgiClients.end() && eventType & EPOLLIN)	// cgi pipe
		return (receiveData(epoll, eventFd, *(cgiClients[eventFd])));
	if (eventType & EPOLLIN)													// not cgi. http request
		return (receiveData(epoll, eventFd, clients[eventFd]));
	if (eventType & EPOLLOUT)													// not cgi. http request
		return (sendData(epoll, eventFd));
}

void Server::acceptNewClient(FileDescriptor &epoll, const FileDescriptor &server)
{
	int	client = accept(server, 0, 0);

	if (client < 0)
		std::cerr << ACCEPT_EXCEPTION_MESSAGE << std::endl;
	else
	{
		fcntl(client, F_SETFL, O_NONBLOCK);
		clients.insert(std::make_pair(client, Client()));
		connection.insert(std::make_pair(client, &(servers[server])));
		controlIOEvent(epoll, EPOLL_CTL_ADD, client, (EPOLLIN | EPOLLOUT));
	}
}

void Server::disconnectClient(FileDescriptor &epoll, FileDescriptor &client, const char *reason)
{
	controlIOEvent(epoll, EPOLL_CTL_DEL, client, 0);
	connection.erase(client);
	clients.erase(client);
	close(client);
	std::cerr << reason << std::endl;
}

void Server::sendData(FileDescriptor &epoll, FileDescriptor &client)
{
	Client		&target = clients[client];
	const char	*response = target.getResponseMessage();
	ssize_t		sent = send(client, response, std::strlen(response), 0);

	if (sent < 0)
		return (disconnectClient(epoll, client, SEND_EXCEPTION_MESSAGE)); //TODO: 5xx server error?
	if (target.updateResponsePointer(sent) == 0)
		target.reset();
}

void Server::receiveData(FileDescriptor &epoll, FileDescriptor &fd, Client &target)
{
	char	buffer[BUFFER_SIZE] = {0};	// C99
	ssize_t	received = recv(fd, buffer, BUFFER_SIZE - 1, 0);

	if (received < 0)
		return (disconnectClient(epoll, fd, RECV_EXCEPTION_MESSAGE)); //TODO: 5xx server error?
	if (received == 0)
		return (disconnectClient(epoll, fd, DISCONNECTION_MESSAGE)); //TODO: 5xx server error?
	target.appendMessage(buffer);
}

Server::Server(const Config config) : config(config)
{ }

void Server::run()
{
	FileDescriptor	epoll = createEpollObject();

	initServerSockets();
	for (ServerMap::const_iterator cIt = servers.begin(); cIt != servers.end(); cIt++)
		controlIOEvent(epoll, EPOLL_CTL_ADD, cIt->first, EPOLLIN);
	while (true)
		loopIOEvents(epoll);
}
