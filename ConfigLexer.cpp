#include "ConfigLexer.hpp"
#include <stdexcept>

// static members initialization
const ConfigLexer::Delimiter	ConfigLexer::WHITESPACES = WHITESPACES_LITERAL;
const ConfigLexer::Delimiter	ConfigLexer::BRACKET_SEMICOLON = BRACKET_SEMICOLON_LITERAL;
ConfigLexer::ConfigFile			ConfigLexer::configFile;

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

void ConfigLexer::processToken(Tokens &tokens)
{
	Lexeme			lexeme;
	std::filebuf	*fileBuf = configFile.rdbuf();
	char			currentChar = fileBuf->sgetc();

	while (isWhiteSpaces(currentChar))
		currentChar = fileBuf->snextc();
	while (currentChar != EOF && isNotDelimiter(currentChar))
	{
		lexeme.push_back(currentChar);
		currentChar = fileBuf->snextc();
	}
	fileBuf->sbumpc();
	if (currentChar == EOF)
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
	syntax[LISTEN] = TOKEN_DIRECTIVE_ONE;
	syntax[SERVER_NAME] = TOKEN_DIRECTIVE_MULT;
	syntax[ERROR_PAGE] = TOKEN_DIRECTIVE_MULT;
	syntax[CLIENT_MAX_BODY_SIZE] = TOKEN_DIRECTIVE_ONE;
	syntax[LIMIT_EXCEPT] = TOKEN_DIRECTIVE_MULT;
	syntax[REDIRECT] = TOKEN_DIRECTIVE_ONE;
	syntax[ALIAS] = TOKEN_DIRECTIVE_ONE;
	syntax[AUTOINDEX] = TOKEN_DIRECTIVE_ONE;
	syntax[INDEX] = TOKEN_DIRECTIVE_MULT;
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
	Tokens	tokens;

	configFile.open(filePath);
	if (!configFile.good())
		throw (std::invalid_argument(FILEPATH_EXCEPT_MSG));
	while (!configFile.eof())
		processToken(tokens);
	configFile.close();
	return (tokens);
}
