#ifndef REQUESTPARSER_H
# define REQUESTPARSER_H

#include "RequestLexer.hpp"
#include "Request.hpp"
#include <utility>
#include <sstream>
#include <cctype>

# define REQUEST_METHOD_LITERAL		"GETPOSTDELETE"
# define SP_LITERAL	            	" "
# define HTTP_VERSION             "HTTP/1.1"

class RequestParser
{
  typedef RequestLexer::Tokens Tokens;
  typedef RequestLexer::Token Token;
  typedef RequestLexer::MandatoryHeaderMap MandatoryHeaderMap;
  typedef RequestLexer::SizeType SizeType;
  typedef std::string Method, Space, HttpVersion, FieldName, FieldValue, Port;
  typedef int HeaderType;


  private:
    //startline 검사
    static void startLineValidity(Tokens &tokens);
    static void headerLineValidity(Tokens &tokens);
    static void bodyLineValidity(Tokens &tokens);
    static void errorHandling(const char *code);

    static void previousErrorCheck(Tokens &tokens);
    static bool isMethod(std::string str);
    static bool isSpace(std::string str);
    static bool isHttpVersion(std::string str);
    static Request::method_enum  settingMethod(std::string str);
    static void headerCheck(Token &fieldNameToken, Token &fieldValueToken);
    static void mandatoryHeaderValidity(HeaderType fieldNameType, FieldName fieldName, FieldValue fieldValue);
    static void nonMandatoryHeaderValidity(HeaderType fieldNameType, FieldName fieldName, FieldValue fieldValue);
    static void hostValidity(FieldName fieldName, FieldValue fieldValue);
    static void transferEncodingValidity(FieldName fieldName, FieldValue fieldValue);
    static void contentLengthValidity(FieldName fieldName, FieldValue fieldValue);
    static void cookieValidity(FieldName fieldName, FieldValue fieldValue);
    static bool isFieldNameSpace(FieldName fieldName);
    static void hostPortCheck(Port port);
    static bool isInMandatoryHeader(Tokens &tokens);
    static Request::FieldValueListType splitValue(std::string input, char delimiter);
    static std::string trimAll(std::string &str);

  private:	// constants
		static const Method	METHOD;
    static const Space	SPACE;
    static const HttpVersion HTTPVERSION;

  private:
		static Request	request;

  public:
    static Request httpParser(Tokens &tokens);
};




#endif