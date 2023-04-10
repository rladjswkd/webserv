#ifndef CONFIGTOKEN_HPP
# define CONFIGTOKEN_HPP

// TOK_LBRACKET, TOK_RBRACKET은 TOK_SERVER, TOK_LOCATION과 조합해 server block의 bracket인지, location block의 bracket인지 구분
// bitmask 활용

# define TOKEN_SERVER				1
# define TOKEN_LOCATION				2
# define TOKEN_LBRACKET				4
# define TOKEN_RBRACKET				8
# define TOKEN_SEMICOLON			16
# define TOKEN_DIRECTIVE_ONE		32
# define TOKEN_DIRECTIVE_MULT		64
# define TOKEN_ARGUMENT				128

#endif