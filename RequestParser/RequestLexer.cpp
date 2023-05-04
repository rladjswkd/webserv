#include "RequestLexer.hpp"

const char *g_CRLF = "\r\n";
std::string	RequestLexer::requestMessage;
const RequestLexer::Delimiter	RequestLexer::WHITESPACES = WHITESPACES_LITERAL;

std::string RequestLexer::ft_toLower(std::string str)
{
  for (unsigned int i = 0; i < str.length(); i++) {
    str[i] = std::tolower(str[i]);
  }
	return str;
}

bool RequestLexer::isDelimiter(char c)
{
    return (WHITESPACES.find(c) != std::string::npos);
}

bool RequestLexer::isCRLF()
{
	SizeType pos = requestMessage.find(g_CRLF);	
	
	if (pos == 0)
		return true;
	return false;
}

RequestLexer::TokenType RequestLexer::evaluateDelimiterLexeme(const Delimiter &delimiter)
{
	if (delimiter == " ")
		return (SP);
	if (delimiter == ":")
		return (COLON);
	return (ETC);
}

void RequestLexer::addToken(Tokens &tokens, Lexeme lexeme, Lexeme delimiter)
{
	if (lexeme.size())		
		tokens.push_back(std::make_pair(STARTLINE_STR, lexeme));
	if (delimiter != "")
		tokens.push_back(std::make_pair(evaluateDelimiterLexeme(delimiter), ""));
}

RequestLexer::HeaderField	RequestLexer::colonSplit(std::string str, char delimiter)
{	
	SizeType pos = str.find(delimiter);	
	SizeType len = str.length();

	if (pos == std::string::npos)
		throw ("400");
	
	return std::make_pair(str.substr(0, pos), str.substr(pos + 1, len));
}

std::string	RequestLexer::getLine()
{	
	SizeType pos = requestMessage.find(g_CRLF);
	std::string	line;
	
	if (pos == std::string::npos)
		throw ("400");
	line = requestMessage.substr(0, pos);
	requestMessage = requestMessage.substr(pos + 2, requestMessage.length());
	return line;
}

void	RequestLexer::startLineTokenize(Tokens &tokens)
{	
	std::string 			startLine = getLine();	
	std::istringstream		ss(startLine); 
	char					currentChar = ss.get();
	Lexeme	lexeme;		

	while (!ss.eof() || currentChar != EOF)
	{
		if (!isDelimiter(currentChar))
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

	return mandatoryHeader;
}

int	RequestLexer::isMandatoryHeader(MandatoryHeaderMap mandatoryHeader, std::string str)
{
	return mandatoryHeader.count(str);
}

void	RequestLexer::mandatoryHeaderProcess(Tokens &tokens, HeaderField &headerField, MandatoryHeaderMap mandatoryHeaderMap)
{

	std::string headerFieldLower = ft_toLower(headerField.first); //headerfield는 대소문자 구별이 없다. 소문자 기준으로 프로그램을 작성하였다.
	
	tokens.push_back(std::make_pair(mandatoryHeaderMap[headerFieldLower], headerFieldLower));
	if (headerFieldLower == "content-type" 
		&& ft_toLower(headerField.second).find("multipart/form-data") != std::string::npos)
		tokens.push_back(std::make_pair(mandatoryHeaderMap["multipart/form-data"], headerField.second));
	else
		tokens.push_back(std::make_pair(mandatoryHeaderMap[headerFieldLower + "_value"], headerField.second));
}

void	RequestLexer::headerLineTokenize(Tokens &tokens)
{	
	std::string headerLine;
	HeaderField	headerfield;
	Lexeme	lexeme;
	MandatoryHeaderMap mandatoryHeader = mandatoryHeaderInitial();

	while (!isCRLF())
	{
		headerLine = getLine();
		headerfield = colonSplit(headerLine, ':');
		if (isMandatoryHeader(mandatoryHeader, ft_toLower(headerfield.first)))
			mandatoryHeaderProcess(tokens, headerfield, mandatoryHeader);
		else
		{
			tokens.push_back(std::make_pair(FIELDNAME, ft_toLower(headerfield.first)));
			tokens.push_back(std::make_pair(FIELDVALUE, headerfield.second));
		}
	}
}

void	RequestLexer::bodyLineTokenize(Tokens &tokens)
{
	std::string body;
	SizeType len;

	len = requestMessage.length();
	if (requestMessage.find(g_CRLF) == 0)
		body = requestMessage.substr(2, len);
	else
		body = requestMessage;
	tokens.push_back(std::make_pair(BODY, body));
}

void	RequestLexer::errorHandling(Tokens &tokens, const char *code)
{
	tokens.empty();
	tokens.push_back(std::make_pair(ERROR, code)); //code가 char형인데 되는지 모르겠음. 여기는 string
}

RequestLexer::Tokens RequestLexer::httpTokenize(std::string inputRequestMessage)	
{
	Tokens	tokens;

	requestMessage = inputRequestMessage;
	try
	{
		RequestLexer::startLineTokenize(tokens);
		RequestLexer::headerLineTokenize(tokens);
		RequestLexer::bodyLineTokenize(tokens);
	}
	catch(const char *code)
	{
		errorHandling(tokens, code);
	}
	return (tokens);
}


RequestLexer::Tokens RequestLexer::startLineHeaderLineTokenize(const std::string &inputRequestMessage)
{
	Tokens	tokens;

	requestMessage = inputRequestMessage;
	try
	{
		RequestLexer::startLineTokenize(tokens);
		RequestLexer::headerLineTokenize(tokens);
	}
	catch(const char *code)
	{
		errorHandling(tokens, code);
	}
	return (tokens);
}

RequestLexer::Tokens RequestLexer::bodyLineTokenize(const std::string &inputRequestMessage)
{
	Tokens	tokens;

	requestMessage = inputRequestMessage;
	try
	{
		RequestLexer::bodyLineTokenize(tokens);
	}
	catch(const char *code)
	{
		errorHandling(tokens, code);
	}
	return (tokens);
}
