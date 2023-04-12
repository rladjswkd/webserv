#include "ConfigParser.hpp"
#include <sstream>
#include <string>
#include <limits>

// parsing 과정은 uri가 올바른지 확인하진 않는다.
// TODO: 기본 값을 활용할지, 필수 입력 지시자들을 정할지 결정한 후 필수 입력 지시자들을 정한 경우, 각 블록에 필수 지시자들이 모두 포함되었는지 확인하고 그렇지 않다면 예외 던지기

Config ConfigParser::parse(const Tokens &tokens)
{
	Config			config;
	const_iterator	cItEnd = tokens.end();

	for (const_iterator cIt = tokens.begin(); cIt != cItEnd; cIt++)
		parseCurrentToken(config, cIt, cItEnd);
	return (config);
}

ConfigParser::Number ConfigParser::convertToNumber(const NumericString &str)
{
	static std::stringstream	ss;
	Number						res;

	for (NumericString::const_iterator cIt = str.begin(); cIt != str.end(); cIt++)
		checkIfNumber(*cIt);
	ss << str;
	ss >> res;
	ss.str("");
	ss.clear();
	return (res);
}

void ConfigParser::checkIfNumberBetween(Number number, Number lower, Number upper)
{
	if (number < lower || upper < number)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
}

void ConfigParser::checkIfNumber(const char &c)
{
	checkIfNumberBetween(c, NUMBER_ZERO, NUMBER_NINE);
}

void ConfigParser::parseCurrentToken(Config &config, const_iterator &cIt, const_iterator &cItEnd)
{
	switch (cIt->first)
	{
		case TOKEN_SERVER:
			return (parseServer(config, ++cIt, cItEnd));
		case TOKEN_ERROR_PAGE:
			return (parseErrorPage(config, ++cIt));
		case TOKEN_CLIENT_MAX_BODY_SIZE:
			return(parseClientMaxBodySize(config, ++cIt));
		case TOKEN_AUTOINDEX:
			return (parseAutoIndex(config, ++cIt));
		case TOKEN_INDEX:
			return (parseIndex(config, ++cIt));
	}
	throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
}

ConfigParser::Arguments ConfigParser::parseDirectiveOne(const_iterator &cIt)
{
	Arguments	args;

	if (cIt->first & TOKEN_ARGUMENT)
		args.push_back((cIt++)->second);
	if (args.size() != 1 || (cIt++)->first != TOKEN_SEMICOLON)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	return (args);
}

ConfigParser::Arguments ConfigParser::parseDirectiveMult(const_iterator &cIt)
{
	Arguments	args;

	while (cIt->first & TOKEN_ARGUMENT)
		args.push_back((cIt++)->second);
	if (args.empty() || (cIt++)->first != TOKEN_SEMICOLON)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	return (args);
}

void ConfigParser::parseServer(Config &config, const_iterator &cIt, const_iterator &cItEnd)
{
	ConfigServer	server;

	if (cIt->first != TOKEN_LBRACKET)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	for (; cIt->first != TOKEN_RBRACKET && cIt != cItEnd; cIt++)
		parseServerCurrentToken(server, cIt, cItEnd);
	if (cIt == cItEnd)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	cIt++;
	config.addConfigServer(server);
}

void ConfigParser::parseServerCurrentToken(ConfigServer &server, const_iterator &cIt, const_iterator &cItEnd)
{
	switch (cIt->first)
	{
		case TOKEN_LOCATION:
			return (parseLocation(server, cIt, cItEnd));
		case TOKEN_SERVER_NAME:
			return (parseServerName(server, cIt));
		case TOKEN_LISTEN:
			return (parseListen(server, cIt));
		case TOKEN_REDIRECT:
			return (parseRedirect(server, cIt));
		case TOKEN_ERROR_PAGE:
			return (parseErrorPage(server, cIt));
		case TOKEN_CLIENT_MAX_BODY_SIZE:
			return (parseClientMaxBodySize(server, cIt));
		case TOKEN_AUTOINDEX:
			return (parseAutoIndex(server, cIt));
		case TOKEN_INDEX:
			return (parseIndex(server, cIt));
	}
	throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
}

void ConfigParser::parseLocation(ConfigServer &server, const_iterator &cIt, const_iterator &cItEnd)
{
	ConfigLocation	location;
	Route			route;
	
	route = parseDirectiveOne(cIt)[0];
	if ((cIt++)->first != TOKEN_LBRACKET)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	for (; cIt->first != TOKEN_RBRACKET && cIt != cItEnd; cIt++)
		parseLocationCurrentToken(location, cIt);
	if (cIt == cItEnd)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	cIt++;
	server.addConfigLocation(route, location);
}

void ConfigParser::parseLocationCurrentToken(ConfigLocation &location, const_iterator &cIt)
{
	switch (cIt->first)
	{
		case TOKEN_ERROR_PAGE:
			return (parseErrorPage(location, cIt));
		case TOKEN_CLIENT_MAX_BODY_SIZE:
			return (parseClientMaxBodySize(location, cIt));
		case TOKEN_LIMIT_EXCEPT:
			return (parseLimitExcept(location, cIt));
		case TOKEN_REDIRECT:
			return (parseRedirect(location, cIt));
		case TOKEN_AUTOINDEX:
			return (parseAutoIndex(location, cIt));
		case TOKEN_INDEX:
			return (parseIndex(location, cIt));
		case TOKEN_ALIAS:
			return (parseAlias(location, cIt));
	}
	throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
}

void ConfigParser::parseServerName(ConfigServer &server, const_iterator &cIt)
{
	server.setServerName(parseDirectiveMult(cIt));
}

// 1024 ~ 49151 are reserved for user server application.
// https://www.ibm.com/docs/en/ztpf/2020?topic=overview-port-numbers
void ConfigParser::parseListen(ConfigServer &server, const_iterator &cIt)
{
	Arguments			args = parseDirectiveOne(cIt);
	std::stringstream	ss(args[0]);
	std::string			buf;
	int					count = 0;

	std::getline(ss, buf, COLON);
	for (; std::getline(ss, buf, DOT); count++)
		checkIfNumberBetween(convertToNumber(buf), OCTET_LOWER, OCTET_UPPER);
	if (count != 4)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	std::getline(ss, buf, COLON);
	checkIfNumberBetween(convertToNumber(buf), PORT_LOWER, PORT_UPPER);
	server.setListen(args);
}

// nginx 기준 동일한 메소드가 여러 번 들어오는 걸 허용한다.
// 사용할 때 std::set으로 만들어서 사용하자.
void ConfigParser::parseLimitExcept(ConfigLocation &location, const_iterator &cIt)
{
	Arguments	args = parseDirectiveMult(cIt);
	
	for (Arguments::const_iterator cIt = args.begin(); cIt != args.end(); cIt++)
		checkLimitExcept(*cIt);
	location.setLimitExcept(args);
}

void ConfigParser::checkLimitExcept(const Argument &argument)
{
	if (argument != METHOD_GET && argument != METHOD_POST && argument != METHOD_DELETE)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
}

void ConfigParser::parseAlias(ConfigLocation &location, const_iterator &cIt)
{
	location.setAlias(parseDirectiveOne(cIt));
}

void ConfigParser::parseErrorPage(BaseBlock &block, const_iterator &cIt)
{
	Arguments					args = parseDirectiveMult(cIt);
	Arguments::const_iterator	pageIt = --(args.end());

	if (args.size() < 2)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	for (Arguments::const_iterator cIt = args.begin(); cIt != pageIt; cIt++)
		checkIfNumberBetween(convertToNumber(*cIt), STATUS_CODE_LOWER, STATUS_CODE_UPPER);
	block.setErrorPage(args);
}

// std::stringstream으로 unsigned long long 타입에 값을 넣으면, 해당 타입의 범위를 넘은 값이 들어갈 경우 타입의 최대값을 넣는다.
void ConfigParser::parseClientMaxBodySize(BaseBlock &block, const_iterator &cIt)
{
	Arguments	args = parseDirectiveOne(cIt);

	if (convertToNumber(args[0]) > std::numeric_limits<unsigned int>::max())
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	block.setClientMaxBodySize(args);
}

void ConfigParser::parseRedirect(InterBlock &block, const_iterator &cIt)
{
	Arguments	args = parseDirectiveOne(cIt);

	if (args.size() < 2)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	checkIfNumberBetween(convertToNumber(args[0]), STATUS_CODE_LOWER, STATUS_CODE_UPPER);
	if (args[1].find(HTTP) != 0 && args[1].find(HTTP_SECURE) != 0)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	block.setRedirect(args);
}

void ConfigParser::parseAutoIndex(BaseBlock &block, const_iterator &cIt)
{
	Arguments	args = parseDirectiveOne(cIt);
	Argument	arg = args[0];

	if (arg != AUTOINDEX_ON && arg != AUTOINDEX_OFF)
		throw (std::invalid_argument(FILE_FORMAT_EXCEPT_MSG));
	block.setAutoIndex(args);
}

void ConfigParser::parseIndex(BaseBlock &block, const_iterator &cIt)
{
	block.setIndex(parseDirectiveMult(cIt));
}
