#ifndef REQUESTPARSER_H
# define REQUESTPARSER_H

#include "RequestLexer.hpp"
#include "Request.hpp"
#include <utility>
#include <sstream>
#include <iostream>
#include <cctype>
#include <cstdlib>

# define SP_LITERAL	            	" "
# define HTTP_VERSION             "HTTP/1.1"

class RequestParser
{
  typedef RequestLexer::Tokens Tokens;
  typedef RequestLexer::Token Token;
  typedef RequestLexer::SizeType SizeType;
  typedef std::string Space, HttpVersion, FieldName, FieldValue, Port, BodyType;
  typedef int HeaderType;


  private:
    //startline 검사
    static void startLineValidity(Tokens &tokens, Request &request);
    static void headerLineValidity(const Tokens &tokens, Request &request);
    static void bodyLineValidity(const Tokens &tokens, Request &request);
    static void errorHandling(const char *code, Request &request);
    static void throwError(const char *code, const std::string &errorReason);

    static void previousErrorCheck(const Tokens &tokens);
    static bool isMethod(const std::string &str);
    static bool isSpace(const std::string &str);
    static bool isQueryString(const std::string &str);
    static void queryStringSyntaxCheck(const std::string &queryString, Request &request);
    static bool isHttpVersion(const std::string &str);
    static Request::MethodType  settingMethod(const std::string &str);
    static void headerCheck(const Token &fieldNameToken, Token &fieldValueToken, Request &request);
    static void mandatoryHeaderValidity(const Token &fieldName, Token &fieldValue, Request &request);
    static void nonMandatoryHeaderValidity(const FieldName &fieldName, FieldValue &fieldValue, Request &request);
    static void hostValidity(const FieldValue &fieldValue, Request &request);
    static void transferEncodingValidity(const FieldValue &fieldValue, Request &request);
    static void contentLengthValidity(const FieldValue &fieldValue, Request &request);
    static void cookieValidity(FieldValue &fieldValue, Request &request);
    static void contentTypeValidity(FieldValue &fieldValue, const HeaderType &fieldValueType, Request &request);
    static void connectionValidity(FieldValue &fieldValue, Request &request);
    static void extractMultipartFormDataId(const FieldValue &fieldValue, Request &request);
    static bool isFieldNameSpace(const FieldName &fieldName);
    static void hostPortCheck(const Port &port, Request &request);
    static bool isInMandatoryHeader(const Tokens &tokens);
    static Request::FieldValueListType splitValue(const std::string &input, char delimiter);
    static std::string &trimAll(std::string &str);

    static void inputBodyData(const Tokens &tokens, Request &request);
    static void chunkedProcess(Request &request);
    static Request::ChunkedListType splitBodyData(const std::string &input);
    static double chunkedLengthConvert(const std::string &str);
    static void multipartFormDataIdProcess(Request &request);
    static std::string &ft_toLower(std::string &str);
    static std::string getBodyLine(BodyType &body);
    static void chunkedContentLengthOverlapCheck(const Tokens &tokens, Request &request);

  private:	// constants
    static const Space	SPACE;
    static const HttpVersion HTTPVERSION;

  public:
    // static Request httpParser(Tokens &tokens);//TODO: remove
    static void startLineHeaderLineParsing(Tokens &tokens, Request &request);
    static void bodyLineParsing(const Tokens &tokens, Request &request);
};




#endif