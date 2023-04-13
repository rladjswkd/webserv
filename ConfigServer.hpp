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
	LocationMap	locationMap;

public:
	void	addConfigLocation(Route route, ConfigLocation configLocation);
};
#endif