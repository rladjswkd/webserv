#include "ConfigLexer.hpp"
#include "ConfigParser.hpp"
#include "Config.hpp"

int	main()
{
	Config	config = ConfigParser::parse(ConfigLexer::tokenize("./exampleInherit"));
	return (0);
}