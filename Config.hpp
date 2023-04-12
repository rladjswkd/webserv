#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "Block.hpp"
# include "ConfigServer.hpp"

class Config : public BaseBlock
{
public:
	typedef std::string							Host, Port, ServerName;
	typedef std::pair<Host, Port>				SocketAddr;
	typedef std::map<ServerName, ConfigServer>	ServerSubMap;
	typedef std::map<SocketAddr, ServerSubMap>	ServerMap;

private:
	ServerMap	serverList;

public:
	void	addConfigServer(ConfigServer configServer);
};
#endif