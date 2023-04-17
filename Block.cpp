#include "Block.hpp"

BaseBlock::BaseBlock()
: clientMaxBodySize(CLIENT_MAX_BODY_SIZE_DEFAULT)
, autoIndex(AUTOINDEX_DEFAULT)
{
	index.push_back(INDEX_DEFAULT);
}

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
