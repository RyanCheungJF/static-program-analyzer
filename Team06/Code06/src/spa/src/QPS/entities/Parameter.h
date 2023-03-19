#ifndef SPA_QPS_PARAMETER_H
#define SPA_QPS_PARAMETER_H
#include <string>
#include <unordered_map>
#include <unordered_set>

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

enum class AttributeType {
    PROCNAME,
    VARNAME,
    VALUE,
    STMTNO,
    NONE
};

class Parameter {
public:
    string getValue();
    ParameterType getType() const;
    Parameter(string, ParameterType);
    Parameter(string, ParameterType, AttributeType);
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
    static ParameterType stringToType(string);
    bool isUncheckedSynonym();
    bool hasValidAttributeType();
    void updateSynonymType(ParameterType);
    string getTypeString() const;
    bool operator==(const Parameter&) const;

    static ParameterType guessParameterType(string);

private:
    const static unordered_map<string, ParameterType> stringToTypeMap;
    const static unordered_map<string, AttributeType> stringToAttributeMap;
    const static unordered_map<ParameterType, unordered_set<AttributeType>> typeToAttributeTypes;
    static AttributeType stringToAttribute(string);
    string value;
    ParameterType type;
    AttributeType attribute;
};

#endif