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
#include <sys/types.h>
#include <sys/wait.h>
#include "ResponseHandler.hpp"
#include "RequestHandler.hpp"

void Server::generateServerSocket(SocketAddr socketAddr)
{
	static struct addrinfo	hints = createaddrHints();
	struct addrinfo			*result = NULL;
	const FileDescriptor	fd = createSocket();
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
	return (hints);
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
	FileDescriptor	fd = epoll_create(servers.size() + 1);

	if (fd < 0)	// fd < 0 means system limits has been reached or there is insufficient memory. so server can't run.
		throw (std::runtime_error(std::strerror(errno)));
	return (fd);
}

void Server::controlIOEvent(const FileDescriptor &epoll, const int &option, const FileDescriptor &target, const uint32_t &eventType)
{
	struct epoll_event	event = {.events = eventType, .data = {.fd = target}};

	if (epoll_ctl(epoll, option, target, &event) < 0)
		throw (std::runtime_error(std::strerror(errno))); // here fd < 0 means system limits has been reached or there is insufficient memory. so server can't run.
}

void Server::loopIOEvents(const FileDescriptor &epoll)
{
	struct epoll_event	events[MAX_EVENT_COUNT];
	int					eventCount;
	
	eventCount = waitIOEventOccurrence(epoll, events);
	for (int eventId = 0; eventId < eventCount; eventId++)
		handleIOEvent(epoll, events[eventId]);
}

int Server::waitIOEventOccurrence(const FileDescriptor &epoll, epoll_event *events)
{
	int	eventCount = epoll_wait(epoll, events, MAX_EVENT_COUNT, -1);

	if (eventCount < 0)
		std::cerr << EPOLL_WAIT_EXCEPTION_MESSAGE << std::endl; // this is not a systemic error.
	return (eventCount);
}

void Server::handleIOEvent(const FileDescriptor &epoll, const epoll_event &event)
{
	int			eventFd = event.data.fd;
	uint32_t	eventType = event.events;

	if (servers.find(eventFd) != servers.end())
		return (acceptNewClient(epoll, eventFd));
	if (cgiClients.find(eventFd) != cgiClients.end())
		return (receiveCGI(epoll, eventFd, *(cgiClients[eventFd])));
	if (eventType & EPOLLIN)
		return (receiveRequest(epoll, eventFd, clients[eventFd]));
	if (eventType & EPOLLOUT && clients[eventFd].isComplete())
		return (sendData(epoll, eventFd));
}

void Server::acceptNewClient(const FileDescriptor &epoll, const FileDescriptor &server)
{
	int	client = accept(server, 0, 0);

	if (client < 0)
		std::cerr << ACCEPT_EXCEPTION_MESSAGE << std::endl;
	else
	{
		struct timeval	tv;
		tv.tv_sec = 15;
		setsockopt(client, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(struct timeval *));
		clients.insert(std::make_pair(client, Client()));
		connection.insert(std::make_pair(client, &(servers[server])));
		controlIOEvent(epoll, EPOLL_CTL_ADD, client, (EPOLLIN | EPOLLOUT));
	}
}

void Server::disconnect(const FileDescriptor &epoll, const FileDescriptor &client, const char *reason)
{
	if (clients[client].isDisconnected())
		return;
	clients[client].setDisconnectedState();
	controlIOEvent(epoll, EPOLL_CTL_DEL, client, 0);
	connection.erase(client);
	clients.erase(client);
	close(client);
	std::cerr << reason << std::endl;
}

void Server::receiveRequest(const FileDescriptor &epoll, const FileDescriptor &client, Client &target)
{
	receiveData(epoll, client, target);
	if (target.isComplete())
		return (processRequest(epoll, client, target));
}

void Server::receiveCGI(const FileDescriptor &epoll, const FileDescriptor &pipe, Client &target)
{
	receiveData(epoll, pipe, target);
	if (target.isComplete())
	{
		controlIOEvent(epoll, EPOLL_CTL_DEL, pipe, EPOLLERR);
		cgiClients.erase(pipe);
		ResponseHandler::cgiMessageParsing(const_cast<Response &>(target.getResponseObject()));
		target.setResponseMessage(ResponseHandler::createResponseMessage(target.getResponseObject()));
		while (waitpid(0, 0, WNOHANG) == 0);
	}
}

void Server::processRequest(const FileDescriptor &epoll, const FileDescriptor &client, Client &target)
{
	FileDescriptor	cgiPipe = 0;

	target.setResponseObject(RequestHandler::processRequest(cgiPipe, *(connection[client]), config, const_cast<Request &>(target.getRequestObject())));
	if (cgiPipe == 0)
		return (target.setResponseMessage(ResponseHandler::createResponseMessage(target.getResponseObject())));
	fcntl(cgiPipe, F_SETFL, O_NONBLOCK); // Without this, cgiPipe must be blocking.
	controlIOEvent(epoll, EPOLL_CTL_ADD, cgiPipe, EPOLLIN);
	cgiClients[cgiPipe] = &target;
	target.setCGIState();
}

void Server::sendData(const FileDescriptor &epoll, const FileDescriptor &client)
{
	Client		&target = clients[client];
	const char	*response = target.getResponseMessage();
	ssize_t		sent = send(client, response, std::strlen(response), MSG_DONTWAIT);

	if (sent < 0)
		return (disconnect(epoll, client, SEND_EXCEPTION_MESSAGE)); //TODO: 5xx server error?
	if (target.updateResponsePointer(sent) == CHAR_NULL)
		handleConnection(epoll, client);
		// target.reset();	//TODO:	target의 connection이 close면 연결 닫고, keep alive면 연결 유지 및 reset 호출하게 구현
		//receiveData에서 0을 수신해서 연결을 끊었을 때를 생각해보자. 여기서 connection option을 확인했을 때 그 값이 close여서 disconnect를 다시 호출할 때 문제가 없을까? -> 동작에 문제는 없지만 close가 -1을 반환하고 errno가 EBADF로 설정될 것이다.
}

void Server::receiveData(const FileDescriptor &epoll, const FileDescriptor &fd, Client &target)
{
	char	buffer[BUFFER_SIZE] = {0};	// C99
	ssize_t	received = read(fd, buffer, BUFFER_SIZE - 1);

	if (received < 0)
		return (disconnect(epoll, fd, RECV_EXCEPTION_MESSAGE)); //TODO: 5xx server error?
	target.appendMessage(buffer);
}

void Server::handleConnection(const FileDescriptor &epoll, const FileDescriptor &client)
{
	Client	&target = clients[client];

	if (target.isKeepAlive())
		return (target.reset());
	disconnect(epoll, client, DISCONNECTING_MESSAGE);	
}

Server::Server(const Config config) : config(config)
{ }

void Server::run()
{
	FileDescriptor	epoll;

	initServerSockets();
	epoll = createEpollObject();
	for (ServerMap::const_iterator cIt = servers.begin(); cIt != servers.end(); cIt++)
		controlIOEvent(epoll, EPOLL_CTL_ADD, cIt->first, EPOLLIN);
	while (true)
		loopIOEvents(epoll);
}
