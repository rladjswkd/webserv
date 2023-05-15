#include "ConfigServer.hpp"

void ConfigServer::updateResultRoute(const_iterator &result, const_iterator &current, const Route &requested) const
{
	const Route	&locationUri = current->first;

	if (requested.find(locationUri) == 0 && ((result->first).size() < locationUri.size() || result == locationMap.end()))
		result = current;
}

void ConfigServer::addConfigLocation(Route route, ConfigLocation configLocation)
{
	locationMap.insert(std::make_pair(route, configLocation));
}

void ConfigServer::setUndeclaredLocationDirectives(const ConfigServer &server)
{
	for (LocationMap::iterator it = locationMap.begin(); it != locationMap.end(); it++)
		(it->second).setDirectivesInter(server);
}

ConfigServer::const_iterator ConfigServer::findLocation(const Route &requested) const
{
	const_iterator	ret = locationMap.end();

	for (const_iterator cIt = locationMap.begin(); cIt != locationMap.end(); cIt++)
		updateResultRoute(ret, cIt, requested);
	return (ret);
}

ConfigServer::const_iterator ConfigServer::end() const
{
	return (locationMap.end());
}
