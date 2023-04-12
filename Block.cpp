#include "Block.hpp"

void BaseBlock::setErrorPage(Directive directive)
{
	errorPage = directive;
}

void BaseBlock::setClientMaxBodySize(Directive directive)
{
	clientMaxBodySize = directive;
}

void BaseBlock::setAutoIndex(Directive directive)
{
	autoIndex = directive;
}

void BaseBlock::setIndex(Directive directive)
{
	index = directive;
}

void InterBlock::setRedirect(Directive directive)
{
	redirect = directive;
}
