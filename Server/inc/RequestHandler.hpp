#ifndef REQUEST_HANDLER_H
# define REQUEST_HANDLER_H

#include "Request.hpp"
#include "Response.hpp"
#include "Block.hpp"
#include "Config.hpp"
#include "ConfigLocation.hpp"
#include <unistd.h>
#include <dirent.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cerrno>

# define ERROR_PAGE_DIR_PATH    "./ErrorPage/"
# define CGI_PATH               "./cgi-bin/"
# define ROOT_PATH              "."
# define W_PIPE_READ            0
# define W_PIPE_WRITE           1
# define R_PIPE_READ            2
# define R_PIPE_WRITE           3

extern char **environ;

class RequestHandler
{
    public:
        typedef std::string PathType;
        typedef const std::string							Host, Port;
        typedef std::pair<Host, Port>						SocketAddr;
        typedef Request::HostType                           HostType;
        typedef std::string                                 Uri, Route, Path;
        typedef BaseBlock::ErrorPageMap::iterator           ErrorPageIterator;
        typedef BaseBlock::ErrorPageMap                     ErrorPageMap;
        typedef BaseBlock::ArgumentList                     ArgumentList;

    private:
        static Request request;
        static Config config;

    private:
        RequestHandler();
        static std::string createDirectoryListing(PathType path);
        static std::string getDirectoryList(PathType path);
        static Response processLocation(int &fd, const ConfigLocation &location, Route route, const Request &request);     
        static std::string getFile(PathType path);
        static std::string readFileToString(const Path &filePath);
        static Response responseError(std::string statusCode, ErrorPageMap errorPage);
        static Response responseRedirect(std::vector<std::string> redirect);
        static Response responseRedirect(const std::string &statuscode, const std::string &location);
        static Response responseIndex(int &fd, const ConfigLocation location, const Path requestPath, const Request &request);
        static bool     isAllowed(const ArgumentList  &limitExcept, const std::string method);
        static bool     isRequestBodyTooLarge(size_t clientMaxBodySize, size_t contentLength);
        static bool     isDirectoryPath(Path requestPath);
        static Response responseAutoIndex(const ConfigLocation location, const Path &requestPath, const Request &request);
        static bool     isCGIPath(Path requestPath);
        static void     setAddtionalEnv(Path requestPath, const Request &request);
        static void     executeScript(int *pipefd, const Path requestPath, const Request &request);
        static Response responseCGI(int &fd, const ConfigLocation &location, const Path requestPath, const Request &request);
        static Response responseFile(const  ConfigLocation &location, const Path requestPath, const Request &request);
        static bool     isDirectoryFile(const Path requestPath);
        static void     tokenizeUriPath(std::vector<std::string> &tokens, Path uriPath);
        static bool     resolveRerativePath(Request &request);
    public:
        static  Response    responseError(std::string statusCode);
        static  Response    processRequest(int &fd, const SocketAddr &socketaddr, const Config &config, Request &request);
};

#endif