#ifndef CONFIGTOKEN_HPP
# define CONFIGTOKEN_HPP

// TOK_LBRACKET, TOK_RBRACKET은 TOK_SERVER, TOK_LOCATION과 조합해 server block의 bracket인지, location block의 bracket인지 구분
// bitmask 활용

# define TOK_SERVER				1
# define TOK_LOCATION			2
# define TOK_LBRACKET			4
# define TOK_RBRACKET			8
# define TOK_SEMICOLON			16
# define TOK_DIRECTIVE_ONE_ARG	32
# define TOK_DIRECTIVE_MUL_ARGS	64
# define TOK_DIRECTIVE_ERR_PAGE	128
# define TOK_ARGUMENT			256

#endif