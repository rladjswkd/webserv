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

private:
	void			updateResultRoute(const_iterator &result, const_iterator &current, const Route &requested) const;

public:
	void			addConfigLocation(Route route, ConfigLocation configLocation);
	void			setUndeclaredLocationDirectives(const ConfigServer &server);
	const_iterator	findLocation(const Route &requested) const;
	const_iterator	end() const;
};

#endif