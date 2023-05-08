#ifndef CONFIGSERVER_HPP
# define CONFIGSERVER_HPP

# include "Block.hpp"
# include "ConfigLocation.hpp"

class ConfigServer : public InterBlock
{
public:
	typedef std::string						Route;
	typedef std::map<Route, ConfigLocation>	LocationMap;
	typedef LocationMap::const_iterator		const_iterator;
private:
	LocationMap	locationMap;

public:
	void			addConfigLocation(Route route, ConfigLocation configLocation);
	void			setUndeclaredLocationDirectives(const ConfigServer &server);
	const_iterator	findLocation(Route route);
};

#endif