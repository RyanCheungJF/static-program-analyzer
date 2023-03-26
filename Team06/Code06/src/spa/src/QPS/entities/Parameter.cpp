#include "Parameter.h"

#include "../syntaxValidator/ParamSyntaxValidator.h"

string Parameter::getValue() const {
    return value;
}

ParameterType Parameter::getType() const {
    return type;
}

AttributeType Parameter::getAttribute() const {
    return attribute;
}

Parameter::Parameter() {
    type = ParameterType::UNKNOWN;
    value = "";
    attribute = AttributeType::NONE;
}

Parameter::Parameter(string v, ParameterType t) {
    value = v;
    type = t;
    attribute = AttributeType::NONE;
}

Parameter::Parameter(string v, ParameterType t, AttributeType a) {
    value = v;
    type = t;
    attribute = a;
}

Parameter::Parameter(const Parameter& p) {
    value = p.value;
    type = p.type;
    attribute = p.attribute;
}

Parameter Parameter::makeParameter(string val) {
    ParamSyntaxValidator paramSynVal;
    ParameterType type = guessParameterType(val);
    Parameter p(trim(removeCharFromString(val, '\"')), type);
    paramSynVal.validate(p);
    return p;
}

Parameter Parameter::makeParameter(string val, string attr) {
    ParamSyntaxValidator paramSynVal;
    ParameterType type = guessParameterType(val);
    AttributeType attribute = stringToAttribute(attr);
    Parameter p(trim(removeCharFromString(val, '\"')), type, attribute);
    paramSynVal.validate(p);
    return p;
}

bool Parameter::isSyntacticStatementRef(Parameter& p) {
    return p.type == ParameterType::SYNONYM || isStatementRef(p);
}

bool Parameter::isStatementRef(Parameter& p) {
    switch (p.type) {
    case ParameterType::STMT:
    case ParameterType::READ:
    case ParameterType::PRINT:
    case ParameterType::WHILE:
    case ParameterType::IF:
    case ParameterType::ASSIGN:
    case ParameterType::FIXED_INT:
    case ParameterType::WILDCARD:
    case ParameterType::CALL:
        return true;
    }
    return false;
}

bool Parameter::isProcedure(Parameter& p) {
    return p.type == ParameterType::PROCEDURE || p.type == ParameterType::FIXED_STRING;
}

bool Parameter::isDsgEntity(Parameter& p) {
    return isDesignEntity(p.getTypeString());
}

bool Parameter::isSyntacticEntityRef(Parameter& p) {
    return p.type == ParameterType::SYNONYM || isEntityRef(p) || isProcedure(p);
}

bool Parameter::isEntityRef(Parameter& p) {
    return p.type == ParameterType::VARIABLE || isFixedStringOrWildcard(p);
}

bool Parameter::isPatternSyn(Parameter& p) {
    return p.type == ParameterType::ASSIGN || p.type == ParameterType::WHILE || p.type == ParameterType::IF;
}

bool Parameter::isFixedStringOrWildcard(Parameter& p) {
    return p.type == ParameterType::FIXED_STRING || p.type == ParameterType::WILDCARD;
}

bool Parameter::isFixedIntOrWildCard(Parameter& p) {
    return p.type == ParameterType::FIXED_INT || p.type == ParameterType::WILDCARD;
}

bool Parameter::isFixedInt() {
    return type == ParameterType::FIXED_INT;
}

bool Parameter::isFixedString() {
    return type == ParameterType::FIXED_STRING;
}

bool Parameter::isVariable() {
    return type == ParameterType::VARIABLE;
}

bool Parameter::isStmt() {
    return type == ParameterType::STMT;
}

bool Parameter::isWildcard() {
    return type == ParameterType::WILDCARD;
}

bool Parameter::isAssign() {
    return type == ParameterType::ASSIGN;
}

bool Parameter::isProcedureOnly() {
    return type == ParameterType::PROCEDURE;
}

bool Parameter::isConstant() {
    return type == ParameterType::CONSTANT;
}

bool Parameter::isComparable(Parameter& p1, Parameter& p2) {
    ParameterType p1CompType = p1.getComparisonType();
    ParameterType p2CompType = p2.getComparisonType();
    if (p1CompType == ParameterType::UNKNOWN) {
        return false;
    }
    return p1CompType == p2CompType;
}

bool Parameter::isUncheckedSynonym() {
    return type == ParameterType::SYNONYM;
}

bool Parameter::hasValidAttributeType() {
    return Parameter::typeToAttributeTypes.at(type).count(attribute) == 1 || attribute == AttributeType::NONE;
}

bool Parameter::hasAttribute() {
    return attribute != AttributeType::NONE;
}

void Parameter::updateSynonymType(ParameterType pt) {
    if (type != ParameterType::SYNONYM) {
        throw InternalException("Error: Parameter.updateSynonymType parameter is not a synonym.");
    }
    type = pt;
}

ParameterType Parameter::stringToType(string s) {
    auto iter = Parameter::stringToTypeMap.find(s);
    if (iter == stringToTypeMap.end()) {
        throw SyntaxException();
    }
    return iter->second;
}

AttributeType Parameter::stringToAttribute(string s) {
    auto iter = Parameter::stringToAttributeMap.find(s);
    if (iter == stringToAttributeMap.end()) {
        throw SyntaxException();
    }
    return iter->second;
}

string Parameter::getTypeString() const {
    for (pair<string, ParameterType> item : stringToTypeMap) {
        if (item.second == this->getType()) {
            return item.first;
        }
    }
    return "None";
}

ParameterType Parameter::guessParameterType(string s) {
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

ParameterType Parameter::getComparisonType() {
    if (type == ParameterType::FIXED_INT || type == ParameterType::FIXED_STRING) {
        return type;
    }
    else {
        auto iter = Parameter::attributeToReturnType.find(attribute);
        if (iter == attributeToReturnType.end()) {
            return ParameterType::UNKNOWN;
        }
        return iter->second;
    }
}

bool Parameter::operator==(const Parameter& p) const {
    return type == p.type && value == p.value;
}

const unordered_map<string, ParameterType> Parameter::stringToTypeMap = {
    {AppConstants::STMT, ParameterType::STMT},           {AppConstants::READ, ParameterType::READ},
    {AppConstants::PRINT, ParameterType::PRINT},         {AppConstants::CALL, ParameterType::CALL},
    {AppConstants::WHILE, ParameterType::WHILE},         {AppConstants::IF, ParameterType::IF},
    {AppConstants::ASSIGN, ParameterType::ASSIGN},       {AppConstants::VARIABLE, ParameterType::VARIABLE},
    {AppConstants::CONSTANT, ParameterType::CONSTANT},   {AppConstants::PROCEDURE, ParameterType::PROCEDURE},
    {AppConstants::SYNONYM, ParameterType::SYNONYM},     {AppConstants::WILDCARD, ParameterType::WILDCARD},
    {AppConstants::FIXED_INT, ParameterType::FIXED_INT}, {AppConstants::FIXED_STRING, ParameterType::FIXED_STRING},
    {AppConstants::BOOLEAN, ParameterType::BOOLEAN}};

const unordered_map<string, AttributeType> Parameter::stringToAttributeMap = {
    {AppConstants::PROCNAME, AttributeType::PROCNAME},
    {AppConstants::VARNAME, AttributeType::VARNAME},
    {AppConstants::VALUE, AttributeType::VALUE},
    {AppConstants::STMTNO, AttributeType::STMTNO},
};

const unordered_map<ParameterType, unordered_set<AttributeType>> Parameter::typeToAttributeTypes = {
    {ParameterType::STMT, {AttributeType::STMTNO}},
    {ParameterType::READ, {AttributeType::STMTNO, AttributeType::VARNAME}},
    {ParameterType::PRINT, {AttributeType::STMTNO, AttributeType::VARNAME}},
    {ParameterType::CALL, {AttributeType::STMTNO, AttributeType::PROCNAME}},
    {ParameterType::WHILE, {AttributeType::STMTNO}},
    {ParameterType::IF, {AttributeType::STMTNO}},
    {ParameterType::ASSIGN, {AttributeType::STMTNO}},
    {ParameterType::VARIABLE, {AttributeType::VARNAME}},
    {ParameterType::CONSTANT, {AttributeType::VALUE}},
    {ParameterType::PROCEDURE, {AttributeType::PROCNAME}},
    {ParameterType::BOOLEAN, {}},
    {ParameterType::SYNONYM, {}},
    {ParameterType::WILDCARD, {}},
    {ParameterType::FIXED_INT, {}},
    {ParameterType::FIXED_STRING, {}},
};

const unordered_map<AttributeType, ParameterType> Parameter::attributeToReturnType = {
    {AttributeType::PROCNAME, ParameterType::FIXED_STRING},
    {AttributeType::VARNAME, ParameterType::FIXED_STRING},
    {AttributeType::VALUE, ParameterType::FIXED_INT},
    {AttributeType::STMTNO, ParameterType::FIXED_INT},
};