#ifndef SPA_QPS_PARAMETER_H
#define SPA_QPS_PARAMETER_H
#include <string>
#include <unordered_map>
#include "qps/QPSGrammarUtils.h"
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
    ParameterType getType() const;
	Parameter(string, string);
	Parameter(string, ParameterType);
	Parameter(const Parameter&);
	Parameter();
	static bool isEntityRef(Parameter&);
	static bool isStatementRef(Parameter&);
    static bool isDsgEntity(Parameter&);
	bool isUncheckedSynonym();
	void updateSynonymType(ParameterType);
    string getTypeString() const;
    bool isEqualTo(Parameter);
    static ParameterType guessParameterType(string);
private:
	const static unordered_map<string, ParameterType> stringToTypeMap;
	static ParameterType stringToType(string);
	string value;
	ParameterType type;
};

#endif