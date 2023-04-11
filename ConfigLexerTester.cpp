#include "ConfigLexer.hpp"
#include <iostream>

std::string colorRed(std::string str)
{
    return ("\033[1;31m" + str + "\033[0m");
}

std::string getTokenName(int tok)
{
    switch (tok)
    {
        case 1:
            return (colorRed("TOKEN_SERVER"));				
        case 2:
            return (colorRed("TOKEN_LOCATION"));				
        case 4:
            return (colorRed("TOKEN_LBRACKET"));				
        case 8:
            return (colorRed("TOKEN_RBRACKET"));				
        case 16:
            return (colorRed("TOKEN_SEMICOLON"));			
        case 32:
            return (colorRed("TOKEN_LISTEN"));				
        case 64:
            return (colorRed("TOKEN_SERVER_NAME"));			
        case 128:
            return (colorRed("TOKEN_ERROR_PAGE"));			
        case 256:
            return (colorRed("TOKEN_CLIENT_MAX_BODY_SIZE"));	
        case 512:
            return (colorRed("TOKEN_LIMIT_EXCEPT"));			
        case 1024:
            return (colorRed("TOKEN_REDIRECT"));				
        case 2048:
            return (colorRed("TOKEN_ALIAS"));				
        case 4096:
            return (colorRed("TOKEN_AUTOINDEX"));			
        case 8192:
            return (colorRed("TOKEN_INDEX"));				
        case 16384:
            return (colorRed("TOKEN_ARGUMENT"));				
    }
    return (colorRed("INVALID TOKEN"));
}
int main()
{
    ConfigLexer::Tokens tok = ConfigLexer::tokenize("./example");
    for (std::list<ConfigLexer::Token>::iterator it = tok.begin(); it != tok.end(); it++)
        std::cout << getTokenName(it->first) << " " << it->second << std::endl;
        // std::cout << it->first << " " << it->second << std::endl;
    return (0);
}