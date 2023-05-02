#include "RequestHandler.hpp"


Response    RequestHandler::processRequest(int &fd, const SocketAddr &socketaddr, const Config &config, const Request &request){
    ConfigServer    server;
    ConfigLocation  location;
    Route           route;

    fd = 0; //default value if Server didn't initalize it.
    server = config.getServer(socketaddr, request.getHost());
    if (server.hasRedirect())
        return responseRedirect(server.getRedirect());
    if (!config.getLocation(request.getUriPath(), route, location))
        return responseError(501, server.getErrorPage()); // error response status 5XX?
    if (location.hasRedirect())
        return responseRedirect(location.getRedirect());
    return processLocation(fd, location, route, request);
}