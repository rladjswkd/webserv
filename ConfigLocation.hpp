#ifndef CONFIGLOCATION_HPP
# define CONFIGLOCATION_HPP

# include "Block.hpp"

class ConfigLocation : public InterBlock
{
private:
	Argument		alias;			// no default value
	ArgumentList	limitExcept;	// no default value
	
public:
	void	setAlias(Argument list);
	void	setLimitExcept(ArgumentList list);
};
#endif