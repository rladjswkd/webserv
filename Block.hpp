#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <string>
# include <vector>
# include <map>

class BaseBlock
{
public:
	typedef std::string				Argument;
	typedef std::vector<Argument>	ArgumentList;

protected:
	Argument		clientMaxBodySize, autoIndex;
	ArgumentList	errorPage, index;

public:
	void	setErrorPage(ArgumentList list);
	void	setClientMaxBodySize(Argument arg);
	void	setAutoIndex(Argument arg);
	void	setIndex(ArgumentList list);
};

class InterBlock : public BaseBlock
{
protected:
	ArgumentList	redirect;

public:
	void	setRedirect(ArgumentList list);
};
#endif