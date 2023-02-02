#include "Parameter.h"

string Parameter::getValue()
{
	return value;
}

ParameterType Parameter::getType() const
{
	return type;
}

Parameter::Parameter(string v, string t)
{

	type = stringToType(t);
	value = v;
}

Parameter::Parameter(string v, ParameterType t)
{
	value = v;
	type = t;
}

Parameter::Parameter(const Parameter& p)
{
	value = p.value;
	type = p.type;
}

Parameter::Parameter()
{
	type = ParameterType::UNKNOWN;
	value = "";
}

bool Parameter::isStatementRef(Parameter& p)
{
	return isStmtRef(p.getValue());
}

bool Parameter::isUncheckedSynonym()
{
	return type == ParameterType::SYNONYM;
}

void Parameter::updateSynonymType(ParameterType pt)
{
	if (type != ParameterType::SYNONYM) {
		throw - 1;
	}
	type = pt;
}

bool Parameter::isEntityRef(Parameter& p)
{
	return isEntRef(p.getValue());
}

//TODO: IF NOT FOUND, MAY WANT TO THROW ERROR
ParameterType Parameter::stringToType(string s)
{
	auto iter = Parameter::stringToTypeMap.find(s);
	if (iter == stringToTypeMap.end()) {
		return ParameterType::UNKNOWN;
	}
	return iter->second;
}

string Parameter::typeToString(ParameterType t) const {
    for(pair<string, ParameterType> item: stringToTypeMap) {
        if(item.second == t) {
            return item.first;
        }
    }
    return "None";
}

ParameterType Parameter::guessParameterType(string s)
{
	if (isSynonym(s)) {
		return ParameterType::SYNONYM;
	}
	if (isFixedString(s)) {
		return ParameterType::FIXED_STRING;
	}
	if (isInteger(s)) {
		return ParameterType::FIXED_INT;
	}
	if (isWildCard(s)) {
		return ParameterType::WILDCARD;
	}
	return ParameterType::UNKNOWN;
}

const unordered_map<string, ParameterType> Parameter::stringToTypeMap = {
	{"stmt", ParameterType::STMT},
	{"read", ParameterType::READ},
	{"print", ParameterType::PRINT},
	{"call", ParameterType::CALL},
	{"while", ParameterType::WHILE},
	{"if", ParameterType::IF},
	{"assign", ParameterType::ASSIGN},
	{"variable", ParameterType::VARIABLE},
	{"constant", ParameterType::CONSTANT},
	{"procedure", ParameterType::PROCEDURE},
	{"synonym", ParameterType::SYNONYM},
	{"wildcard", ParameterType::WILDCARD},
	{"fixed_int", ParameterType::FIXED_INT},
	{"fixed_string", ParameterType::FIXED_STRING},
	{"fixed_string_with_wildcard", ParameterType::FIXED_STRING_WITH_WILDCARD},
};