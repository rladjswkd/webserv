#include "ConfigServer.hpp"

void ConfigServer::addConfigLocation(Route route, ConfigLocation configLocation)
{
	locationMap.insert(std::make_pair(route, configLocation));
}

void ConfigServer::setUndeclaredLocationDirectives(const ConfigServer &server)
{
	for (LocationMap::iterator it = locationMap.begin(); it != locationMap.end(); it++)
		(it->second).setDirectivesInter(server);
}

ConfigServer::const_iterator ConfigServer::findLocation(Route route) const
{
	return (locationMap.find(route));
}

ConfigServer::const_iterator ConfigServer::end() const
{
	return (locationMap.end());
}
