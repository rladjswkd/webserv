#ifndef PARSER_HPP
# define PARSER_HPP

# include <utility>
# include <vector>
# include "ConfigLexer.hpp"
# include "Config.hpp"
# include "ConfigServer.hpp"
# include "ConfigLocation.hpp"
# include "ValidArguements.hpp"

# define FILE_FORMAT_EXCEPT_MSG	"INVALID FILE FORMAT!"
# define DOT					'.'
# define COLON					':'

class ConfigParser
{
public:
	typedef ConfigLexer::Tokens				Tokens;
	typedef ConfigLexer::TokenType			TokenType;
	typedef Tokens::const_iterator			const_iterator;
	typedef std::string						Keyword, Argument, NumericString, Route;
	typedef std::vector<Argument>			Arguments;
	typedef std::pair<Keyword, Arguments>	Directive;
	typedef unsigned long long				Number;

private:
	ConfigParser();
	static Number		convertToNumber(const NumericString &str);
	static void			checkIfNumberBetween(Number number, Number lower, Number upper);
	static void			checkIfNumber(const char &c);

	static void			parseCurrentToken(Config &config, const_iterator &cIt, const_iterator &cItEnd);
	static Arguments	parseDirectiveOne(const_iterator &cIt);
	static Arguments	parseDirectiveMult(const_iterator &cIt);

	static void			parseServer(Config &config, const_iterator &cIt, const_iterator &cItEnd);
	static void			parseServerCurrentToken(ConfigServer &server, const_iterator &cIt, const_iterator &cItEnd);
	static void			parseLocation(ConfigServer &server, const_iterator &cIt, const_iterator &cItEnd);
	static void			parseLocationCurrentToken(ConfigLocation &location, const_iterator &cIt);
	
	static void			parseServerName(ConfigServer &server, const_iterator &cIt);
	static void			parseListen(ConfigServer &server, const_iterator &cIt);
	static void			parseLimitExcept(ConfigLocation &location, const_iterator &cIt);
	static void			checkLimitExcept(const Argument &argument);
	static void			parseAlias(ConfigLocation &location, const_iterator &cIt);
	static void			parseErrorPage(BaseBlock &block, const_iterator &cIt);
	static void			parseClientMaxBodySize(BaseBlock &block, const_iterator &cIt);
	static void			parseRedirect(InterBlock &block, const_iterator &cIt);
	static void			parseAutoIndex(BaseBlock &block, const_iterator &cIt);
	static void			parseIndex(BaseBlock &block, const_iterator &cIt);

public:
	static Config		parse(const Tokens &tokens);

};

#endif
