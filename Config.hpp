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
		void				setUndeclaredServerDirectives(const Config &config);
	};

public:
	typedef const std::string							Host, Port;
	typedef std::pair<Host, Port>						SocketAddr;
	typedef std::map<SocketAddr, ConfigServerContainer>	ServerMap;
	typedef ConfigServerContainer::ServerSubMap			ServerSubMap;
	typedef ServerMap::const_iterator					const_iterator;
private:
	ServerMap	serverMap;

public:
	void				addConfigServer(ArgumentList &serverNames, SocketAddr addr, ConfigServer configServer);
	void				setUndeclaredDirectives();
	const_iterator		begin() const;
	const_iterator		end() const;

	const ConfigServer	&getServer(SocketAddr socketAddr, ConfigServerContainer::ServerName serverName);
};
#endif