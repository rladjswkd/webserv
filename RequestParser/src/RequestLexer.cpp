#include "RequestLexer.hpp"
#include <algorithm>

const char 						*RequestLexer::CRLF = "\r\n";
const RequestLexer::Delimiter	RequestLexer::WHITESPACES = WHITESPACES_LITERAL;
const RequestLexer::StatusCode	RequestLexer::CLIENT_ERROR = "400";

std::string &RequestLexer::ft_toLower(std::string &str)
{
	std::for_each(str.begin(), str.end(), ::tolower);
	return (str);
}

RequestLexer::TokenType RequestLexer::evaluateDelimiterLexeme(const Delimiter &delimiter)
{
	if (delimiter == " ")
		return (SP);
	if (delimiter == ":")
		return (COLON);
	return (ETC);
}

void RequestLexer::addToken(Tokens &tokens, Lexeme &lexeme, const Lexeme &delimiter)
{
	if (lexeme.size())		
		tokens.push_back(std::make_pair(STARTLINE_STR, lexeme));
	if (delimiter != "")
		tokens.push_back(std::make_pair(evaluateDelimiterLexeme(delimiter), ""));
}

RequestLexer::HeaderField	RequestLexer::colonSplit(const std::string &str, const char delimiter)
{	
	SizeType pos = str.find(delimiter);	

	if (pos == std::string::npos)
		throw (CLIENT_ERROR);
	return (std::make_pair(str.substr(0, pos), str.substr(pos + 1, str.length())));
}

std::string	RequestLexer::getLine(std::string &requestMessage)
{	
	SizeType pos = requestMessage.find(CRLF);
	std::string	line;
	
	if (pos == std::string::npos)
		throw (CLIENT_ERROR);
	line = requestMessage.substr(0, pos);
	requestMessage.erase(0, pos + 2);
	return (line);
}

void	RequestLexer::startLineTokenize(Tokens &tokens, std::string &requestMessage)
{	
	std::istringstream		ss(getLine(requestMessage)); 
	char					currentChar = ss.get();
	Lexeme	lexeme;		

	while (!ss.eof() || currentChar != EOF)
	{
		if (WHITESPACES.find(currentChar) == std::string::npos)
		{
			lexeme.push_back(currentChar);
			currentChar = ss.get();
		}
		else
		{
			addToken(tokens, lexeme, Lexeme(1, currentChar));
			currentChar = ss.get();
			lexeme = "";
		}
	}
	addToken(tokens, lexeme, "");
}

RequestLexer::MandatoryHeaderMap RequestLexer::mandatoryHeaderInitial()
{
	MandatoryHeaderMap mandatoryHeader;

	mandatoryHeader["host"] = HOST;
	mandatoryHeader["host_value"] = HOST_VALUE;
	mandatoryHeader["transfer-encoding"] = TRANSFER_ENCODING;
	mandatoryHeader["transfer-encoding_value"] = TRANSFER_ENCODING_VALUE;
	mandatoryHeader["content-length"] = CONTENT_LENGTH;
	mandatoryHeader["content-length_value"] = CONTENT_LENGTH_VALUE;
	mandatoryHeader["cookie"] = COOKIE;
	mandatoryHeader["cookie_value"] = COOKIE_VALUE;
	mandatoryHeader["content-type"] = CONTENT_TYPE;
	mandatoryHeader["multipart/form-data"] = MULTIPART_FORM_DATA;
	mandatoryHeader["connection"] = CONNECTION;
	mandatoryHeader["connection_value"] = CONNECTION_VALUE;


	return mandatoryHeader;
}

void	RequestLexer::mandatoryHeaderProcess(Tokens &tokens, HeaderField &headerField, MandatoryHeaderMap &mandatoryHeaderMap)
{

	std::string headerFieldLower = ft_toLower(headerField.first); //headerfield는 대소문자 구별이 없다. 소문자 기준으로 프로그램을 작성하였다.
	
	tokens.push_back(std::make_pair(mandatoryHeaderMap[headerFieldLower], headerFieldLower));
	if (headerFieldLower == "content-type" 
		&& ft_toLower(headerField.second).find("multipart/form-data") != std::string::npos)
		tokens.push_back(std::make_pair(mandatoryHeaderMap["multipart/form-data"], headerField.second));
	else
		tokens.push_back(std::make_pair(mandatoryHeaderMap[headerFieldLower + "_value"], headerField.second));
}

void	RequestLexer::headerLineTokenize(Tokens &tokens, std::string &requestMessage)
{	
	std::string headerLine;
	HeaderField	headerField;
	Lexeme	lexeme;
	MandatoryHeaderMap mandatoryHeader = mandatoryHeaderInitial();

	while (requestMessage.find(CRLF) != 0)
	{
		headerLine = getLine(requestMessage);
		headerField = colonSplit(headerLine, ':');
		if (mandatoryHeader.find(ft_toLower(headerField.first)) != mandatoryHeader.end())
			mandatoryHeaderProcess(tokens, headerField, mandatoryHeader);
		else
		{
			tokens.push_back(std::make_pair(FIELDNAME, ft_toLower(headerField.first)));
			tokens.push_back(std::make_pair(FIELDVALUE, headerField.second));
		}
	}
}

void	RequestLexer::bodyLineDoTokenize(Tokens &tokens, const std::string &requestMessage)
{
	std::string body;
	SizeType len;

	len = requestMessage.length();
	if (requestMessage.find(CRLF) == 0)
		body = requestMessage.substr(2, len);
	else
		body = requestMessage;
	tokens.push_back(std::make_pair(BODY, body));
}

void	RequestLexer::errorHandling(Tokens &tokens, const std::string &code)
{
	tokens.empty();
	tokens.push_back(std::make_pair(ERROR, code)); //code가 char형인데 되는지 모르겠음. 여기는 string
}

RequestLexer::Tokens RequestLexer::startLineHeaderLineTokenize(std::string &requestMessage)
{
	Tokens	tokens;

	try
	{
		RequestLexer::startLineTokenize(tokens,  requestMessage);
		RequestLexer::headerLineTokenize(tokens, requestMessage);
	}
	catch(const std::string &code)
	{
		errorHandling(tokens, code);
	}
	return (tokens);
}

RequestLexer::Tokens RequestLexer::bodyLineTokenize(const std::string &requestMessage)
{
	Tokens	tokens;

	try
	{
		RequestLexer::bodyLineDoTokenize(tokens, requestMessage);
	}
	catch(const std::string &code)
	{
		errorHandling(tokens, code);
	}
	return (tokens);
}
