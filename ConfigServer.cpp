#include "ConfigServer.hpp"

void ConfigServer::setServerName(ArgumentList list)
{
	serverName = list;
}

const ConfigServer::ArgumentList &ConfigServer::getServerName()
{	
	return (serverName);
}

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
