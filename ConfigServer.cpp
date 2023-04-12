#include "ConfigServer.hpp"

void ConfigServer::setServerName(Directive directive)
{
	serverName = directive;
}

void ConfigServer::setListen(Directive directive)
{
	listen = directive;
}

void ConfigServer::addConfigLocation(Route route, ConfigLocation configLocation)
{
	locationMap.insert(std::make_pair(route, configLocation));
}
