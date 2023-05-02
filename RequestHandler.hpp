#ifndef REQUEST_HANDLER_H
# define REQUEST_HANDLER_H

#include "./RequestParser/Request.hpp"
#include "./ResponseHandler/Response.hpp"
#include "Config.hpp"
#include "ConfigLocation.hpp"
#include <unistd.h>
#include <iostream>

class RequestHandler
{
    public:
        typedef std::string PathType;
        typedef const std::string							Host, Port;
        typedef std::pair<Host, Port>						SocketAddr;
        typedef Request::HostType                           HostType;
        typedef std::string                                 Uri, Route;

    private:
        static Request request;
        static Config config;

    private:
        RequestHandler();
        static Response processLocation(int &fd, ConfigLocation location, Request request);        
        static bool isMethodAvailable();
        static PathType createPath(std::string route, std::string alias);

        static bool isPathAvailable(PathType path);
        static bool isPathDirectory(PathType path);
        static bool isIndexAvailable(PathType path, ArgumentList index); //index vector 순회하면서 찾아봄
        static std::string getDirectoryList(PathType path);

        static std::string getFile(PathType path);

    public:
        static  std::string createDirectoryListing(PathType path);

    public:
        static  Response    processRequest(int &fd, const SocketAddr &socketaddr, const Config &config, const Request &request);
};

#endif