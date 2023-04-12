#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include "Block.hpp"
# include "ConfigLocation.hpp"

class ConfigServer : public InterBlock
{
public:
	typedef std::string						Route;
	typedef std::map<Route, ConfigLocation>	LocationMap;
	
private:
	ArgumentList	serverName;
	Argument		host, port;
	LocationMap		locationMap;

public:
	void				setServerName(ArgumentList list);
	const ArgumentList	getServerName();
	void				setHostPort(Argument host, Argument port);
	const Argument		getHost();
	const Argument		getPort();
	void				addConfigLocation(Route route, ConfigLocation configLocation);
};
#endif