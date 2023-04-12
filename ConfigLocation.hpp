#ifndef CONFIGLOCATION_HPP
# define CONFIGLOCATION_HPP

# include "Block.hpp"

class ConfigLocation : public InterBlock
{
private:
	Argument		alias;
	ArgumentList	limitExcept;
	
public:
	void	setAlias(Argument list);
	void	setLimitExcept(ArgumentList list);
};
#endif