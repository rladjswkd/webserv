#ifndef SERVER_HPP
# define SERVER_HPP

# include <set>
# include <utility>
# include <string>
# include <netdb.h>
# include "Config.hpp"
# include "Client.hpp"

# define LOCALHOST_STR					"localhost"
# define LOCALHOST_NUMERIC_STR			"127.0.0.1"
# define ASTERISK_STR					"*"
# define ASTERISK_NUMERIC_STR			NULL

class Server
{
public:
	typedef int                 		FileDescriptor;
	typedef std::set<FileDescriptor>	FileDescriptorSet;
	typedef std::string					Host, ErrorMessage;
	typedef Config::SocketAddr			SocketAddr;

private:
	const Config		config;
	FileDescriptorSet	socketSet;

private:
	Server();
	void			generateServerSocket(SocketAddr socketAddr);
	FileDescriptor	createSocket();
	addrinfo		createaddrHints();
	void			throwException(addrinfo *info);
	const char		*extractNumericHost(const Host &host);
	void			initServerSockets();
public:
	Server(const Config config);
	void    run();
};
#endif