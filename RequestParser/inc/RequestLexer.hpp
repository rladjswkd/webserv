#ifndef REQUESTLEXER_H
# define REQUESTLEXER_H

#include <string>
#include <vector>
#include <list>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <cctype>
#include "Request.hpp"
#include "RequestLexeme.hpp"

# define WHITESPACES_LITERAL		" \n\t\r\f\v"

class RequestLexer
{
	public:
		typedef std::string::size_type						SizeType;
		typedef std::string												Delimiter, Lexeme, FieldName, FieldValue, StatusCode;
		typedef int																TokenType;
		typedef std::pair<TokenType, Lexeme>			Token;
		typedef std::list<Token>									Tokens;
		typedef std::pair<FieldName, FieldValue>	HeaderField;
		typedef	std::map<std::string, int>				MandatoryHeaderMap;

	private:	// constants
		static const Delimiter	WHITESPACES;
		static const StatusCode	CLIENT_ERROR;
		static const char		*CRLF;
	// private:
		// static std::string	requestMessage;

	private:
		static void				startLineTokenize(Tokens &tokens, std::string &requestMessage);
		static void				headerLineTokenize(Tokens &tokens, std::string &requestMessage);
		static void				bodyLineDoTokenize(Tokens &tokens, const std::string &requestMessage);
		static void				errorHandling(Tokens &tokens, const std::string &code);
		
		static std::string		getLine(std::string &requestMessage);
		static HeaderField		colonSplit(const std::string &str, const char delimiter);
		static TokenType evaluateDelimiterLexeme(const Delimiter &delimiter);
		static void addToken(Tokens &tokens, Lexeme &lexeme, const Lexeme &delimiter);
		static void	mandatoryHeaderProcess(Tokens &tokens, HeaderField &headerField, MandatoryHeaderMap &mandatoryHeaderMap);
		static MandatoryHeaderMap mandatoryHeaderInitial();
		static std::string &ft_toLower(std::string &str);
		
	public:
		// static Tokens httpTokenize(std::string inputRequestMessage);
		static Tokens startLineHeaderLineTokenize(std::string &inputRequestMessage);
		static Tokens bodyLineTokenize(const std::string &inputRequestMessage);
};

#endif
