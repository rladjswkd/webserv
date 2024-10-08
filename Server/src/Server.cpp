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
#include <algorithm>
#include "ResponseHandler.hpp"
#include "RequestHandler.hpp"

void Server::generateServerSocket(const SocketAddr &socketAddr)
{
	struct addrinfo *result = getAvailableAddress(socketAddr);
	// const FileDescriptor socket = createSocket();
	const FileDescriptor fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

	if (fd < 0)
	{
		freeaddrinfo(result);
		throw(std::runtime_error(std::strerror(errno)));
	}
	doBind(fd, result);
	doListen(fd, result);
	freeaddrinfo(result);
	servers.insert(std::make_pair(fd, socketAddr));
}

addrinfo *Server::getAvailableAddress(const SocketAddr &socketAddr)
{
	static struct addrinfo hints = createaddrHints();
	struct addrinfo *result = NULL;
	int retVal = getaddrinfo(extractNumericHost(socketAddr.first), socketAddr.second.c_str(), &hints, &result);

	if (retVal != 0)
	{
		freeaddrinfo(result);
		throw(std::runtime_error(gai_strerror(retVal)));
	}
	return (result);
}

void Server::doBind(const FileDescriptor socket, addrinfo *result)
{
	if (bind(socket, result->ai_addr, sizeof(sockaddr_in)) == 0 || errno == EADDRINUSE) // *:8080과 127.0.0.1:8080이 configuration file에 선언돼있다면 127.0.0.1:8080은 무시하고 넘어가기 위함.
		return;
	freeaddrinfo(result);
	throw(std::runtime_error(std::strerror(errno)));
}

void Server::doListen(const FileDescriptor socket, addrinfo *result)
{
	if (listen(socket, SOMAXCONN) == 0)
		return;
	freeaddrinfo(result);
	throw(std::runtime_error(std::strerror(errno)));
}

// Server::FileDescriptor Server::createSocket()
// {
// 	FileDescriptor fd = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0);

// 	if (fd < 0)
// 		throw(std::runtime_error(std::strerror(errno))); // every server socket must be created.
// 	return (fd);
// }

addrinfo Server::createaddrHints()
{
	struct addrinfo hints;

	std::memset(&hints, 0, sizeof(addrinfo));
	// getaddrinfo 호출 시 node가 NULL이 아니므로 AI_PASSIVE는 무시된다.
	// AI_ADDRCONFIG 플래그는 로컬 시스템에 IPv4 주소가 하나 이상 구성되어있으면 IPv4 주소를 반환하고, IPv6 주소가 하나 이상 구성되어있으면 IPv6 주소를 반환한다.
	// 둘 다 구성되어있다면 가능한 IPv4, IPv6 체계의 주소들을 반환한다.
	// AF_INET6를 지정하지 않고 AF_INET만 지정한 경우 IPv4 주소만 반환한다.
	// 여기선 AI_ADDRCONFIG를 지정했으므로, 만약 로컬 시스템에 IPv4 주소가 구성되지 않고 IPv6 주소만 구성되었다면 NULL을 반환할 것이다.
	hints.ai_flags = AI_ADDRCONFIG | AI_NUMERICHOST | AI_PASSIVE;
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;
	return (hints);
}

const char *Server::extractNumericHost(const Host &host)
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
	FileDescriptor fd = epoll_create(servers.size() + 1);

	if (fd < 0) // fd < 0 means system limits has been reached or there is insufficient memory. so server can't run.
		throw(std::runtime_error(std::strerror(errno)));
	return (fd);
}

void Server::controlIOEvent(const FileDescriptor &epoll, const int &option, const FileDescriptor &target, const uint32_t &eventType)
{
	struct epoll_event event = {.events = eventType, .data = {.fd = target}};

	if (epoll_ctl(epoll, option, target, &event) < 0)
		throw(std::runtime_error(std::strerror(errno))); // here fd < 0 means system limits has been reached or there is insufficient memory. so server can't run.
}

void Server::loopIOEvents(const FileDescriptor &epoll)
{
	struct epoll_event events[MAX_EVENT_COUNT];
	int eventCount;

	eventCount = waitIOEventOccurrence(epoll, events);
	for (int eventId = 0; eventId < eventCount; eventId++)
		handleIOEvent(epoll, events[eventId]);
}

int Server::waitIOEventOccurrence(const FileDescriptor &epoll, epoll_event *events)
{
	int eventCount = epoll_wait(epoll, events, MAX_EVENT_COUNT, -1);

	if (eventCount < 0)
		std::cerr << EPOLL_WAIT_EXCEPTION_MESSAGE << std::endl; // this is not a systemic error.
	return (eventCount);
}

void Server::handleIOEvent(const FileDescriptor &epoll, const epoll_event &event)
{
	int eventFd = event.data.fd;
	uint32_t eventType = event.events;

	if (servers.find(eventFd) != servers.end())
		return (acceptNewClient(epoll, eventFd));
	if (cgiClients.find(eventFd) != cgiClients.end())
		return (receiveCGI(epoll, eventFd, *(cgiClients[eventFd])));
	if (eventType & EPOLLIN)
		return (receiveRequest(epoll, eventFd, clients[eventFd]));
	if (eventType & EPOLLOUT && clients[eventFd].isComplete())
		return (sendData(epoll, eventFd));
	if (eventType & EPOLLOUT && clients[eventFd].isCGIError())
		return (disconnect(epoll, eventFd, CGI_EXCEPTION_MESSAGE));
}

void Server::acceptNewClient(const FileDescriptor &epoll, const FileDescriptor &server)
{
	int client = accept(server, 0, 0);

	if (client < 0)
		std::cerr << ACCEPT_EXCEPTION_MESSAGE << std::endl;
	else
	{
		struct timeval tv;
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
	controlIOEvent(epoll, EPOLL_CTL_DEL, client, EPOLLIN);
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
	receiveCGIData(epoll, pipe, target);
	// CGI 응답 대기 중 클라이언트가 소켓 연결을 끊어 클라이언트 연결 정보가 사라졌지만, pipe 등 CGI 관련 정보는 살아있어 이벤트가 발생한 경우
	if (target.isDisconnected())
		disconnectPipe(epoll, pipe);
	if (target.isComplete())
	{
		ResponseHandler::cgiMessageParsing(const_cast<Response &>(target.getResponseObject()));
		target.setResponseMessageBuffer(ResponseHandler::createResponseMessage(target.getResponseObject()));
		disconnectPipe(epoll, pipe);
	}
}

void Server::processRequest(const FileDescriptor &epoll, const FileDescriptor &client, Client &target)
{
	CGIData cgi;

	target.setResponseObject(RequestHandler::processRequest(cgi, config.getServer(*connection[client], target.getRequestObject().getHost()), const_cast<Request &>(target.getRequestObject())));
	if (cgi.pipe == 0)
		return (target.setResponseMessageBuffer(ResponseHandler::createResponseMessage(target.getResponseObject())));
	if (cgi.pipe == -1)
		return (disconnect(epoll, client, PIPE_WRITE_EXCEPTION_MESSAGE));
	fcntl(cgi.pipe, F_SETFL, O_NONBLOCK); // Without this, cgi.pipe must be blocking.
	controlIOEvent(epoll, EPOLL_CTL_ADD, cgi.pipe, EPOLLIN);
	cgiClients[cgi.pipe] = &target;
	target.setCGIState();
	target.setCGIPID(cgi.pid);
}

void Server::sendData(const FileDescriptor &epoll, const FileDescriptor &client)
{
	Client &target = clients[client];
	const char *response = target.getResponseMessageBuffer();
	ssize_t sent = send(client, response, target.getResponseLengthToSend(), MSG_DONTWAIT);

	if (sent < 0)
		return (disconnect(epoll, client, SEND_EXCEPTION_MESSAGE));
	if (target.updateMessageBuffer(sent) == 0)
		handleConnection(epoll, client);
}

void Server::receiveData(const FileDescriptor &epoll, const FileDescriptor &fd, Client &target)
{
	char buffer[BUFFER_SIZE] = {0}; // C99
	ssize_t received = read(fd, buffer, BUFFER_SIZE - 1);

	if (received < 0)
		return (disconnect(epoll, fd, RECV_EXCEPTION_MESSAGE));
	if (!received)
		return (disconnect(epoll, fd, DISCONNECTED_MESSAGE));
	target.appendMessage(std::string(buffer, received));
}

void Server::receiveCGIData(const FileDescriptor &epoll, const FileDescriptor &fd, Client &target)
{
	char buffer[BUFFER_SIZE] = {0};
	ssize_t received = read(fd, buffer, BUFFER_SIZE - 1);

	// 오류 발생 시 파이프 해제 및 클라이언트 해제 필요.
	// 클라이언트의 파일 디스크립터 정보가 없으므로 이 코드에선 클라이언트를 해제할 수 없다.
	// disconnectPipe를 호출하고 target에 대해 setCGIErrorState()를 호출하면,
	// 이후 클라이언트에 대해 EPOLLOUT 이벤트가 발생할 때 handleIOEvent 함수의 마지막 if 문에 걸려 클라이언트가 해제되긴 한다.
	// 대신 그때 전달할 메시지를 조금 더 넓은 범위에서 CGI_EXCEPTION_MESSAGE 등을 정의해 사용할 필요가 있다.
	if (received < 0)
	{
		target.setCGIErrorState();
		return (disconnectPipe(epoll, fd));
	}
	// CGI 스크립트 프로세스 정상 종료 및 CGI 응답 수신 완료 시 파이프 해제 필요.
	if (!received)
		return (disconnectPipe(epoll, fd));
	target.appendMessage(std::string(buffer, received));
}

void Server::handleConnection(const FileDescriptor &epoll, const FileDescriptor &client)
{
	Client &target = clients[client];

	if (target.isKeepAlive())
		return (target.reset());
	disconnect(epoll, client, DISCONNECTING_MESSAGE);
}

void Server::destructClients()
{
	for (ClientMap::iterator it = clients.begin(); it != clients.end(); it++)
	{
		it->second.reset();
		close(it->first);
	}
}

void Server::disconnectPipe(const FileDescriptor &epoll, const FileDescriptor &pipe)
{
	int status = 0;
	Client *c = cgiClients[pipe];

	// disconnectPipe()는 receiveCGI 하위에서만 호출된다.
	// 즉, CGI 스크립트를 위한 프로세스가 정상적으로 생성되고 CGI 스크립트 실행 후 CGI 응답 수신 시작까지는 무조건 이뤄진 상태다.
	// Client 객체 내에 CGI 스크립트 프로세스의 pid가 설정되었음을 보장할 수 있다.

	// CGI 프로세스 존재 시 강제 종료
	kill(c->getCGIPID(), SIGKILL);
	while (waitpid(c->getCGIPID(), &status, WNOHANG) == 0)
		;
	if (WIFEXITED(status) && WEXITSTATUS(status) == 1)
		c->setCGIErrorState();
	controlIOEvent(epoll, EPOLL_CTL_DEL, pipe, EPOLLIN);
	cgiClients.erase(pipe);
	close(pipe);
}

template <typename MapType>
inline void Server::closeFileDescriptor(MapType &mapObject, const FileDescriptor &epoll)
{
	for (typename MapType::const_iterator cIt = mapObject.begin(); cIt != mapObject.end(); cIt++)
	{
		controlIOEvent(epoll, EPOLL_CTL_DEL, cIt->first, EPOLLIN);
		close(cIt->first);
	}
}

Server::Server(const Config config) : config(config)
{
}

void Server::run()
{
	FileDescriptor epoll;

	try
	{
		initServerSockets();
		epoll = createEpollObject();
		for (ServerMap::const_iterator cIt = servers.begin(); cIt != servers.end(); cIt++)
			controlIOEvent(epoll, EPOLL_CTL_ADD, cIt->first, EPOLLIN);
		while (true)
			loopIOEvents(epoll);
	}
	catch (const std::runtime_error &ex)
	{
		connection.clear();
		closeFileDescriptor(cgiClients, epoll);
		destructClients();
		closeFileDescriptor(servers, epoll);
		close(epoll);
		throw(ex);
	}
}
