#include "RequestHandler.hpp"

std::string RequestHandler::readFileToString(std::string &filePath){
    std::ifstream   file(filePath.c_str());
    std::string     str;

    while (file.good())
        std::getline(file, str);
    return (str);
}

Response    RequestHandler::responseError(std::string statusCode){
    Response    response;
    Path        errorFilePath = ERROR_PAGE_DIR_PATH;

    // TODO Connection : close
    errorFilePath += statusCode;
    response.setStatusCode(statusCode);
    response.setBody(readFileToString(errorFilePath));
    response.setContentLength(response.getBody());
    return response;
}

Response    RequestHandler::responseError(std::string statusCode, ErrorPageMap errorPage){
    Response            response;
    ErrorPageIterator   it;
    Path                errorFilePath = ERROR_PAGE_DIR_PATH;

    // TODO Connection : close
    errorFilePath += statusCode;
    response.setStatusCode(statusCode);
    it = errorPage.find(statusCode);
    if (it != errorPage.end() && access(it->second.c_str(), R_OK) == 0)
        response.setBody(readFileToString(it->second));
    else
        response.setBody(readFileToString(errorFilePath));
    response.setContentLength(response.getBody());
    return response;
}

Response    RequestHandler::responseRedirect(std::vector<std::string> redirect){
    Response    response;

    // TODO Connection : keep-alive
    response.setStatusCode(redirect[0]);
    response.setLocation(redirect[1]);
    return response;
}

Response    RequestHandler::responseIndex(const ConfigLocation location, const Path requestPath, const Request &request){
    Path temp;
    std::vector<std::string>::iterator index;
    std::vector<std::string> &indexList = location.getIndex();

    for(index = indexList.begin(); index != indexList.end(); ++index){
        temp = requestPath + *index;
        if (access(temp.c_str(), F_OK) == 0)
            return responseFile(location, temp, request);
    }
    if (location.getAutoIndoex())
        return responseAutoIndex(location, requestPath, request);
    return responseError("404", location.getErrorPage());
}

bool    RequestHandler::isAllowed(std::vector<std::string>  &limitExcept, const std::string method){
    std::vector<std::string>::iterator  it;

    for (it = limitExcept.begin(); it != limitExcept.end(); ++it){
        if (method == *it)
            return true;
    }
    return false;
}

bool    RequestHandler::isRequestBodyTooLarge(size_t clientMaxBodySize, size_t contentLength){
    if (clientMaxBodySize < contentLength)
        return true;
    return false;
}

bool    RequestHandler::isDirectoryPath(Path requestPath){
    if (requestPath.back() == '/')
        return true;
    return false;
}

bool    RequestHandler::isCGIPath(Path requestPath){
    Path    rootPath = ROOT_PATH;
    if (rootPath.compare(requestPath) == 0)
        return true;
    return false;
}

void    RequestHandler::setAddtionalEnv(const Path requestPath, const Request &request){
    std::stringstream   ss;

    ss << request.getContentLength();
    setenv("CONTENT_LENGTH", ss.str().c_str(), 1);
    setenv("HTTP_CONTENT_LENGTH", ss.str().c_str(), 1);
    setenv("CONTENT_TYPE", "", 1); // TODO upload file
    setenv("HTTP_CONTENT_TYPE", "", 1); // TODO upload file
    setenv("QUERY_STRING", request.getQueryString().c_str(), 1);
    setenv("REQUEST_METHOD", request.getMethod().c_str(), 1);
    setenv("SCRIPT_FILENAME", requestPath.c_str(), 1);
    setenv("PATH_INFO", "", 1);
    setenv("SCRIPT_NAME", requestPath.substr(requestPath.rfind('/')), 1);
    setenv("SERVER_NAME", request.getHost().c_str(), 1);
    setenv("SERVER_PORT", request.getPort().c_str(), 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("SERVER_SOFTWARE", "Webserver", 1);
    setenv("HTTP_COOKIE", request.getCookie().c_str(), 1);
}

void    RequestHandler::executeScript(int *pipefd, const Path requestPath, const Request &request){

    setAddtionalEnv(requestPath, request);
    dup2(*pipefd, STDIN_FILENO);
    dup2(*(pipefd + 1), STDOUT_FILENO);
    close(*pipefd);
    close(*(pipefd + 1));
    execve(requestPath.c_str(), NULL, environ);
    exit(-1);
}

Response    RequestHandler::responseCGI(int &fd, const ConfigLocation &location, const Path requestPath, const Request &request){
    // TODO extension check
    // TODO response CGI
    Response    response;
    pid_t       pid;
    int         pipefd[2];
    std::string requestBody = request.getBody();

    if (pipe(pipefd) < 0)
        std::cerr << "PIPE ERROR" << std::endl; // FIXME exit or exception? or 5XX responseError => pipe ERROR about FD.
    pid = fork();
    if (pid < 0)
        std::cerr << "FORK ERROR" << std::endl;
    else if (pid == 0)
        executeScript(pipefd);
    if (write(pipefd[WRITE], requestBody.c_str(), requestBody.length()) == -1)
        std::cerr << "PIPE WRITE ERROR" << std::endl;
    close(pipefd[WRITE]);
    fd = pipefd[READ];
    return response;
}

Response    RequestHandler::responseFile(const  ConfigLocation &location, const Path requestPath, const Request &request){
    Response    response;

    // TODO Content-type
    if (access(requestPath.c_str(), F_OK) != 0)
        return responseError("404", location.getErrorPage());
    if (request.getMethod() != "GET")
        return responseError("405", location.getErrorPage());
    if (access(requestPath.c_str(), R_OK) != 0)
        return responseError("403", location.getErrorPage());
    response.setStatusCode("200");
    response.setBody(readFileToString(requestPath));
    response.setContentLength(response.getBody());
    return response;
}

Response    RequestHandler::processLocation(int &fd, const ConfigLocation &location, Route route, const Request &request){
    Path requestPath;

    if (isRequestBodyTooLarge(location.getclientMaxBodySize(), request.getContentLength()))
        return responseError("413", location.getErrorPage());
    if (!isAllowed(location.getLimitExcept(), request.getMethod()))
        return responseError("403", location.getErrorPage());
    if (location.hasAlias())
        requestPath = location.getAlias() + request.getUriPath().erase(0, route.length());
    else
        requestPath = ROOT_PATH + requeset.getUriPath();
    if (isDirectoryPath(requestPath))
        return responseIndex(location, requestPath, request);
    if (isCGIPath(requestPath))
        return responseCGI(fd, location, requestPath, request);
    return responseFile(location, requestPath, request);
}

void   RequestHandler::tokenizeUriPath(std::vector<std::string> &tokens, Path uriPath){
    size_t  start = 0;
    size_t  end = 0;
    std::string token;

    end = uriPath.find('/', start);
    while (end != std::string::npos) {
        token = uriPath.substr(start, end - start);
        start = end + 1;
        if (!token.empty() && token != ".") {
            tokens.push_back(token);
        }
        end = uriPath.find('/', start);
    }
    token = uriPath.substr(start);
    tokens.push_back(token);
}

bool    RequestHandler::resolveRerativePath(Request &request){
    std::vector<std::string>                    tokens;
    std::vector<std::string>                    absoluteTokens;
    Path                                        uriPath = request.getUriPath();
    std::vector<std::string>::const_iterator    it;

    if (uriPath[0] != '/')
        return false;
    tokenizeUriPath(tokens, uriPath);
    for (it = tokens.begin(); it != tokens.end(); ++it){
        if (*it == ".."){
            if (absoluteTokens.empty())
                return false;
            absoluteTokens.pop_back();
        }
        else
            absoluteTokens.push_back(*it);
    }
    uriPath.clear();
    for (it = absoluteTokens.begin(); it != absoluteTokens.end(); ++it){
        uriPath += "/";
        uriPath += *it;
    }
    request.setUriPath(uriPath);
    return true;
}

Response    RequestHandler::processRequest(int &fd, const SocketAddr &socketaddr, const Config &config, Request &request){
    ConfigServer    server;
    ConfigLocation  location;
    Route           route;
    std::string     errorCode;

    errorCode = request.getErrorCode();
    if (!errorCode.empty())
        return responseError(errorCode);
    server = config.getServer(socketaddr, request.getHost());
    if (server.hasRedirect())
        return responseRedirect(server.getRedirect());
    if (!resolveRerativePath(request))
        return responseError("400", server.getErrorPage());
    // if (!config.getLocation(request.getUriPath(), route, location))
        // return responseError("501", server.getErrorPage()); // error response status 5XX?
    if (location.hasRedirect())
        return responseRedirect(location.getRedirect());
    return processLocation(fd, location, route, request);
}