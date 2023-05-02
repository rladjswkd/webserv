#include "ConfigLocation.hpp"

void ConfigLocation::setAlias(Argument list)
{
	alias = list;
}

void ConfigLocation::setLimitExcept(ArgumentList list)
{
	limitExcept = list;
}

const ConfigLocation::Argument &ConfigLocation::getAlias()
{
	return (alias);
}

bool ConfigLocation::hasAlias()
{
	return (!alias.empty());
}

const ConfigLocation::ArgumentList &ConfigLocation::getLimitExcept()
{
	return (limitExcept);
}
