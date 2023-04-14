#include "Block.hpp"
#include <utility>

void BaseBlock::setErrorPage(ErrorCode code, ErrorPage page)
{
	errorPage.insert(std::make_pair(code, page));	
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
