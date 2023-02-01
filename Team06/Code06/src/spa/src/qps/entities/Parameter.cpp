#include "Parameter.h"

string Parameter::getValue()
{
	return value;
}

ParameterType Parameter::getType() const
{
	return type;
}

Parameter::Parameter(string t, string v)
{

	type = stringToType(t);
	value = v;
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