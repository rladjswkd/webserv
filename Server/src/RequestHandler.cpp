#include "RequestHandler.hpp"

void RequestHandler::mimeMapInitial(){
    mimeMap["html"] = "text/html";
    mimeMap["htm"] = "text/html";
    mimeMap["shtml"] = "text/html";
    mimeMap["css"] = "text/css";
    mimeMap["xml"] = "text/xml";
    mimeMap["gif"] = "image/gif";
    mimeMap["jpeg"] = "image/jpeg";
    mimeMap["jpg"] = "image/jpeg";
    mimeMap["js"] = "application/javascript";
    mimeMap["atom"] = "application/atom+xml";
    mimeMap["rss"] = "application/rss+xml";
    mimeMap["mml"] = "text/mathml";
    mimeMap["txt"] = "text/plain";
    mimeMap["jad"] = "text/vnd.sun.j2me.app-descriptor";
    mimeMap["wml"] = "text/vnd.wap.wml";
    mimeMap["htc"] = "text/x-component";
    mimeMap["avif"] = "image/avif";
    mimeMap["png"] = "image/png";
    mimeMap["svg"] = "image/svg+xml";
    mimeMap["svgz"] = "image/svg+xml";
    mimeMap["tif"] = "image/tiff";
    mimeMap["tiff"] = "image/tiff";
    mimeMap["wbmp"] = "image/vnd.wap.wbmp";
    mimeMap["webp"] = "image/webp";
    mimeMap["ico"] = "image/x-icon";
    mimeMap["jng"] = "image/x-jng";
    mimeMap["bmp"] = "image/x-ms-bmp";
    mimeMap["woff"] = "font/woff";
    mimeMap["woff2"] = "font/woff2";
    mimeMap["jar"] = "application/java-archive";
    mimeMap["war"] = "application/java-archive";
    mimeMap["ear"] = "application/java-archive";
    mimeMap["json"] = "application/json";
    mimeMap["hqx"] = "application/mac-binhex40";
    mimeMap["doc"] = "application/msword";
    mimeMap["pdf"] = "application/pdf";
    mimeMap["ps"] = "application/postscript";
    mimeMap["eps"] = "application/postscript";
    mimeMap["ai"] = "application/postscript";
    mimeMap["rtf"] = "application/rtf";
    mimeMap["m3u8"] = "application/vnd.apple.mpegurl";
    mimeMap["kml"] = "application/vnd.google-earth.kml+xml";
    mimeMap["kmz"] = "application/vnd.google-earth.kmz";
    mimeMap["xls"] = "application/vnd.ms-excel";
    mimeMap["eot"] = "application/vnd.ms-fontobject";
    mimeMap["ppt"] = "application/vnd.ms-powerpoint";
    mimeMap["odg"] = "application/vnd.oasis.opendocument.graphics";
    mimeMap["odp"] = "application/vnd.oasis.opendocument.presentation";
    mimeMap["ods"] = "application/vnd.oasis.opendocument.spreadsheet";
    mimeMap["odt"] = "application/vnd.oasis.opendocument.text";
    mimeMap["pptx"] = "application/vnd.openxmlformats-officedocument.presentationml.presentation";
    mimeMap["xlsx"] = "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    mimeMap["docx"] = "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    mimeMap["wmlc"] = "application/vnd.wap.wmlc";
    mimeMap["wasm"] = "application/wasm";
    mimeMap["7z"] = "application/x-7z-compressed";
    mimeMap["cco"] = "application/x-cocoa";
    mimeMap["jardiff"] = "application/x-java-archive-diff";
    mimeMap["jnlp"] = "application/x-java-jnlp-file";
    mimeMap["run"] = "application/x-makeself";
    mimeMap["pl"] = "application/x-perl";
    mimeMap["pm"] = "application/x-perl";
    mimeMap["prc"] = "application/x-pilot";
    mimeMap["pdb"] = "application/x-pilot";
    mimeMap["rar"] = "application/x-rar-compressed";
    mimeMap["rpm"] = "application/x-redhat-package-manager";
    mimeMap["sea"] = "application/x-sea";
    mimeMap["swf"] = "application/x-shockwave-flash";
    mimeMap["sit"] = "application/x-stuffit";
    mimeMap["tcl"] = "application/x-tcl";
    mimeMap["tk"] = "application/x-tcl";
    mimeMap["der"] = "application/x-x509-ca-cert";
    mimeMap["pem"] = "application/x-x509-ca-cert";
    mimeMap["crt"] = "application/x-x509-ca-cert";
    mimeMap["xpi"] = "application/x-xpinstall";
    mimeMap["xhtml"] = "application/xhtml+xml";
    mimeMap["xspf"] = "application/xspf+xml";
    mimeMap["zip"] = "application/zip";
    mimeMap["bin"] = "application/octet-stream";
    mimeMap["exe"] = "application/octet-stream";
    mimeMap["dll"] = "application/octet-stream";
    mimeMap["deb"] = "application/octet-stream";
    mimeMap["dmg"] = "application/octet-stream";
    mimeMap["iso"] = "application/octet-stream";
    mimeMap["img"] = "application/octet-stream";
    mimeMap["msi"] = "application/octet-stream";
    mimeMap["msp"] = "application/octet-stream";
    mimeMap["msm"] = "application/octet-stream";
    mimeMap["mid"] = "audio/midi";
    mimeMap["midi"] = "audio/midi";
    mimeMap["kar"] = "audio/midi";
    mimeMap["mp3"] = "audio/mpeg";
    mimeMap["ogg"] = "audio/ogg";
    mimeMap["m4a"] = "audio/x-m4a";
    mimeMap["ra"] = "audio/x-realaudio";
    mimeMap["3gpp"] = "video/3gpp";
    mimeMap["3gp"] = "video/3gpp";
    mimeMap["ts"] = "video/mp2t";
    mimeMap["mp4"] = "video/mp4";
    mimeMap["mpeg"] = "video/mpeg";
    mimeMap["mpg"] = "video/mpeg";
    mimeMap["mov"] = "video/quicktime";
    mimeMap["webm"] = "video/webm";
    mimeMap["flv"] = "video/x-flv";
    mimeMap["m4v"] = "video/x-m4v";
    mimeMap["mng"] = "video/x-mng";
    mimeMap["asx"] = "video/x-ms-asf";
    mimeMap["asf"] = "video/x-ms-asf";
    mimeMap["wmv"] = "video/x-ms-wmv";
    mimeMap["avi"] = "video/x-msvideo";
}


std::string RequestHandler::readFileToString(const Path &filePath){
    std::ifstream   file(filePath.c_str());
    std::string     buffer;
    std::string     str;

    while (!file.eof())
    {
        std::getline(file, buffer);
        str.append(buffer + '\n');
    }
    return (str);
}

Response    RequestHandler::responseError(std::string statusCode){
    Response    response;
    Path        errorFilePath = ERROR_PAGE_DIR_PATH;

    errorFilePath += statusCode + ".html";
    response.setStatusCode(statusCode);
    response.setBody(readFileToString(errorFilePath));
    response.setKeepAlive(false);
    return response;
}

Response    RequestHandler::responseError(std::string statusCode, ErrorPageMap errorPage){
    Response            response;
    ErrorPageIterator   it;
    Path                errorFilePath = ERROR_PAGE_DIR_PATH;

    errorFilePath += statusCode + ".html";
    response.setStatusCode(statusCode);
    it = errorPage.find(statusCode);
    if (it != errorPage.end() && access(it->second.c_str(), R_OK) == 0)
        response.setBody(readFileToString(it->second));
    else
        response.setBody(readFileToString(errorFilePath));
    response.setKeepAlive(false);
    return response;
}

Response    RequestHandler::responseRedirect(std::vector<std::string> redirect){
    Response    response;

    response.setStatusCode(redirect[0]);
    response.setLocation(redirect[1]);
    response.setKeepAlive(false);
    return response;
}

Response    RequestHandler::responseRedirect(const std::string &statuscode, const std::string &location){
    Response    response;

    response.setStatusCode(statuscode);
    response.setLocation(location);
    response.setKeepAlive(false);
    return response;
}
Response    RequestHandler::responseAutoIndex(const ConfigLocation location, const Path &requestPath, const Request &request){
    Response    response;

    if (request.getMethod() != "GET")
        return responseError("405", location.getErrorPage());
    response.setStatusCode("200");
    response.setBody(createDirectoryListing(requestPath));
    response.setKeepAlive(request.getKeepAlive());
    return response;
}

Response    RequestHandler::responseIndex(int &fd, const ConfigLocation location, const Path requestPath, const Request &request){
    Path temp;
    std::vector<std::string>::const_iterator index;
    const std::vector<std::string> &indexList = location.getIndex();

    for(index = indexList.begin(); index != indexList.end(); ++index){
        temp = requestPath + *index;
        if (access(temp.c_str(), F_OK) == 0){
            if (isCGIPath(temp))
                return responseCGI(fd, location, temp, request);
            return responseFile(location, temp, request);
        }
    }
    if (location.isAutoIndexOn())
        return responseAutoIndex(location, requestPath, request);
    return responseError("404", location.getErrorPage());
}

bool    RequestHandler::isAllowed(const ArgumentList  &limitExcept, const std::string method){
    ArgumentList::const_iterator  it;

    if (limitExcept.empty())
        return true;
    for (it = limitExcept.begin(); it != limitExcept.end(); ++it){
        if (method == *it)
            return true;
    }
    return false;
}

std::string RequestHandler::getDirectoryList(PathType path)
{
    DIR* dir;
    struct dirent* entry;
    std::string directoryList, directoryName;
    if ((dir = opendir(path.c_str())) != NULL) 
    {
        while ((entry = readdir(dir)) != NULL) {
            directoryName = entry->d_name;
            if (directoryName == "." || directoryName == ".DS_Store")
                continue;
            directoryList += "<tr> <td> ";
            directoryList += "<a href=\"" + directoryName + "/\">";
            directoryList += directoryName;
            directoryList += "</a>";
            directoryList += " </td> ";
            directoryList += " <td> ";
            if (entry->d_type == DT_DIR && (directoryName != "." || directoryName != ".."))
                directoryList += "DIR";
            else
                directoryList += "FILE";            
            directoryList += " </td> </tr>";
        }
        closedir(dir);
    }
    return directoryList;
}

std::string RequestHandler::createDirectoryListing(PathType path)
{
    std::string directoryList = "<!DOCTYPE html> <html> <head> <title>Directory Listing</title> <style> body { font-family: Arial, sans-serif; } table { border-collapse: collapse; margin: 20px 0 0 0; } th, td { border: 1px solid #ccc; padding: 8px; text-align: left; } th { background-color: #f0f0f0; } tr:hover { background-color: #f5f5f5; } a { color: #0645ad; text-decoration: none; } a:hover { text-decoration: underline; } </style> </head> <body> <h2>Directory Listing</h2> <table> <thead> <tr> <th>Name</th> <th>Type</th> </tr> </thead> <tbody>";
    directoryList += getDirectoryList(path);
    directoryList += "</tbody> </table> </body> </html>";
    return directoryList;
}

bool    RequestHandler::isRequestBodyTooLarge(size_t clientMaxBodySize, size_t contentLength){
    if (clientMaxBodySize < contentLength)
        return true;
    return false;
}

bool    RequestHandler::isDirectoryPath(Path requestPath){
    if (requestPath[requestPath.length() - 1] == '/')
        return true;
    return false;
}

bool    RequestHandler::isCGIPath(Path requestPath){
    Path    rootPath = CGI_PATH;
    return (requestPath.compare(0, rootPath.size(), rootPath) == 0);
}

void    RequestHandler::setAddtionalEnv(Path requestPath, const Request &request){
    std::stringstream   ss;

    ss << request.getContentLength();
    setenv("CONTENT_LENGTH", ss.str().c_str(), 1);
    setenv("HTTP_CONTENT_LENGTH", ss.str().c_str(), 1);
    setenv("CONTENT_TYPE", request.getContentType().c_str(), 1); // TODO upload file
    setenv("HTTP_CONTENT_TYPE", request.getContentType().c_str(), 1); // TODO upload file
    setenv("QUERY_STRING", request.getQueryString().c_str(), 1);
    setenv("REQUEST_METHOD", request.getMethod().c_str(), 1);
    setenv("SCRIPT_FILENAME", requestPath.c_str(), 1);
    setenv("PATH_INFO", "", 1);
    setenv("SCRIPT_NAME", requestPath.substr(requestPath.rfind('/')).c_str(), 1);
    setenv("SERVER_NAME", request.getHost().c_str(), 1);
    setenv("SERVER_PORT", request.getPort().c_str(), 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("SERVER_SOFTWARE", "Webserver", 1);
    setenv("REDIRECT_STATUS", "", 1);
    setenv("HTTP_COOKIE", request.getCookieString().c_str(), 1);
}

void    RequestHandler::executeScript(int *pipefd, const Path requestPath, const Request &request){
    char *chr[1];
    chr[0] = NULL;
    setAddtionalEnv(requestPath, request);
    dup2(*(pipefd + W_PIPE_READ), STDIN_FILENO); // FIXME throw except
    dup2(*(pipefd + R_PIPE_WRITE), STDOUT_FILENO);
    close(*(pipefd + W_PIPE_READ));
    close(*(pipefd + W_PIPE_WRITE));
    close(*(pipefd + R_PIPE_READ));
    close(*(pipefd + R_PIPE_WRITE));
    execve(requestPath.c_str(), chr, environ);
    exit(-1);
}

Response    RequestHandler::responseCGI(int &fd, const ConfigLocation &location, const Path requestPath, const Request &request){
    // TODO extension check
    // FIXME exit or exception?
    Response    response;
    pid_t       pid;
    int         pipefd[4];
    std::string requestBody = request.getBody();

    if (access(requestPath.c_str(), F_OK) != 0)
        return responseError("404", location.getErrorPage());
    if (pipe(pipefd) < 0 || pipe(pipefd + 2) < 0)
        std::cerr << "PIPE ERROR" << std::endl; // FIXME exit or exception? or 5XX responseError => pipe ERROR about FD.
    pid = fork();
    if (pid < 0)
        std::cerr << "FORK ERROR" << std::endl; // FIXME except
    else if (pid == 0)
        executeScript(pipefd, requestPath, request);
    if (write(pipefd[W_PIPE_WRITE], requestBody.c_str(), requestBody.length()) == -1)
        std::cerr << "PIPE WRITE ERROR" << std::endl; // 
    close(pipefd[W_PIPE_WRITE]);
    close(pipefd[W_PIPE_READ]);
    close(pipefd[R_PIPE_WRITE]);
    fd = pipefd[R_PIPE_READ];
    response.setStatusCode("200");
    return response;
}

bool    RequestHandler::isDirectoryFile(const Path requestPath){
    return (opendir(requestPath.c_str()) != NULL);
}

Response    RequestHandler::responseFile(const  ConfigLocation &location, const Path requestPath, const Request &request){
    Response    response;

    // TODO Content-type
    if (access(requestPath.c_str(), F_OK) != 0)
        return responseError("404", location.getErrorPage());
    if (request.getMethod() != "GET")
        return responseError("405", location.getErrorPage());
    if (isDirectoryFile(requestPath))
        return responseRedirect("301", requestPath + "/");
    if (access(requestPath.c_str(), R_OK) != 0)
        return responseError("403", location.getErrorPage());
    response.setStatusCode("200");
    response.setBody(readFileToString(requestPath));
    response.setKeepAlive(request.getKeepAlive());
    return response;
}

Response    RequestHandler::processLocation(int &fd, const ConfigLocation &location, Route route, const Request &request){
    Path requestPath;

    if (isRequestBodyTooLarge(location.getClientMaxBodySize(), request.getContentLength()))
        return responseError("413", location.getErrorPage());
    if (!isAllowed(location.getLimitExcept(), request.getMethod()))
        return responseError("403", location.getErrorPage());
    if (location.hasAlias())
        requestPath = location.getAlias() + request.getUriPath().erase(0, route.length());
    else
        requestPath = ROOT_PATH + request.getUriPath();
    if (isDirectoryPath(requestPath))
        return responseIndex(fd, location, requestPath, request);
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
    const ConfigServer              &server = config.getServer(socketaddr, request.getHost());
    ConfigServer::const_iterator    locationIt;
    std::string                     errorCode;

    errorCode = request.getErrorCode();
    if (!errorCode.empty())
        return responseError(errorCode);
    if (server.hasRedirect())
        return responseRedirect(server.getRedirect());
    if (!resolveRerativePath(request))
        return responseError("400", server.getErrorPage());
    locationIt = server.findLocation(request.getUriPath());
    if (locationIt == server.end())
        return responseError("404", server.getErrorPage());
    if (locationIt->second.hasRedirect())
        return responseRedirect(locationIt->second.getRedirect());
    return processLocation(fd, locationIt->second, locationIt->first, request);
}