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
		typedef std::string												Delimiter, Lexeme, FieldName, FieldValue;
		typedef int																TokenType;
		typedef std::pair<TokenType, Lexeme>			Token;
		typedef std::list<Token>									Tokens;
		typedef std::pair<FieldName, FieldValue>	HeaderField;
		typedef	std::map<std::string, int>				MandatoryHeaderMap;

	private:	// constants
		static const Delimiter	WHITESPACES;

	private:
		static std::string	requestMessage;

	private:
		static void				startLineTokenize(Tokens &tokens);
		static void				headerLineTokenize(Tokens &tokens);
		static void				bodyLineTokenize(Tokens &tokens);
		static void				errorHandling(Tokens &tokens, const char *code);
		
		static std::string		getLine();
		static HeaderField		colonSplit(std::string str, char delimiter);
		static TokenType evaluateDelimiterLexeme(const Delimiter &delimiter);
		static void addToken(Tokens &tokens, Lexeme lexeme, Lexeme delimiter);
		static bool isDelimiter(char c);
		static bool isCRLF();
		static void	mandatoryHeaderProcess(Tokens &tokens, HeaderField &headerField, MandatoryHeaderMap mandatoryHeaderMap);
		static int	isMandatoryHeader(MandatoryHeaderMap mandatoryHeader, std::string str);
		static MandatoryHeaderMap mandatoryHeaderInitial();
		static std::string ft_toLower(std::string str);
		
	public:
		static Tokens httpTokenize(std::string inputRequestMessage);
		static Tokens startLineHeaderLineTokenize(const std::string &inputRequestMessage);
		static Tokens bodyLineTokenize(const std::string &inputRequestMessage);
};

#endif
