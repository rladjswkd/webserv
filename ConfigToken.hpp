#ifndef CONFIGTOKEN_HPP
# define CONFIGTOKEN_HPP

// TOK_LBRACKET, TOK_RBRACKET은 TOK_SERVER, TOK_LOCATION과 조합해 server block의 bracket인지, location block의 bracket인지 구분
// bitmask 활용

# define TOKEN_SERVER				1
# define TOKEN_LOCATION				2
# define TOKEN_LBRACKET				4
# define TOKEN_RBRACKET				8
# define TOKEN_SEMICOLON			16

# define TOKEN_LISTEN				32
# define TOKEN_SERVER_NAME			64
# define TOKEN_ERROR_PAGE			128
# define TOKEN_CLIENT_MAX_BODY_SIZE	256
# define TOKEN_LIMIT_EXCEPT			512
# define TOKEN_REDIRECT				1024
# define TOKEN_ALIAS				2048
# define TOKEN_AUTOINDEX			4096
# define TOKEN_INDEX				8192
# define TOKEN_ARGUMENT				16384

#endif