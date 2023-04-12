#include "ConfigLocation.hpp"

void ConfigLocation::setAlias(Directive directive)
{
	alias = directive;
}

void ConfigLocation::setLimitExcept(Directive directive)
{
	limitExcept = directive;
}
