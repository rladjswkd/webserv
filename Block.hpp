#ifndef BLOCK_HPP
# define BLOCK_HPP

# include <string>
# include <vector>
# include <map>

# define CLIENT_MAX_BODY_SIZE_DEFAULT	"1000000"
# define AUTOINDEX_DEFAULT				"off"
# define INDEX_DEFAULT					"index.html"

class BaseBlock
{
public:
	typedef std::string						Argument, ErrorCode, ErrorPage;
	typedef std::vector<Argument>			ArgumentList;
	typedef std::map<ErrorCode, ErrorPage>	ErrorPageMap;

protected:
	Argument		clientMaxBodySize, autoIndex;
	ArgumentList	index;
	ErrorPageMap	errorPage;

public:
	BaseBlock();
	void	setErrorPage(ErrorCode code, ErrorPage page);	// default : default error page for each error code
	void	setClientMaxBodySize(Argument arg); 			// default : 1000000
	void	setAutoIndex(Argument arg);						// default : off
	void	setIndex(ArgumentList list);					// default : index.html
};

class InterBlock : public BaseBlock
{
protected:
	ArgumentList	redirect;

public:
	void	setRedirect(ArgumentList list);		// no default value
};
#endif