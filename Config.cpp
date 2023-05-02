#include <utility>
#include "Config.hpp"

void Config::addConfigServer(ArgumentList &serverNames, SocketAddr addr, ConfigServer configServer)
{
	ConfigServerContainer	&serverContainer = serverMap[addr];

	for (ArgumentList::const_iterator cIt = serverNames.begin(); cIt != serverNames.end(); cIt++)
		serverContainer.addServerSubPair(std::make_pair(*cIt, configServer));
}

void Config::setUndeclaredDirectives()
{
	//	every servers
	for (ServerMap::iterator it = serverMap.begin(); it != serverMap.end(); it++)
		(it->second).setUndeclaredServerDirectives(*this);
}

Config::const_iterator Config::begin() const
{
    return (serverMap.begin());
}

Config::const_iterator Config::end() const
{
    return (serverMap.end());
}

const ConfigServer &Config::getServer(SocketAddr socketAddr, ConfigServerContainer::ServerName serverName)
{
	ConfigServerContainer			&container = serverMap[socketAddr];
	ServerSubMap					&mapper = container.serverSubMap;
	ServerSubMap::const_iterator	cIt = mapper.find(serverName);

	if (cIt == mapper.end())
		return (*(container.defaultServer));
	return (cIt->second);		
}

void Config::ConfigServerContainer::addServerSubPair(ServerSubPair pair)
{
	serverSubMap.insert(pair);
	if (serverSubMap.size() == 1)
		defaultServer = &(serverSubMap[pair.first]);
}

void Config::ConfigServerContainer::setUndeclaredServerDirectives(const Config &config)
{
	//	servers on same host:port 
	for (ServerSubMap::iterator it = serverSubMap.begin(); it != serverSubMap.end(); it++)
	{
		ConfigServer	&server = it->second;

		server.setDirectivesBase(config);
		server.setUndeclaredLocationDirectives(server);
	}
		
}
