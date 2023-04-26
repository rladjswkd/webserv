#ifndef REQUEST_HANDLER_H
# define REQUEST_HANDLER_H

// #include "Request.hpp"
// #include "Response.hpp"
#include "Config.hpp"
#include "ConfigLocation.hpp"
#include <unistd.h>
#include <dirent.h> //directoryListing
#include <iostream>

class RequestHandler
{
    public:
        typedef std::string PathType;
        typedef InterBlock::ArgumentList ArgumentList;

    // private:
        // static Request request;
        // static Config config;

    private:
        // RequestHanlder();
        // static Response processLocation(ConfigLocation configLocation, UriType uri);        
        static bool isMethodAvailable();
        static PathType createPath(std::string route, std::string alias);

        static bool isPathAvailable(PathType path);
        static bool isPathDirectory(PathType path);
        static bool isIndexAvailable(PathType path, ArgumentList index); //index vector 순회하면서 찾아봄
        static std::string getDirectoryList(PathType path);

        static std::string getFile(PathType path);

    public:
        static std::string createDirectoryListing(PathType path);

    // public:
        // static Response processRequest(Request request, Config config);
};

#endif