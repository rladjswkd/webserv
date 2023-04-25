#include "RequestHandler.hpp"

bool RequestHandler::isPathAvailable(PathType path)
{   
	int mode = R_OK;
	return access(path.c_str(), mode) == 0;
}

bool RequestHandler::isPathDirectory(PathType path)
{
    return path[path.length() - 1] == '/';
}

bool RequestHandler::isIndexAvailable(PathType path, ArgumentList index)
{
    std::string indexPath;
    ArgumentList::iterator it = index.begin();    
    for(; it != index.end(); ++it)
    {
        indexPath = path + (*it);
        if (isPathAvailable(indexPath))
        {
            response.setBody(getFile(indexPath));
            return true;
        }
    }
    return false;
}

std::string RequestHandler::getDirectoryList(PathType path)
{
    DIR* dir;
    struct dirent* entry;
    std::string directoryList;
    if ((dir = opendir(path.c_str())) != NULL) 
    {
        while ((entry = readdir(dir)) != NULL) {
            directoryList += "<tr> <td> ";
            directoryList += entry->d_name;
            directoryList += " </a> </td> </tr>";
        }
        closedir(dir);
    }
    return directoryList;
}

std::string RequestHandler::createDirectoryListing(PathType path)
{
    std::string fileList = "<!DOCTYPE html> <html> <head> <title>Directory Listing</title> <style> body { font-family: Arial, sans-serif; } table { border-collapse: collapse; margin: 20px 0 0 0; } th, td { border: 1px solid #ccc; padding: 8px; text-align: left; } th { background-color: #f0f0f0; } tr:hover { background-color: #f5f5f5; } a { color: #0645ad; text-decoration: none; } a:hover { text-decoration: underline; } </style> </head> <body> <h2>Directory Listing</h2> <table> <thead> <tr> <th>Name</th> </tr> </thead> <tbody>";
    fileList += getDirectoryList(path);
    fileList += "</tbody> </table> </body> </html>";
}