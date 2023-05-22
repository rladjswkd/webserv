#include "ConfigLexer.hpp"
#include <stdexcept>
#include <cstring>

// static members initialization
const ConfigLexer::Delimiter	ConfigLexer::WHITESPACES = WHITESPACES_LITERAL;
const ConfigLexer::Delimiter	ConfigLexer::BRACKET_SEMICOLON = BRACKET_SEMICOLON_LITERAL;

bool ConfigLexer::isNotDelimiter(char c)
{
    return (WHITESPACES.find(c) == std::string::npos && BRACKET_SEMICOLON.find(c) == std::string::npos);
}

bool ConfigLexer::isWhiteSpaces(char c)
{
    return (WHITESPACES.find(c) != std::string::npos);
}

bool ConfigLexer::isBracketOrSemicolon(Delimiter delimiter)
{
	return (BRACKET_SEMICOLON.find(delimiter) != std::string::npos);
}

void ConfigLexer::processToken(Tokens &tokens, ConfigFile &configFile)
{
	Lexeme			lexeme;
	std::filebuf	*fileBuf = configFile.rdbuf();
	char			currentChar = fileBuf->sgetc();

	while (isWhiteSpaces(currentChar))
		currentChar = fileBuf->snextc();
	while (currentChar != (char)EOF && isNotDelimiter(currentChar))
	{
		lexeme.push_back(currentChar);
		currentChar = fileBuf->snextc();
	}
	fileBuf->sbumpc();
	if (currentChar == (char)EOF)
		configFile.setstate(std::ios_base::eofbit);
	addToken(tokens, lexeme, Lexeme(1, currentChar));
}

void ConfigLexer::addToken(Tokens &tokens, Lexeme lexeme, Lexeme delimiter)
{
	if (lexeme.size())		
		tokens.push_back(std::make_pair(evaluateLexeme(lexeme), lexeme));
	if (isBracketOrSemicolon(delimiter))
		tokens.push_back(std::make_pair(evaluateDelimiterLexeme(delimiter), ""));
}

ConfigLexer::TokenType ConfigLexer::evaluateLexeme(const Lexeme &lexeme)
{
	static Syntax		syntax(initializeSyntax());
	Syntax::iterator	pos = syntax.find(lexeme);

	if (pos != syntax.end())
		return (pos->second);
	return (TOKEN_ARGUMENT);
}

ConfigLexer::Syntax	ConfigLexer::initializeSyntax()
{
	Syntax	syntax;

	syntax[SERVER] = TOKEN_SERVER;
	syntax[LOCATION] = TOKEN_LOCATION;
	syntax[LISTEN] = TOKEN_LISTEN;
	syntax[SERVER_NAME] = TOKEN_SERVER_NAME;
	syntax[ERROR_PAGE] = TOKEN_ERROR_PAGE;
	syntax[CLIENT_MAX_BODY_SIZE] = TOKEN_CLIENT_MAX_BODY_SIZE;
	syntax[LIMIT_EXCEPT] = TOKEN_LIMIT_EXCEPT;
	syntax[REDIRECT] = TOKEN_REDIRECT;
	syntax[ALIAS] = TOKEN_ALIAS;
	syntax[AUTOINDEX] = TOKEN_AUTOINDEX;
	syntax[INDEX] = TOKEN_INDEX;
	return (syntax);
}

ConfigLexer::TokenType ConfigLexer::evaluateDelimiterLexeme(const Delimiter &delimiter)
{
	if (delimiter == LEFT_BRACKET)
		return (TOKEN_LBRACKET);
	if (delimiter == RIGHT_BRACKET)
		return (TOKEN_RBRACKET);
	return (TOKEN_SEMICOLON);
}

ConfigLexer::Tokens ConfigLexer::tokenize(const char *filePath)
{
	Tokens		tokens;
	ConfigFile	configFile;

	if (filePath[std::strlen(filePath) - 1] != '/')
		configFile.open(filePath);
	if (!configFile.is_open() || !configFile.good())
		throw (std::invalid_argument(FILEPATH_EXCEPT_MSG));
	while (!configFile.eof())
		processToken(tokens, configFile);
	configFile.close();
	return (tokens);
}
