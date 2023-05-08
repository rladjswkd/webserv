#include "Block.hpp"

BaseBlock::BaseBlock()
: clientMaxBodySize(CLIENT_MAX_BODY_SIZE_DEFAULT)
, autoIndex(AUTOINDEX_DEFAULT)
, mask()
{
	index.push_back(INDEX_DEFAULT);
}

void BaseBlock::setErrorPage(ErrorCode code, ErrorPage page)
{
	mask |= ERROR_PAGE_BIT;
	errorPage.insert(std::make_pair(code, page));	
}

void BaseBlock::setClientMaxBodySize(size_t arg)
{
	mask |= CLIENT_MAX_BODY_SIZE_BIT;
	clientMaxBodySize = arg;
}

void BaseBlock::setAutoIndex(bool arg)
{
	mask |= AUTOINDEX_BIT;
	autoIndex = arg;
}

void BaseBlock::setIndex(ArgumentList list)
{
	mask |= INDEX_BIT;
	index = list;
}

void BaseBlock::setDirectivesBase(const BaseBlock &other)
{
	if (!(mask & ERROR_PAGE_BIT))
		this->errorPage = other.errorPage;
	if (!(mask & CLIENT_MAX_BODY_SIZE_BIT))
		this->clientMaxBodySize = other.clientMaxBodySize;
	if (!(mask & AUTOINDEX_BIT))
		this->autoIndex = other.autoIndex;
	if (!(mask & INDEX_BIT))
		this->index = other.index;
}

size_t BaseBlock::getClientMaxBodySize() const
{
	return (clientMaxBodySize);
}

bool BaseBlock::isAutoIndexOn() const
{
	return (autoIndex);
}

const BaseBlock::ArgumentList &BaseBlock::getIndex() const
{
	return (index);
}

const BaseBlock::ErrorPageMap &BaseBlock::getErrorPage() const
{
	return (errorPage);
}

void InterBlock::setRedirect(ArgumentList list)
{
	mask |= REDIRECT_BIT;
	redirect = list;
}

void InterBlock::setDirectivesInter(const InterBlock &other)
{
	this->setDirectivesBase(other);
	if (!(mask & REDIRECT_BIT))
		this->redirect = other.redirect;
}

bool InterBlock::hasRedirect() const
{
	return (redirect.size());
}

const InterBlock::ArgumentList &InterBlock::getRedirect() const
{
	return (redirect);
}
