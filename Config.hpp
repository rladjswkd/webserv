#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Block.hpp"
# include "ConfigServer.hpp"

class Config : public BaseBlock
{
private:
	class ConfigServerContainer
	{
	public:
		typedef const std::string					ServerName;
		typedef std::pair<ServerName, ConfigServer>	ServerSubPair;
		typedef std::map<ServerName, ConfigServer>	ServerSubMap;
	public:
		ConfigServer		*defaultServer;
		ServerSubMap		serverSubMap;
	public:
		void				addServerSubPair(ServerSubPair pair);
		const ConfigServer	&getDefaultServer();
	};

public:
	typedef const std::string							Host, Port;
	typedef std::pair<Host, Port>						SocketAddr;
	typedef std::map<SocketAddr, ConfigServerContainer>	ServerMap;

private:
	ServerMap	serverMap;

public:
	void	addConfigServer(ArgumentList &serverNames, ConfigServer configServer);
};
#endif