#ifndef CONFIGLOCATION_HPP
# define CONFIGLOCATION_HPP

# include "Block.hpp"

class ConfigLocation : public InterBlock
{
private:
	Directive	alias, limitExcept;
	
public:
	void	setAlias(Directive directive);
	void	setLimitExcept(Directive directive);
};
#endif