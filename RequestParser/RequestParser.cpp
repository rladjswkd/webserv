#include "RequestParser.hpp"

Request RequestParser::request;
RequestParser::BodyType RequestParser::bodyTemp;
const RequestParser::Method RequestParser::METHOD = REQUEST_METHOD_LITERAL;
const RequestParser::Space RequestParser::SPACE = SP_LITERAL;
const RequestParser::HttpVersion RequestParser::HTTPVERSION = HTTP_VERSION;


//util__
std::string RequestParser::ft_toLower(std::string str)
{
  for (int i = 0; i < str.length(); i++) {
    str[i] = std::tolower(str[i]);
  }
	return str;
}

std::string RequestParser::trimAll(std::string &str)
{
  std::string::size_type pos = str.find_last_not_of(" \t\r\n");
  if(pos != std::string::npos)
  {
      str.erase(pos + 1);
      pos = str.find_first_not_of(" \t\r\n");
      if(pos != std::string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
  return str;
}

//util__end



Request::FieldValueListType RequestParser::splitValue(std::string input, char delimiter) {
    Request::FieldValueListType result;
    std::stringstream ss(input);
    std::string temp;
 
    while (getline(ss, temp, delimiter)) {
        result.push_back(temp);
    }
    return result;
}

bool RequestParser::isMethod(std::string str)
{
    return (METHOD.find(str) != std::string::npos);
}

bool RequestParser::isSpace(std::string str)
{
    return (SPACE.find(str) != std::string::npos);
}

bool RequestParser::isHttpVersion(std::string str)
{
    return (HTTPVERSION.find(str) != std::string::npos);
}

Request::method_enum  RequestParser::settingMethod(std::string str)
{
  if (str == "GET")
    return Request::GET;
  else if (str == "POST")
    return Request::POST;
  return Request::DELETE;
}

void RequestParser::startLineValidity(Tokens &tokens)
{
  Token method = *(tokens.begin());
  tokens.pop_front();
  Token sp1 = *(tokens.begin());
  tokens.pop_front();
  Token uri = *(tokens.begin());
  tokens.pop_front();
  Token sp2 = *(tokens.begin());
  tokens.pop_front();
  Token httpVersion = *(tokens.begin());
  tokens.pop_front();
  if (!isMethod(method.second))
    throwError("400", "startline method syntax error!");
  if (!isSpace(sp1.second) || !isSpace(sp2.second))
    throwError("400", "startline SP syntax error!");
  if (!isHttpVersion(httpVersion.second))
    throwError("400", "startline http version syntax error!");
  
  request.setMethod(settingMethod(method.second));
  request.setUriPath(uri.second);
  request.setHttpVersion(httpVersion.second);
}

bool RequestParser::isFieldNameSpace(FieldName fieldName)
{
  SizeType pos = fieldName.find(" ");
  return pos != std::string::npos;
}

void RequestParser::hostPortCheck(Port port)
{
  int portNum = atoi(port.c_str());
  if (portNum == 0)
    throwError("400", "host port syntax error!");
  request.setPort(portNum);
}

void RequestParser::hostValidity(FieldName fieldName, FieldValue fieldValue)
{
  if (fieldValue.empty()) //fieldValue 비었는지 확인함.
    throwError("400", "field value empty!");
  if (request.getHost().size() > 0) //Host 2개 이상 들어왔는지 체크, 이미 Host에 넣었으면 size가 0이상
    throwError("400", "host field is allowed only one!");
  SizeType pos = fieldValue.find(":");
  SizeType len = fieldValue.length();
  if (pos != std::string::npos)
    hostPortCheck(fieldValue.substr(pos + 1, len));
  std::string host = fieldValue.substr(0, pos);
  host = trimAll(host);
  request.setHost(host);
}

void RequestParser::transferEncodingValidity(FieldName fieldName, FieldValue fieldValue)
{
  Request::FieldValueListType transferEncodingValue;
  transferEncodingValue = splitValue(fieldValue, ',');
  if (!transferEncodingValue.size())
    return ;
  Request::FieldValueListType::iterator it = transferEncodingValue.begin();
  int cnt = 1;
  int len = transferEncodingValue.size();
  int chunked = 0;
  std::string currStr;
  
  for (; it != transferEncodingValue.end(); ++it, cnt++)
  {
    currStr = trimAll((*it));
    if (cnt == 1 && currStr == "chunked") //처음에 오면 괜찮음.
      chunked = 1;
    
    if (cnt != len && currStr == "chunked") //중간에 chunked오면 문법오류
      throwError("400", "chunked syntax error!");
    
    if (chunked == 1 && currStr == "chunked") //chunked 중복으로 나오면 문법오류
      throwError("400", "chunked syntax error!");
    
    if (chunked == 0 && cnt == len && currStr == "chunked") //마지막에 chunked있으면 ok
      chunked = 1;
  }
  if (chunked == 1)
    request.setChunked(true);
  request.setTransferEncoding(transferEncodingValue);
}

void RequestParser::contentLengthValidity(FieldName fieldName, FieldValue fieldValue)
{
  Request::FieldValueListType contentLengthValue;
  contentLengthValue = splitValue(fieldValue, ',');
  if (!contentLengthValue.size())
    return ;
  Request::FieldValueListType::iterator it = contentLengthValue.begin();
  uint32_t contentLength = 0;
  uint32_t checkNum = atoi((*it).c_str());

  for (; it != contentLengthValue.end(); ++it)
  {
    contentLength = atoi((*it).c_str());
    if (!contentLength) //숫자 외에 다른 것이 써져 있으면 문법 오류
      throwError("400", "content length syntax error!");
    if (contentLength != checkNum) //뒤 숫자가 앞과 다르면 문법 오류
      throwError("400", "content length syntax error!");
  }
  request.setContentLength(contentLength);
}

void RequestParser::cookieValidity(FieldName fieldName, FieldValue fieldValue)
{
  Request::FieldValueListType cookieValue, cookieKeyValue;
  cookieValue = splitValue(fieldValue, ';');
  if (!cookieValue.size())
    return ;
  Request::FieldValueListType::iterator it = cookieValue.begin();
  SizeType pos;

  for (; it != cookieValue.end(); ++it)
  {
    cookieKeyValue = splitValue((*it), '=');
    if (cookieKeyValue.size() != 2) //=이 없거나, 1개 이상이면 문법 오류
      throwError("400", "cookie syntax error!");
    request.setCookie(trimAll(*(cookieKeyValue.begin())), trimAll(*(--cookieKeyValue.end())));
  }
}

void RequestParser::extractMultipartFormDataId(FieldValue fieldValue)
{
  SizeType pos = fieldValue.find("boundary=");
  request.setMultipartFormDataId(fieldValue.substr(pos + 9, fieldValue.length()));
}

void RequestParser::contentTypeValidity(FieldName fieldName, FieldValue fieldValue, HeaderType fieldValueType)
{
  if (fieldValueType == MULTIPART_FORM_DATA)
  {
    extractMultipartFormDataId(fieldValue);
    request.setChunked(false);
  }
}

void RequestParser::mandatoryHeaderValidity(HeaderType fieldNameType, FieldName fieldName, HeaderType fieldValueType, FieldValue fieldValue)
{
  if (isFieldNameSpace(fieldName))
    throwError("400", "fieldname is not allowed SP!");
  switch (fieldNameType)
  {
    case HOST:
      hostValidity(fieldName, fieldValue);
      break;
    case TRANSFER_ENCODING:
      transferEncodingValidity(fieldName, fieldValue);
      break;
    case CONTENT_LENGTH:
      contentLengthValidity(fieldName, fieldValue);
      break;
    case COOKIE:
      cookieValidity(fieldName, fieldValue);
      break;
    case CONTENT_TYPE:
      contentTypeValidity(fieldName, fieldValue, fieldValueType);
      break;
    default:
      break;
  }
}

void RequestParser::nonMandatoryHeaderValidity(HeaderType fieldNameType, FieldName fieldName, FieldValue fieldValue)
{
  if (isFieldNameSpace(fieldName))
    throwError("400", "fieldname is not allowed SP!");
  request.setHeaderFields(fieldName, trimAll(fieldValue));
}

void RequestParser::headerCheck(Token &fieldNameToken, Token &fieldValueToken)
{
  if (HOST <= fieldNameToken.first and fieldNameToken.first <= CONTENT_TYPE)
    mandatoryHeaderValidity(fieldNameToken.first, fieldNameToken.second, fieldValueToken.first ,fieldValueToken.second);
  else
    nonMandatoryHeaderValidity(fieldNameToken.first, fieldNameToken.second, fieldValueToken.second);
}

bool RequestParser::isInMandatoryHeader(Tokens &tokens)
{
  int host = 0;

  Tokens::iterator it = tokens.begin();
  for(; it != tokens.end(); ++it)
  {
    if ((*it).first == HOST)
      host = 1;
  }
  if (host == 0)
    throwError("400", "host must be only one!");
  
  return true;
}

void RequestParser::headerLineValidity(Tokens &tokens)
{
  Tokens::iterator fieldNameTokenIt, fieldValueTokenIt;
  int currTokenType;

  isInMandatoryHeader(tokens);
  Tokens::iterator it = tokens.begin();
  for (; it != tokens.end(); ++it)
  {
    currTokenType = (*it).first;
    if (FIELDNAME <= currTokenType and currTokenType <= CONTENT_TYPE){
      fieldNameTokenIt = (it);
      fieldValueTokenIt = (++it);
      if (fieldValueTokenIt == tokens.end())
        throwError("400", "header field syntax error!");
      headerCheck(*fieldNameTokenIt, *fieldValueTokenIt);
    }
  }
}

void RequestParser::inputBodyData(Tokens &tokens)
{
  int currTokenType;

  Tokens::iterator it = tokens.begin();
  for(; it != tokens.end(); ++it)
  {
    currTokenType = (*it).first;
    if (currTokenType == BODY)
      request.setBody((*it).second);
  }
}

Request::ChunkedListType RequestParser::splitBodyData(std::string input) {
    Request::ChunkedListType result;
    std::stringstream ss(input);
    std::string temp;
    char checkN;
 
    while (getline(ss, temp, '\r')) {
      ss.get(checkN);
      if (checkN != '\n')
        throwError("400", "body syntax error!");
      result.push_back(temp);
    }
    return result;
}

double RequestParser::chunkedLengthConvert(std::string str)
{
  char *end;
  double num = strtoll(str.c_str(), &end, 16);
  if (num == 0 && end == str)
    throwError("400", "chunked body number syntax error!");
  return num;
}


std::string	RequestParser::getBodyLine()
{	
	std::string::size_type pos = bodyTemp.find("\r\n");
	std::string	line;
	
	if (pos == std::string::npos)
	  return ("");
	line = bodyTemp.substr(0, pos);
  bodyTemp = bodyTemp.substr(pos + 2, bodyTemp.length());
	return line;
}


void RequestParser::chunkedProcess()
{
  double chunkedLength;
  bodyTemp = request.getBody();
  std::string bodyConverted;
  std::string line;

  while ((line = getBodyLine()).size() > 0)
  {  
    chunkedLength = chunkedLengthConvert(line);    
    if (chunkedLength == 0)
      break;
    line = getBodyLine();
    if ((line).length() != chunkedLength)
      throwError("400", "chunked body syntax error!");
    bodyConverted.append(line);
  }
  request.setBody(bodyConverted);
}

void RequestParser::multipartFormDataIdProcess()
{
  std::string bodyConverted, id, contentDisposition, contentType, emptyArea, bodyPiece, multipartFormDataId;

  bodyTemp = request.getBody();
  multipartFormDataId = request.getMultipartFormDataId();
  while ((id = getBodyLine()).size() > 0)
  {
    if (id == "--" + multipartFormDataId + "--") // end point
      break;

    if (id != "--" + multipartFormDataId)
      throwError("400", "multipart form data id error!");
    
    contentDisposition = getBodyLine();
    if (id.length() == 0 || ft_toLower(contentDisposition).find("content-disposition") == std::string::npos)
      throwError("400", "content-disposition error!");
    
    contentType = getBodyLine();
    if (contentType.length() == 0 || ft_toLower(contentType).find("content-type") == std::string::npos)
      throwError("400", "content-type error!");
 
    emptyArea = getBodyLine();
    if (!emptyArea.empty())
      throwError("400", "body sytax(multipart form) error!");
    
    bodyPiece = getBodyLine();
    if (bodyPiece.length() != 0)
      bodyConverted.append(bodyPiece);
  }
  request.setBody(bodyConverted);
}

void RequestParser::bodyLineValidity(Tokens &tokens)
{
  inputBodyData(tokens);
  if (request.getMultipartFormDataId().length() > 0)
    multipartFormDataIdProcess();
  if (request.getChunked())
    chunkedProcess();
}

void RequestParser::throwError(const char *code, std::string errorReason)
{
  std::cout << "Error : " << errorReason << std::endl;
  std::cout << "Error code : " << code << std::endl;
  throw code;
}

void RequestParser::errorHandling(const char *code)
{
  request.setErrorCode(code);
}

void RequestParser::previousErrorCheck(Tokens &tokens)
{
  if ((*tokens.begin()).first == ERROR)
    throwError((*tokens.begin()).second.c_str(), "previous(httpLexer) syntax error!");
}

Request RequestParser::httpParser(Tokens &tokens)
{
  Request requestNew;

  request = requestNew;
  try
  {
    previousErrorCheck(tokens);
    startLineValidity(tokens);
    headerLineValidity(tokens);
    bodyLineValidity(tokens);
  }
  catch(const char *code)
  {
    errorHandling(code);
  }

  return request;
}
