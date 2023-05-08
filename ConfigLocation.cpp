#include "ConfigLocation.hpp"

void ConfigLocation::setAlias(Argument list)
{
	alias = list;
}

void ConfigLocation::setLimitExcept(ArgumentList list)
{
	limitExcept = list;
}

const ConfigLocation::Argument &ConfigLocation::getAlias() const
{
	return (alias);
}

bool ConfigLocation::hasAlias() const
{
	return (!alias.empty());
}

const ConfigLocation::ArgumentList &ConfigLocation::getLimitExcept() const
{
	return (limitExcept);
}
