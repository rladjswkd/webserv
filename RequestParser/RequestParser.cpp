#include "RequestParser.hpp"

Request RequestParser::request;
const RequestParser::Method RequestParser::METHOD = REQUEST_METHOD_LITERAL;
const RequestParser::Space RequestParser::SPACE = SP_LITERAL;
const RequestParser::HttpVersion RequestParser::HTTPVERSION = HTTP_VERSION;

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
  if (tokens.size() < 5)
    throw ("400");
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
    throw ("400");
  if (!isSpace(sp1.second) || !isSpace(sp2.second))
    throw ("400");
  if (!isHttpVersion(httpVersion.second))
    throw ("400");
  
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
    throw("400");
  request.setPort(portNum);
}

void RequestParser::hostValidity(FieldName fieldName, FieldValue fieldValue)
{
  if (fieldValue.empty()) //fieldValue 비었는지 확인함.
    throw ("400");
  if (request.getHost().size() > 0) //Host 2개 이상 들어왔는지 체크, 이미 Host에 넣었으면 size가 0이상
    throw ("400");
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
      throw ("400");
    
    if (chunked == 1 && currStr == "chunked") //chunked 중복으로 나오면 문법오류
      throw ("400");
    
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
      throw ("400");
    if (contentLength != checkNum) //뒤 숫자가 앞과 다르면 문법 오류
      throw ("400");
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
      throw ("400");
    request.setCookie(trimAll(*(cookieKeyValue.begin())), trimAll(*(--cookieKeyValue.end())));
  }
}

void RequestParser::mandatoryHeaderValidity(HeaderType fieldNameType, FieldName fieldName, FieldValue fieldValue)
{
  if (isFieldNameSpace(fieldName))
    throw ("400");
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
    default:
      break;
  }
}

void RequestParser::nonMandatoryHeaderValidity(HeaderType fieldNameType, FieldName fieldName, FieldValue fieldValue)
{
  if (isFieldNameSpace(fieldName))
    throw ("400");
  request.setHeaderFields(fieldName, trimAll(fieldValue));
}

void RequestParser::headerCheck(Tokens &tokens)
{
  HeaderType fieldNameType = (*tokens.begin()).first;
  FieldName fieldName = (*tokens.begin()).second;
  if (tokens.size() > 0)
    tokens.pop_front();
  else
    throw ("400");
  HeaderType fieldValueType = (*tokens.begin()).first;
  FieldValue fieldValue = (*tokens.begin()).second;
  
  if (HOST <= fieldNameType and fieldNameType <= COOKIE_VALUE)
    mandatoryHeaderValidity(fieldNameType, fieldName, fieldValue);
  else
    nonMandatoryHeaderValidity(fieldNameType, fieldName, fieldValue);
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
    throw("400");
  
  return true;
}

void RequestParser::headerLineValidity(Tokens &tokens)
{
  isInMandatoryHeader(tokens);
  if (tokens.size() == 0)
    throw("400");
  int currTokenType = (*tokens.begin()).first;
  while (FIELDNAME <= currTokenType and currTokenType <= COOKIE_VALUE)
  {
    headerCheck(tokens);
    if (tokens.size() > 0)
      tokens.pop_front();
    else
      break;
    currTokenType = (*tokens.begin()).first;
  }
}

void RequestParser::bodyLineValidity(Tokens &tokens)
{
  int currTokenType;

  Tokens::iterator it = tokens.begin();
  for(; it != tokens.end(); ++it)
  {
    currTokenType = (*tokens.begin()).first;
    if (currTokenType == BODY)
      request.setBody((*it).second);
  }
}

void RequestParser::errorHandling(const char *code)
{
  std::cout << "Error : " << code << std::endl;
  request.setErrorCode(code);
}

void RequestParser::previousErrorCheck(Tokens &tokens)
{
  if ((*tokens.begin()).first == ERROR)
    throw (*tokens.begin()).second;
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
