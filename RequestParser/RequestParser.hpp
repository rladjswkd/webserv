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
  typedef RequestLexer::MandatoryHeaderMap MandatoryHeaderMap;
  typedef RequestLexer::SizeType SizeType;
  typedef std::string Method, Space, HttpVersion, FieldName, FieldValue, Port, BodyType;
  typedef int HeaderType;


  private:
    //startline 검사
    static void startLineValidity(Tokens &tokens);
    static void headerLineValidity(Tokens &tokens);
    static void bodyLineValidity(Tokens &tokens);
    static void errorHandling(const char *code);
    static void throwError(const char *code, std::string errorReason);

    static void previousErrorCheck(Tokens &tokens);
    static bool isMethod(std::string str);
    static bool isSpace(std::string str);
    static bool isQueryString(std::string str);
    static void queryStringSyntaxCheck(std::string queryString);
    static bool isHttpVersion(std::string str);
    static Request::MethodType  settingMethod(std::string str);
    static void headerCheck(Token &fieldNameToken, Token &fieldValueToken);
    static void mandatoryHeaderValidity(HeaderType fieldNameType, FieldName fieldName, HeaderType fieldValueType, FieldValue fieldValue);
    static void nonMandatoryHeaderValidity(FieldName fieldName, FieldValue fieldValue);
    static void hostValidity(FieldValue fieldValue);
    static void transferEncodingValidity(FieldValue fieldValue);
    static void contentLengthValidity(FieldValue fieldValue);
    static void cookieValidity(FieldValue fieldValue);
    static void contentTypeValidity(FieldValue fieldValue, HeaderType fieldValueType);
    static void connectionValidity(FieldValue fieldValue);
    static void extractMultipartFormDataId(FieldValue fieldValue);
    static bool isFieldNameSpace(FieldName fieldName);
    static void hostPortCheck(Port port);
    static bool isInMandatoryHeader(Tokens &tokens);
    static Request::FieldValueListType splitValue(std::string input, char delimiter);
    static std::string trimAll(std::string &str);

    static void inputBodyData(Tokens &tokens);
    static void chunkedProcess();
    static Request::ChunkedListType splitBodyData(std::string input);
    static double chunkedLengthConvert(std::string str);
    static void multipartFormDataIdProcess();
    static std::string ft_toLower(std::string str);
    static std::string getBodyLine();
    static void chunkedContentLengthOverlapCheck(Tokens &tokens);

  private:	// constants
		static const Method	METHOD;
    static const Space	SPACE;
    static const HttpVersion HTTPVERSION;

  private:
		static Request	request;
    static BodyType bodyTemp;

  public:
    static Request httpParser(Tokens &tokens);
    static Request startLineHeaderLineParsing(Tokens &tokens);
    static Request bodyLineParsing(Tokens &tokens, Request &inputRequest);
};




#endif