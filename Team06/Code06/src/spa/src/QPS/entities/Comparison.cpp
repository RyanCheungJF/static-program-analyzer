#include "Comparison.h"

#include "QPS/syntaxValidator/CompSyntaxValidator.h"

Comparison::Comparison() {
    op = ComparisonOperator::EQUALS;
}

Comparison::Comparison(ComparisonOperator o, Parameter leftP, Parameter rightP) {
    op = o;
    leftParam = leftP;
    rightParam = rightP;
}

Parameter Comparison::getLeftParam() {
    return leftParam;
}

Parameter Comparison::getRightParam() {
    return rightParam;
}

ComparisonOperator Comparison::getOperator() {
    return op;
}

std::pair<ParameterType, ParameterType> Comparison::getParameterTypes() {
    return {leftParam.getType(), rightParam.getType()};
}

std::pair<std::string, std::string> Comparison::getParameterValues() {
    return {leftParam.getValue(), rightParam.getValue()};
}

std::pair<AttributeType, AttributeType> Comparison::getParameterAttributes() {
    return {leftParam.getAttribute(), rightParam.getAttribute()};
}

vector<Parameter*> Comparison::getAllUncheckedSynonyms() {
    vector<Parameter*> synonyms;
    if (leftParam.isUncheckedSynonym()) {
        synonyms.push_back(&leftParam);
    }
    if (rightParam.isUncheckedSynonym()) {
        synonyms.push_back(&rightParam);
    }
    return synonyms;
}

Comparison Comparison::makeComparison(string o, Parameter leftP, Parameter rightP) {
    CompSyntaxValidator syntaxVal;
    Comparison comp = Comparison(stringToOp(o), leftP, rightP);
    syntaxVal.validate(comp);
    return comp;
}

ComparisonOperator Comparison::stringToOp(string s) {
    auto iter = stringToOpMap.find(s);
    if (iter == stringToOpMap.end()) {
        throw SyntaxException();
    }
    return iter->second;
}

bool Comparison::hasValidComparisonTypes() {
    if (leftParam.getComparisonType() == ParameterType::UNKNOWN) {
        return false;
    }
    if (rightParam.getComparisonType() == ParameterType::UNKNOWN) {
        return false;
    }
    return true;
}

bool Comparison::validateParams() {
    if (!leftParam.hasValidAttributeType()) {
        return false;
    }
    if (!rightParam.hasValidAttributeType()) {
        return false;
    }
    if (!Parameter::isComparable(leftParam, rightParam)) {
        return false;
    }
    return true;
}

const unordered_map<string, ComparisonOperator> Comparison::stringToOpMap = {
    {AppConstants::OP_EQUALS, ComparisonOperator::EQUALS},
};