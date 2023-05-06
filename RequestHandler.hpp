#ifndef REQUEST_HANDLER_H
# define REQUEST_HANDLER_H

#include "./RequestParser/Request.hpp"
#include "./ResponseHandler/Response.hpp"
#include "Block.hpp"
#include "Config.hpp"
#include "ConfigLocation.hpp"
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>

# define ERROR_PAGE_DIR_PATH    "./ResponseHandler/ErrorPage/"
# define ROOT_PATH              ""
# define READ                   0
# define WRITE                  1

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

    private:
        static Request request;
        static Config config;

    private:
        RequestHandler();
        static Response processLocation(int &fd, const ConfigLocation &location, Route route, const Request &request);     
        static bool isMethodAvailable();
        static PathType createPath(std::string route, std::string alias);

        static bool isPathAvailable(PathType path);
        static bool isPathDirectory(PathType path);
        static bool isIndexAvailable(PathType path, ArgumentList index); //index vector 순회하면서 찾아봄
        static std::string getDirectoryList(PathType path);

        static std::string getFile(PathType path);
        static std::string readFileToString(std::string &filePath);
        static Response responseError(std::string statusCode, ErrorPageMap errorPage);
        static Response responseRedirect(std::vector<std::string> redirect);
        static Response responseIndex(const ConfigLocation location, const Path requestPath, const Request &request);
        static bool     isAllowed(std::vector<std::string>  &limitExcept, const std::string method);
        static bool     isRequestBodyTooLarge(size_t clientMaxBodySize, size_t contentLength);
        static bool     isDirectoryPath(Path requestPath);
        static bool     isCGIPath(Path requestPath);
        static void     setAddtionalEnv(const Path requestPath, const Request &request);
        static void     xecuteScript(int *pipefd, const Path requestPath, const Request &request);
        static Response responseCGI(int &fd, const ConfigLocation &location, const Path requestPath, const Request &request);
        static Response responseFile(const  ConfigLocation &location, const Path requestPath, const Request &request);
        static void     tokenizeUriPath(std::vector<std::string> &tokens, Path uriPath);
        static bool     resolveRerativePath(Request &request);
    public:
        static  std::string createDirectoryListing(PathType path);
    public:
        static  Response    responseError(std::string statusCode);
        static  Response    processRequest(int &fd, const SocketAddr &socketaddr, const Config &config, Request &request);
};

#endif