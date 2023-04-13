#include <utility>
#include "Config.hpp"

void Config::addConfigServer(ArgumentList &serverNames, ConfigServer configServer)
{
	ConfigServerContainer	&serverContainer = serverMap[std::make_pair(configServer.getHost(), configServer.getPort())];

	if (serverNames.empty())
		serverNames.push_back("");
	for (ArgumentList::const_iterator cIt = serverNames.begin(); cIt != serverNames.end(); cIt++)
		serverContainer.addServerSubPair(std::make_pair(*cIt, configServer));
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
