#include "Block.hpp"

void BaseBlock::setErrorPage(ArgumentList list)
{
	errorPage = list;
}

void BaseBlock::setClientMaxBodySize(Argument arg)
{
	clientMaxBodySize = arg;
}

void BaseBlock::setAutoIndex(Argument arg)
{
	autoIndex = arg;
}

void BaseBlock::setIndex(ArgumentList list)
{
	index = list;
}

void InterBlock::setRedirect(ArgumentList list)
{
	redirect = list;
}
