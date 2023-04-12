#include <utility>
#include "Config.hpp"

void Config::addConfigServer(ConfigServer configServer)
{
	SocketAddr		socketAddr(configServer.getHost(), configServer.getPort());
	ArgumentList	serverNameList = configServer.getServerName();

	for (ArgumentList::iterator cIt = serverNameList.begin(); cIt != serverNameList.end(); cIt++)
		serverMap[socketAddr][*cIt] = configServer;
}
