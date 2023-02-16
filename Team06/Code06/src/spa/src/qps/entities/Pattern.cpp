#include "Pattern.h"

Pattern::Pattern()
{
}

Pattern::Pattern(const Pattern& p)
{
	synAssign = p.synAssign;
	entRef = p.entRef;
	pattern = p.pattern;
}

Pattern::Pattern(Parameter sa, Parameter er, string p)
{
	synAssign = sa;
	entRef = er;
	pattern = p;
}

Parameter* Pattern::getSynAssign()
{
	return &synAssign;
}

Parameter* Pattern::getEntRef()
{
	return &entRef;
}

bool Pattern::validateParams()
{
	return Parameter::isPatternSyn(synAssign) && Parameter::isEntityRef(entRef);
}
