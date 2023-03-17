#include "Pattern.h"

#include "../syntaxValidator/PatThreeParamSyntaxValidator.h"
#include "../syntaxValidator/PatTwoParamSyntaxValidator.h"

Pattern::Pattern() {}

Pattern::Pattern(const Pattern& p) {
    patternSyn = p.patternSyn;
    entRef = p.entRef;
    exprSpecs = p.exprSpecs;
}

Pattern::Pattern(Parameter p, Parameter ent, vector<string>& es) {
    patternSyn = p;
    entRef = ent;
    exprSpecs = es;
}

Pattern Pattern::makePattern(Parameter p, Parameter ent, vector<string>& es) {
    if (!Parameter::isPatternSyn(p) && !(p.getType() == ParameterType::SYNONYM)) {
        throw SyntaxException();
    }
    int paramCount = 1 + es.size();
    auto iter = paramCountToValidatorMap.find(paramCount);
    if (iter == paramCountToValidatorMap.end()) {
        throw SyntaxException();
    }
    shared_ptr<SyntaxValidator<Pattern>> validator = iter->second;
    Pattern pat(p, ent, es);
    validator->validate(pat);
    vector<string> processedEs;
    for (string s : es) {
        processedEs.push_back(removeCharFromString(s, '\"'));
    }
    Pattern processedPat(p, ent, processedEs);
    return processedPat;
}

Parameter* Pattern::getPatternSyn() {
    return &patternSyn;
}

Parameter* Pattern::getEntRef() {
    return &entRef;
}

vector<string> Pattern::getExprSpecs() {
    return exprSpecs;
}

bool Pattern::validateParams() {
    ParameterType patternType = patternSyn.getType();
    if (!Parameter::isEntityRef(entRef)) {
        return false;
    }
    switch (patternType) {
    case ParameterType::ASSIGN:
        return exprSpecs.size() == 1;
    case ParameterType::WHILE:
        return exprSpecs.size() == 1 && isWildCard(exprSpecs[0]);
    case ParameterType::IF:
        return exprSpecs.size() == 2 && isWildCard(exprSpecs[0]) && isWildCard(exprSpecs[1]);
    default:
        return false;
    }
}

ParameterType Pattern::getPatternType() {
    return patternSyn.getType();
}

shared_ptr<SyntaxValidator<Pattern>> patTwoParamVal =
    make_shared<PatTwoParamSyntaxValidator>(PatTwoParamSyntaxValidator());
shared_ptr<SyntaxValidator<Pattern>> patThreeParamVal =
    make_shared<PatThreeParamSyntaxValidator>(PatThreeParamSyntaxValidator());

const unordered_map<int, shared_ptr<SyntaxValidator<Pattern>>> Pattern::paramCountToValidatorMap = {
    {2, patTwoParamVal},
    {3, patThreeParamVal},
};