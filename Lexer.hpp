#ifndef LEXER_H
# define LEXER_H

# include <list>
# include <map>
# include <string>
# include <fstream>
# include "Lexeme.hpp"
# include "Token.hpp"

# define DEFALUT_FILEPATH			"./default"
# define WHITESPACES_LITERAL		" \n\t\r\f\v"
# define BRACKET_SEMICOLON_LITERAL	"{};"

# define FILEPATH_EXCEPT_MSG		"INVALID FILE PATH!"

class Lexer
{
public:
	typedef const std::string				FilePath;
	typedef std::string						Delimiter, Lexeme;
	typedef int								TokenType;
	typedef std::pair<TokenType, Lexeme>	Token;
	typedef std::list<Token>				Tokens;
	typedef std::map<Lexeme, TokenType>		Syntax;
	typedef std::ifstream					ConfigFile;

private:	// constants
	FilePath			FILEPATH;
	Delimiter			WHITESPACES;
	Delimiter			BRACKET_SEMICOLON;

private:
	ConfigFile			configFile;

private:
	void		validateFileOpen();
	bool		isNotDelimiter(char c);
	bool		isWhiteSpaces(char c);
	bool		isBracketOrSemicolon(Delimiter delimiter);
	void		processToken(Tokens &tokens);
	void    	addToken(Tokens &tokens, Lexeme lexeme, Lexeme delimiter);
	TokenType	evaluateLexeme(const Lexeme &lexeme);
	Syntax		initializeSyntax();
	TokenType	evaluateDelimiterLexeme(const Delimiter &delimiter);

public:
	Lexer(const char *filePath);
	Tokens	tokenize();
};

#endif