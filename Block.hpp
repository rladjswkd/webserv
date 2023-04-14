#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <string>
# include <vector>
# include <map>

# define CLIENT_MAX_BODY_SIZE_DEFAULT	"10000"
# define AUTOINDEX_DEFAULT				"off"
# define INDEX_DEFAULT					"index.html"

class BaseBlock
{
public:
	typedef std::string				Argument;
	typedef std::vector<Argument>	ArgumentList;

protected:
	Argument		clientMaxBodySize, autoIndex;
	ArgumentList	errorPage, index;

public:
	void	setErrorPage(ArgumentList list);	// no default value
	void	setClientMaxBodySize(Argument arg); // default : 1000000
	void	setAutoIndex(Argument arg);			// default : off
	void	setIndex(ArgumentList list);		// default : index.html
};

class InterBlock : public BaseBlock
{
protected:
	ArgumentList	redirect;

public:
	void	setRedirect(ArgumentList list);		// no default value
};
#endif