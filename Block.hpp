#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <string>
# include <vector>
# include <map>

class BaseBlock
{
public:
	typedef std::string				Argument;
	typedef std::vector<Argument>	Directive;

protected:
	Directive	errorPage, clientMaxBodySize, autoIndex, index;

public:
	void	setErrorPage(Directive directive);
	void	setClientMaxBodySize(Directive directive);
	void	setAutoIndex(Directive directive);
	void	setIndex(Directive directive);
};

class InterBlock : public BaseBlock
{
public:
	typedef BaseBlock::Directive	Directive;

protected:
	Directive	redirect;

public:
	void	setRedirect(Directive directive);
};
#endif