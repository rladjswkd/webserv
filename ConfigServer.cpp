#include "ConfigServer.hpp"

const ConfigServer::Argument &ConfigServer::getHost()
{
	return (host);
}

const ConfigServer::Argument &ConfigServer::getPort()
{
	return (port);
}

void ConfigServer::setHostPort(Argument host, Argument port)
{
	this->host = host;
	this->port = port;
}

void ConfigServer::addConfigLocation(Route route, ConfigLocation configLocation)
{
	locationMap.insert(std::make_pair(route, configLocation));
}
