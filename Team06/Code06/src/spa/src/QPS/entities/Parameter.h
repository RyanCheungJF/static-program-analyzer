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
    string getValue() const;
    ParameterType getType() const;
    AttributeType getAttribute() const;
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
    static bool isComparable(Parameter&, Parameter&);
    bool isFixedInt();
    bool isFixedStringType();
    bool isVariable();
    bool isStmt();
    bool isWildcard();
    bool isAssign();
    bool isProcedureOnly();
    bool isConstant();
    static ParameterType stringToType(string);
    bool isUncheckedSynonym();
    bool hasValidAttributeType();
    bool hasAttribute();
    void updateSynonymType(ParameterType);
    void updateAttributeType(AttributeType);
    int getIntValue();
    string getTypeString() const;
    bool operator==(const Parameter&) const;
    static ParameterType guessParameterType(string);
    ParameterType getComparisonType();

private:
    const static unordered_map<string, ParameterType> stringToTypeMap;
    const static unordered_map<string, AttributeType> stringToAttributeMap;
    const static unordered_map<ParameterType, unordered_set<AttributeType>> typeToAttributeTypes;
    const static unordered_map<AttributeType, ParameterType> attributeToReturnType;
    static AttributeType stringToAttribute(string);
    string value;
    ParameterType type;
    AttributeType attribute;
};

template <> struct std::hash<Parameter> {
    std::size_t operator()(Parameter const& param) const {
        std::size_t h1 = std::hash<ParameterType>{}(param.getType());
        std::size_t h2 = std::hash<std::string>{}(param.getValue());
        std::size_t h3 = std::hash<AttributeType>{}(param.getAttribute());
        return ((h1 ^ (h2 << 1)) >> 1) ^ (h3 << 1);
    }
};

#endif