#include "Comparison.h"

#include "QPS/syntaxValidator/CompSyntaxValidator.h"

Comparison::Comparison() {
    op = ComparisonOperator::EQUALS;
    evalPriority = 0;
}

Comparison::Comparison(ComparisonOperator o, Parameter leftP, Parameter rightP) {
    op = o;
    leftParam = leftP;
    rightParam = rightP;
    evalPriority = calcPriority();
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

bool Comparison::operator==(const Comparison& c) const {
    return leftParam == c.leftParam && rightParam == c.rightParam && op == c.op;
}

bool Comparison::operator>(const Comparison& c) const {
    return evalPriority > c.evalPriority;
}

bool Comparison::operator<(const Comparison& c) const {
    return evalPriority < c.evalPriority;
}

double Comparison::getPriority() {
    return evalPriority;
}

double Comparison::calcPriority() {
    int wildcardCounter = 0;
    int fixedValCounter = 0;

    if (leftParam.isFixedValue()) {
        fixedValCounter++;
    }
    else {
        wildcardCounter++;
    }

    if (rightParam.isFixedValue()) {
        fixedValCounter++;
    }
    else {
        wildcardCounter++;
    }

    double prio = wildcardCounter * AppConstants::wildcardWeight + fixedValCounter * AppConstants::fixedValWeight;
    return prio;
}