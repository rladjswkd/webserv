#include "Lexer.hpp"
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
            return (colorRed("SERVER"));
        case 2:
            return (colorRed("LOCATION"));
        case 4:
            return (colorRed("LBRACKET"));
        case 8:
            return (colorRed("RBRACKET"));
        case 16:
            return (colorRed("SEMICOLON"));
        case 32:
            return (colorRed("ONE"));
        case 64:
            return (colorRed("MULTIPLE"));        
        case 128:
            return (colorRed("ERRORPAGE"));
        case 256:
            return (colorRed("ARGUMENT"));
    }
    return ("EEEEEEEEEEEEEEEE");
}
int main()
{
    Lexer::Tokens tok = Lexer("./example").tokenize();
    for (std::list<std::pair< int, std::string> >::iterator it = tok.begin(); it != tok.end(); it++)
        std::cout << getTokenName(it->first) << " " << it->second << std::endl;
        // std::cout << it->first << " " << it->second << std::endl;
    return (0);
}