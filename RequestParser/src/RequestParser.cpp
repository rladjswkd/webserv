#include "RequestParser.hpp"
#include <algorithm>

const RequestParser::Space RequestParser::SPACE = SP_LITERAL;
const RequestParser::HttpVersion RequestParser::HTTPVERSION = HTTP_VERSION;
const RequestParser::StatusCode RequestParser::CLIENT_ERROR = "400";
const RequestParser::StatusCode RequestParser::NOT_IMPLEMENTED = "501";
const RequestParser::StatusCode RequestParser::HTTP_VERSION_NOT_SUPPORTED = "505";

//util__
std::string RequestParser::ft_toLower(std::string str)
{
  std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return (str);
}

std::string &RequestParser::trimAll(std::string &str)
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



Request::FieldValueListType RequestParser::splitValue(const std::string &input, char delimiter) {
    Request::FieldValueListType result;
    std::stringstream ss(input);
    std::string temp;
 
    while (getline(ss, temp, delimiter)) {
      result.push_back(temp);
    }
    return result;
}

bool RequestParser::isMethod(const std::string &str)
{
  return (str == "GET" || str == "POST" || str == "DELETE");
}

bool RequestParser::isSpace(const std::string &str)
{
    return (SPACE.find(str) != std::string::npos);
}

bool RequestParser::isHttpVersion(const std::string &str)
{
    return (HTTPVERSION.find(str) != std::string::npos);
}

Request::MethodType  RequestParser::settingMethod(const std::string &str)
{
  if (str == "GET")
    return "GET";
  else if (str == "POST")
    return "POST";
  return "DELETE";
}

void RequestParser::queryStringSyntaxCheck(const std::string &uri, Request &request)
{
  Request::UriListType uriList, queryStringList, keyValueList;
  std::string queryString;

  uriList = splitValue(uri, '?');
  if (uriList.size() != 2)
    throwError(CLIENT_ERROR, "uri syntax error!");

  queryString = *(--uriList.end());
  if (queryString.find("=") == std::string::npos)
    throwError(CLIENT_ERROR, "uri syntax error!");
  
  queryStringList = splitValue(queryString, '&');
  Request::UriListType::iterator it = queryStringList.begin();
  for(; it != queryStringList.end(); ++it)
  {
    keyValueList = splitValue((*it), '=');
    if (keyValueList.size() != 2 || (*keyValueList.begin()).length() == 0 || (*(++keyValueList.begin())).length() == 0)
      throwError(CLIENT_ERROR, "queryString syntax error!");
    request.setQueryStringMap(*keyValueList.begin(), *(++keyValueList.begin()));
  }
  request.setUriPath(uri.substr(0, uri.length() - queryString.length() - 1));
  request.setQueryString(queryString);
}

bool RequestParser::isQueryString(const std::string &str)
{
  if (str.find("?") != std::string::npos)
    return true;
  else
    return false;  
}

void RequestParser::startLineValidity(Tokens &tokens, Request &request)
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
    throwError(NOT_IMPLEMENTED, "startline method syntax error!");
  if (!isSpace(sp1.second) || !isSpace(sp2.second))
    throwError(CLIENT_ERROR, "startline SP syntax error!");
  if (isQueryString(uri.second))
    queryStringSyntaxCheck(uri.second, request);
  else
    request.setUriPath(uri.second);
  if (!isHttpVersion(httpVersion.second))
    throwError(HTTP_VERSION_NOT_SUPPORTED, "startline http version syntax error!");
  
  request.setMethod(settingMethod(method.second));
  request.setHttpVersion(httpVersion.second);
}

bool RequestParser::isFieldNameSpace(const FieldName &fieldName)
{
  return fieldName.find(" ") != std::string::npos;
}

void RequestParser::hostPortCheck(const Port &port, Request &request)
{
  int portNum = atoi(port.c_str());
  if (portNum == 0 || (portNum < 1024 || portNum > 49151))
    throwError(CLIENT_ERROR, "host port syntax error!");
  request.setPort(port);
}

void RequestParser::hostValidity(const FieldValue &fieldValue, Request &request)
{
  if (fieldValue.empty()) //fieldValue 비었는지 확인함.
    throwError(CLIENT_ERROR, "field value empty!");
  if (request.getHost().size() > 0) //Host 2개 이상 들어왔는지 체크, 이미 Host에 넣었으면 size가 0이상
    throwError(CLIENT_ERROR, "host field is allowed only one!");
  SizeType pos = fieldValue.find(":");
  SizeType len = fieldValue.length(); //naver.com:8000
  if (pos != std::string::npos)
    hostPortCheck(fieldValue.substr(pos + 1, len), request);
  std::string host = fieldValue.substr(0, pos);
  host = trimAll(host);
  request.setHost(host);
}

void RequestParser::transferEncodingValidity(const FieldValue &fieldValue, Request &request)
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
    if (chunked == 0 && (cnt > 1 && cnt < len)) // 중간에 chunked 나오면 에러처리
      throwError(CLIENT_ERROR, "chunked syntax error!");
    if (chunked == 1 && cnt > 1 && currStr.length() > 0) //chunked가 1개가 아니거나, chunked이후에 다른 것이 나오면 문법오류
      throwError(CLIENT_ERROR, "chunked syntax error!");
    if (chunked == 0 && cnt == len && currStr == "chunked") //마지막에 chunked있으면 ok
      chunked = 1;
  }
  if (chunked == 1)
    request.setChunked(true);
  request.setTransferEncoding(transferEncodingValue);
}

void RequestParser::contentLengthValidity(const FieldValue &fieldValue, Request &request)
{
  Request::FieldValueListType contentLengthValue;
  contentLengthValue = splitValue(fieldValue, ',');
  if (!contentLengthValue.size())
    return ;
  Request::FieldValueListType::iterator it = contentLengthValue.begin();
  size_t contentLength = 0;
  size_t checkNum = atoi((*it).c_str());

  for (; it != contentLengthValue.end(); ++it)
  {
    contentLength = atoi((*it).c_str());
    if (!contentLength) //숫자 외에 다른 것이 써져 있으면 문법 오류
      throwError(CLIENT_ERROR, "content length syntax error!");
    if (contentLength != checkNum) //뒤 숫자가 앞과 다르면 문법 오류
      throwError(CLIENT_ERROR, "content length syntax error!");
  }
  request.setContentLength(contentLength);
  request.setHeaderFields("content-length","content-length_value");
}

void RequestParser::cookieValidity(FieldValue &fieldValue, Request &request)
{
  Request::FieldValueListType cookieValue, cookieKeyValue;
  cookieValue = splitValue(fieldValue, ';');
  if (!cookieValue.size())
    return ;
  Request::FieldValueListType::iterator it = cookieValue.begin();
  for (; it != cookieValue.end(); ++it)
  {
    cookieKeyValue = splitValue((*it), '=');
    if (cookieKeyValue.size() != 2) //=이 없거나, 1개 이상이면 문법 오류
      throwError(CLIENT_ERROR, "cookie syntax error!");
    request.setCookie(trimAll(*(cookieKeyValue.begin())), trimAll(*(--cookieKeyValue.end())));
  }
  request.setCookieString(trimAll(fieldValue));
}

void RequestParser::extractMultipartFormDataId(const FieldValue &fieldValue, Request &request)
{
  request.setMultipartFormDataId(fieldValue.substr(fieldValue.find("boundary=") + 9, fieldValue.length()));
}

void RequestParser::contentTypeValidity(FieldValue &fieldValue, const HeaderType &fieldValueType, Request &request)
{
  if (fieldValueType == MULTIPART_FORM_DATA)
  {
    extractMultipartFormDataId(fieldValue, request);
    request.setChunked(false);
  }
  request.setContentType(trimAll(fieldValue));
}

void RequestParser::connectionValidity(FieldValue &fieldValue, Request &request)
{
  if (ft_toLower(trimAll(fieldValue)) == "close")
    request.setKeepAlive(false);
}

void RequestParser::mandatoryHeaderValidity(const Token &fieldNameToken, Token &fieldValueToken, Request &request)
{
  const HeaderType  &fieldValueType = fieldValueToken.first;
  FieldValue  &fieldValue = fieldValueToken.second;

  if (isFieldNameSpace(fieldNameToken.second))
    throwError(CLIENT_ERROR, "fieldname is not allowed SP!");
  switch (fieldNameToken.first)
  {
    case HOST:
      return (hostValidity(fieldValue, request));
    case TRANSFER_ENCODING:
      return (transferEncodingValidity(fieldValue, request));
    case CONTENT_LENGTH:
      return (contentLengthValidity(fieldValue, request));
    case COOKIE:
      return (cookieValidity(fieldValue, request));
    case CONTENT_TYPE:
      return (contentTypeValidity(fieldValue, fieldValueType, request));
    case CONNECTION:
      return (connectionValidity(fieldValue, request));
    default:
      break;
  }
}

void RequestParser::nonMandatoryHeaderValidity(const FieldName &fieldName, FieldValue &fieldValue, Request &request)
{
  if (isFieldNameSpace(fieldName))
    throwError(CLIENT_ERROR, "fieldname is not allowed SP!");
  request.setHeaderFields(fieldName, trimAll(fieldValue));
}

void RequestParser::headerCheck(const Token &fieldNameToken, Token &fieldValueToken, Request &request)
{
  if (HOST <= fieldNameToken.first && fieldNameToken.first <= CONTENT_TYPE)
    mandatoryHeaderValidity(fieldNameToken, fieldValueToken, request);
  else
    nonMandatoryHeaderValidity(fieldNameToken.second, fieldValueToken.second, request);
}

bool RequestParser::isInMandatoryHeader(const Tokens &tokens)
{
  int host = 0;

  Tokens::const_iterator it = tokens.begin();
  for(; it != tokens.end(); ++it)
  {
    if ((*it).first == HOST)
      host = 1;
  }
  if (host == 0)
    throwError(CLIENT_ERROR, "host must be only one!");
  
  return true;
}

void RequestParser::chunkedContentLengthOverlapCheck(const Tokens &tokens, Request &request)
{
  int currTokenType;
  Tokens::const_iterator it = tokens.begin();
  if (!request.getChunked()) // chunked 아니면 early return;
    return ;
  for (; it != tokens.end(); ++it) // chunked이고 CONTENT_LENGTH가 있는 대상은 connection을 keep-alive가 아닌 close를 처리함.
  {
    currTokenType = (*it).first;
    if (currTokenType == CONTENT_LENGTH) {
      request.setKeepAlive(false);
    }
  }
}

void RequestParser::headerLineValidity(const Tokens &tokens, Request &request)
{
  Tokens::const_iterator fieldNameTokenIt, fieldValueTokenIt;
  int currTokenType;

  isInMandatoryHeader(tokens);
  Tokens::const_iterator it = tokens.begin();
  for (; it != tokens.end(); ++it)
  {
    currTokenType = (*it).first;
    if (FIELDNAME <= currTokenType && currTokenType <= CONTENT_TYPE){
      fieldNameTokenIt = (it);
      fieldValueTokenIt = (++it);
      if (fieldValueTokenIt == tokens.end())
        throwError(CLIENT_ERROR, "header field syntax error!");
      headerCheck(*fieldNameTokenIt, const_cast<Token &>(*fieldValueTokenIt), request);
    }
  }
  chunkedContentLengthOverlapCheck(tokens, request);
}

void RequestParser::inputBodyData(const Tokens &tokens, Request &request)
{
  int currTokenType;

  Tokens::const_iterator it = tokens.begin();
  for(; it != tokens.end(); ++it)
  {
    currTokenType = (*it).first;
    if (currTokenType == BODY)
      request.setBody((*it).second);
  }
}

Request::ChunkedListType RequestParser::splitBodyData(const std::string &input) {
    Request::ChunkedListType result;
    std::stringstream ss(input);
    std::string temp;
    char checkN;
 
    while (getline(ss, temp, '\r')) {
      ss.get(checkN);
      if (checkN != '\n')
        throwError(CLIENT_ERROR, "body syntax error!");
      result.push_back(temp);
    }
    return result;
}

double RequestParser::chunkedLengthConvert(const std::string &str)
{
  char *end;
  double num = strtol(str.c_str(), &end, 16);
  
  if (*end != '\0' && *end != ';')
    throwError(CLIENT_ERROR, "chunked body number syntax error!");
  return num;
}


std::string	RequestParser::getBodyLine(BodyType &body)
{	
	std::string::size_type pos = body.find("\r\n");
	std::string	line;
	
	if (pos == std::string::npos)
	  return ("");
	line = body.substr(0, pos);
  body.erase(0, pos + 2);
	return line;
}


void RequestParser::chunkedProcess(Request &request)
{
  double chunkedLength;
  BodyType  body = request.getBody();
  std::string bodyConverted;
  std::string line;

  while ((line = getBodyLine(body)).size() > 0)
  {  
    chunkedLength = chunkedLengthConvert(line);    
    if (chunkedLength == 0)
      break;
    line = getBodyLine(body);
    if ((line).length() != chunkedLength)
      throwError(CLIENT_ERROR, "chunked body syntax error!");
    bodyConverted.append(line);
  }
  request.setBody(bodyConverted);
}

void RequestParser::multipartFormDataIdProcess(Request &request)
{
  std::string bodyConverted, id, contentDisposition, contentType, emptyArea, bodyPiece, multipartFormDataId;
  BodyType  body = request.getBody();

  multipartFormDataId = request.getMultipartFormDataId();
  while ((id = getBodyLine(body)).size() > 0)
  {
    if (id == "--" + multipartFormDataId + "--") // end point
      break;

    if (id != "--" + multipartFormDataId)
      throwError(CLIENT_ERROR, "multipart form data id error!");
    
    contentDisposition = getBodyLine(body);
    if (id.length() == 0 || ft_toLower(contentDisposition).find("content-disposition") != 0)
      throwError(CLIENT_ERROR, "content-disposition error!");
    
    contentType = getBodyLine(body);
    if (contentType.length() == 0 || ft_toLower(contentType).find("content-type") != 0)
      throwError(CLIENT_ERROR, "content-type error!");
 
    emptyArea = getBodyLine(body);
    if (!emptyArea.empty())
      throwError(CLIENT_ERROR, "body sytax(multipart form) error!");
    
    bodyPiece = getBodyLine(body);
    if (bodyPiece.length() != 0)
      bodyConverted.append(bodyPiece);
  }
  request.setBody(bodyConverted);
}

void RequestParser::bodyLineValidity(const Tokens &tokens, Request &request)
{
  inputBodyData(tokens, request);
  if (request.getChunked())
    chunkedProcess(request);
  if (request.getHeaderFields().count("content-length") == 0)
    request.setContentLength(request.getBody().length());
}

void RequestParser::throwError(const std::string &code, const std::string &errorReason)
{
  std::cout << "Error : " << errorReason << std::endl;
  std::cout << "Error code : " << code << std::endl;
  throw code;
}

void RequestParser::errorHandling(const std::string &code, Request &request)
{
  request.setErrorCode(code);
}

void RequestParser::previousErrorCheck(const Tokens &tokens)
{
  if ((*tokens.begin()).first == ERROR)
    throwError((*tokens.begin()).second.c_str(), "previous(httpLexer) syntax error!");
}

void RequestParser::startLineHeaderLineParsing(Tokens &tokens, Request &request)
{
  try
  {
    previousErrorCheck(tokens);
    startLineValidity(tokens, request);
    headerLineValidity(tokens, request);
  }
  catch (const std::string &code)
  {
    errorHandling(code, request);
  }
}

void  RequestParser::bodyLineParsing(const Tokens &tokens, Request &request)
{
  try
  {
    previousErrorCheck(tokens);
    bodyLineValidity(tokens, request);
  }
  catch (const std::string &code)
  {
    errorHandling(code, request);
  }
}