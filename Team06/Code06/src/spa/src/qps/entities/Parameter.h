#ifndef SPA_QPS_PARAMETER_H
#define SPA_QPS_PARAMETER_H
#include <string>
#include <unordered_map>
using namespace std;

enum class ParameterType {
	STMT,
	READ,
	PRINT,
	CALL,
	WHILE,
	IF,
	ASSIGN,
	VARIABLE,
	CONSTANT,
	PROCEDURE,
	SYNONYM,
	WILDCARD,
	FIXED_INT,
	FIXED_STRING,
	FIXED_STRING_WITH_WILDCARD,
	UNKNOWN
};

class Parameter {
public:
	string getValue();
	ParameterType getType();
	Parameter(string, string);
	Parameter(const Parameter&);
	Parameter();
private:
	const static unordered_map<string, ParameterType> stringToTypeMap;
	static ParameterType stringToType(string);
	string value;
	ParameterType type;
};

#endif