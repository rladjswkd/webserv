#ifndef REQUEST_H
# define REQUEST_H

#include <map>
#include <string>


class Request
{
	public:
		typedef std::map<std::string, std::string>	HeaderFieldsMap;		
		typedef std::pair<std::string, std::string>	HostType;
		typedef std::map<std::string, std::string>  CookieType;

		enum	method_enum
		{
			get = 0,
			POST = 1,
			DELETE = 2
		};

	private:
		//start line
		method_enum	method;
		std::string	uriPath;
		std::string	queryString;
		std::string	httpVersion;

		//header fields
		HeaderFieldsMap	headerFields;//헤더필드 전체를 이곳에 우선 저장 후, 필요한 내용만 꺼내 requeset 구조체 멤버변수에 저장
		HostType	host;
		std::string	transferEncoding;
		uint32_t	contentLength;
		CookieType	cookie;

		//body
		std::string	body; // /r/n/r/n 뒤를 전체 저장함.

	public:
		method_enum getMethod() const;
		std::string getUriPath() const ;
		std::string getQueryString() const;
		std::string getHttpVersion() const;
		HeaderFieldsMap getHeaderFields() const;
		HostType getHost() const;
		std::string getTransferEncoding() const;
		uint32_t getContentLength() const;
		CookieType getCookie() const;
		std::string getBody() const;

		void setMethod(method_enum method);
		void setUriPath(std::string uriPath);
		void setQueryString(std::string queryString);
		void setHttpVersion(std::string httpVersion);
		void setHeaderFields(HeaderFieldsMap headerFields);
		void setHost(HostType host);
		void setTransferEncoding(std::string transferEncoding);
		void setContentLength(uint32_t contentLength);
		void setCookie(CookieType cookie);
		void setBody(std::string body);
	
};

#endif