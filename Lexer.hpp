#ifndef LEXER_H
# define LEXER_H

# include <list>
# include <map>
# include <string>
# include <fstream>
# include "Lexeme.hpp"
# include "Token.hpp"

# define WHITESPACES_LITERAL		" \n\t\r\f\v"
# define BRACKET_SEMICOLON_LITERAL	"{};"

# define FILEPATH_EXCEPT_MSG		"INVALID FILE PATH!"

class Lexer
{
public:
	typedef std::string						Delimiter, Lexeme;
	typedef int								TokenType;
	typedef std::pair<TokenType, Lexeme>	Token;
	typedef std::list<Token>				Tokens;
	typedef std::map<Lexeme, TokenType>		Syntax;
	typedef std::ifstream					ConfigFile;

private:	// constants
	static const Delimiter	WHITESPACES;
	static const Delimiter	BRACKET_SEMICOLON;

private:
	static ConfigFile	configFile;

private:
	Lexer();

	static bool			isNotDelimiter(char c);
	static bool			isWhiteSpaces(char c);
	static bool			isBracketOrSemicolon(Delimiter delimiter);
	static void			processToken(Tokens &tokens);
	static void    		addToken(Tokens &tokens, Lexeme lexeme, Lexeme delimiter);
	static TokenType	evaluateLexeme(const Lexeme &lexeme);
	static Syntax		initializeSyntax();
	static TokenType	evaluateDelimiterLexeme(const Delimiter &delimiter);

public:
	static Tokens		tokenize(const char *filePath);
};

#endif