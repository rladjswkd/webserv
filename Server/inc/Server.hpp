#ifndef SERVER_HPP
# define SERVER_HPP

# include <set>
# include <map>
# include <utility>
# include <string>
# include <netdb.h>
# include <sys/epoll.h>
# include "Config.hpp"
# include "Client.hpp"

# define LOCALHOST_STR					"localhost"
# define LOCALHOST_NUMERIC_STR			"127.0.0.1"
# define ASTERISK_STR					"*"
# define ASTERISK_NUMERIC_STR			NULL
# define MAX_EVENT_COUNT				10
# define BUFFER_SIZE					8000
# define CHAR_NULL						'\0'
# define RECV_EXCEPTION_MESSAGE			"recv() ERROR!"
# define SEND_EXCEPTION_MESSAGE			"send() ERROR!"
# define DISCONNECTED_MESSAGE			"CLIENT DISCONNECTED!"
# define DISCONNECTING_MESSAGE			"DISCONNECTED CONNECTION!"
# define ACCEPT_EXCEPTION_MESSAGE		"accpet() ERROR!"
# define EPOLL_WAIT_EXCEPTION_MESSAGE	"epoll_wait() ERROR!"

class Server
{
public:
	typedef Config::SocketAddr						SocketAddr;
	typedef int                 					FileDescriptor;
	typedef std::string								Host, ErrorMessage;
	typedef std::map<FileDescriptor, SocketAddr>	ServerMap;
	typedef std::map<FileDescriptor, Client>		ClientMap;
	typedef std::map<FileDescriptor, SocketAddr*>	ConnectionMap;	// client file descriptor - connected server address info
	typedef std::map<FileDescriptor, Client*>		CGIClientMap;	// FileDescriptor is pipe file descriptor

private:
	const Config	config;
	ServerMap		servers;
	ClientMap		clients;
	CGIClientMap	cgiClients;
	ConnectionMap	connection;

private:
	Server();
	void			generateServerSocket(SocketAddr socketAddr);
	FileDescriptor	createSocket();
	addrinfo		createaddrHints();
	void			throwException(addrinfo *info);
	const char		*extractNumericHost(const Host &host);
	void			initServerSockets();
	FileDescriptor	createEpollObject();
	void			controlIOEvent(const FileDescriptor &epoll, const int &option, const FileDescriptor &target, const uint32_t &eventType);
	void			loopIOEvents(const FileDescriptor &epoll);
	int				waitIOEventOccurrence(const FileDescriptor &epoll, epoll_event *events);
	void			handleIOEvent(const FileDescriptor &epoll, const epoll_event &event);
	void			acceptNewClient(const FileDescriptor &epoll, const FileDescriptor &server);
	void			disconnectClient(const FileDescriptor &epoll, const FileDescriptor &client, const char *reason);
	void			receiveRequest(const FileDescriptor &epoll, const FileDescriptor &fd, Client &target);
	void			receiveCGI(const FileDescriptor &epoll, const FileDescriptor &fd, Client &target);
	void			processRequest(const FileDescriptor &epoll, const FileDescriptor &fd, Client &target);
	void			sendData(const FileDescriptor &epoll, const FileDescriptor &client);
	void			receiveData(const FileDescriptor &epoll, const FileDescriptor &fd, Client &target);
	void			waitChildProcessNonblocking();
	void			handleConnection(const FileDescriptor &epoll, const FileDescriptor &client);

public:
	Server(const Config config);
	void    run();
};
#endif