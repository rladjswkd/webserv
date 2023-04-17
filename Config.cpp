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

void Config::ConfigServerContainer::addServerSubPair(ServerSubPair pair)
{
	serverSubMap.insert(pair);
	if (serverSubMap.size() == 1)
		defaultServer = &(serverSubMap[pair.first]);
}

const ConfigServer &Config::ConfigServerContainer::getDefaultServer()
{
	return (*defaultServer);
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
