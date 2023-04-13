#include <utility>
#include "Config.hpp"

void Config::addConfigServer(ConfigServer configServer)
{
	ConfigServerContainer	&serverContainer = serverMap[std::make_pair(configServer.getHost(), configServer.getPort())];
	const ArgumentList		&serverNameList = configServer.getServerName();

	if (serverNameList.empty())
		configServer.setServerName(ArgumentList(1, ""));
	for (ArgumentList::const_iterator cIt = serverNameList.begin(); cIt != serverNameList.end(); cIt++)
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
