#ifndef SPA_QPS_PARAMETER_H
#define SPA_QPS_PARAMETER_H
#include <string>
#include <unordered_map>

#include "../../../src/utils/AppConstants.h"
#include "QPS/QPSGrammarUtils.h"
#include "exceptions/InternalException.h"
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
    BOOLEAN,
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
    static Parameter makeParameter(string);
    static Parameter makeParameter(string, string);
    static bool isSyntacticEntityRef(Parameter&);
    static bool isEntityRef(Parameter&);
    static bool isSyntacticStatementRef(Parameter&);
    static bool isStatementRef(Parameter&);
    static bool isProcedure(Parameter&);
    static bool isDsgEntity(Parameter&);
    static bool isPatternSyn(Parameter&);
    static bool isFixedStringOrWildcard(Parameter&);
    static bool isFixedIntOrWildCard(Parameter&);
    bool isUncheckedSynonym();
    void updateSynonymType(ParameterType);
    string getTypeString() const;
    bool operator==(const Parameter&) const;

    static ParameterType guessParameterType(string);

private:
    const static unordered_map<string, ParameterType> stringToTypeMap;
    static ParameterType stringToType(string);
    string value;
    ParameterType type;
};

#endif