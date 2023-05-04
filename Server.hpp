#ifndef SERVER_HPP
# define SERVER_HPP

# include <set>
# include <map>
# include <utility>
# include <string>
# include <netdb.h>
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
# define DISCONNECTION_MESSAGE			"CLIENT DISCONNECTED!"
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
	void			controlIOEvent(FileDescriptor &epoll, int option, const FileDescriptor &target, uint32_t eventType);
	void			loopIOEvents(FileDescriptor &epoll);
	int				waitIOEventOccurrence(FileDescriptor &epoll, epoll_event *events);
	void			handleIOEvent(FileDescriptor &epoll, const epoll_event &event);
	void			acceptNewClient(FileDescriptor &epoll, const FileDescriptor &server);
	void			disconnectClient(FileDescriptor &epoll, FileDescriptor &client, const char *reason);
	void			receiveRequest(FileDescriptor &epoll, FileDescriptor &fd, Client &target);
	void			receiveCGI(FileDescriptor &epoll, FileDescriptor &fd, Client &target);
	void			processRequest(FileDescriptor &epoll, FileDescriptor &fd, Client &target);
	void			sendData(FileDescriptor &epoll, FileDescriptor &client);
	void			receiveData(FileDescriptor &epoll, FileDescriptor &fd, Client &target);
	void			waitChildProcessNonblocking();

public:
	Server(const Config config);
	void    run();
};
#endif