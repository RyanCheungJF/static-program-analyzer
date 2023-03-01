#include "Pattern.h"

Pattern::Pattern()
{
}

Pattern::Pattern(const Pattern& p)
{
    patternSyn = p.patternSyn;
	entRef = p.entRef;
	pattern = p.pattern;
    ifsParam = p.ifsParam;
}

Pattern::Pattern(Parameter ps, Parameter er, string p)
{
    patternSyn = ps;
	entRef = er;
	pattern = p;
}

Pattern::Pattern(Parameter ps, Parameter er, string p, Parameter ip)
{
    patternSyn = ps;
    entRef = er;
    pattern = p;
    ifsParam = ip;
}

Parameter* Pattern::getPatternSyn()
{
	return &patternSyn;
}

Parameter* Pattern::getEntRef()
{
	return &entRef;
}

bool Pattern::validateParams()
{
    ParameterType patternType = patternSyn.getType();
    bool validSyntax = true;
    switch(patternType) {
        case ParameterType::ASSIGN:
            validSyntax = ifsParam.getType() == ParameterType::UNKNOWN;
            break;
        case ParameterType::WHILE:
            validSyntax = ifsParam.getType() == ParameterType::UNKNOWN && pattern == "_";
            break;
        case ParameterType::IF:
            validSyntax = ifsParam.getType() == ParameterType::WILDCARD && pattern == "_";
            break;
        default:
            break;
    }
    if (!validSyntax) {
        throw SyntaxException();
    }
	return Parameter::isPatternSyn(patternSyn) && Parameter::isEntityRef(entRef);
}
