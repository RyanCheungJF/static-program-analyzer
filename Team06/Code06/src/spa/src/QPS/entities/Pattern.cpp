#include "Pattern.h"

#include "../syntaxValidator/PatThreeParamSyntaxValidator.h"
#include "../syntaxValidator/PatTwoParamSyntaxValidator.h"

Pattern::Pattern() {
    evalPriority = 0;
}

Pattern::Pattern(const Pattern& p) {
    patternSyn = p.patternSyn;
    entRef = p.entRef;
    exprSpecs = p.exprSpecs;
    evalPriority = 0;
}

Pattern::Pattern(Parameter p, Parameter ent, vector<string>& es) {
    patternSyn = p;
    entRef = ent;
    exprSpecs = es;
    evalPriority = 0;
}

Pattern Pattern::makePattern(Parameter p, Parameter ent, vector<string>& es) {
    if (!p.isPatternSyn() && !(p.getType() == ParameterType::SYNONYM)) {
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
        processedEs.push_back(removeCharFromString(removeCharFromString(s, '\"'), ' '));
    }
    Pattern processedPat(p, ent, processedEs);
    return processedPat;
}

const Parameter& Pattern::getPatternSyn() const {
    return patternSyn;
}

const Parameter& Pattern::getEntRef() const {
    return entRef;
}

vector<string> Pattern::getExprSpecs() const {
    return exprSpecs;
}

vector<Parameter*> Pattern::getAllUncheckedSynonyms() {
    vector<Parameter*> synonyms;
    if (patternSyn.isUncheckedSynonym()) {
        synonyms.push_back(&patternSyn);
    }
    if (entRef.isUncheckedSynonym()) {
        synonyms.push_back(&entRef);
    }
    return synonyms;
}

bool Pattern::validateParams() {
    ParameterType patternType = patternSyn.getType();
    if (!entRef.isEntityRef()) {
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

ParameterType Pattern::getPatternType() const {
    return patternSyn.getType();
}

ParameterType Pattern::getEntRefType() const {
    return entRef.getType();
}

std::string Pattern::getEntRefValue() const {
    return entRef.getValue();
}

bool Pattern::hasSyntacticEntityRef() const {
    return entRef.isSyntacticEntityRef();
}

bool Pattern::operator==(const Pattern& p) const {
    return patternSyn == p.patternSyn && entRef == p.entRef && exprSpecs == p.exprSpecs;
}

bool Pattern::operator>(const Pattern& p) const {
    return evalPriority > p.evalPriority;
}

bool Pattern::operator<(const Pattern& p) const {
    return evalPriority < p.evalPriority;
}

double Pattern::calcPriority() {
    int wildcardCounter = 0;
    int fixedValCounter = 0;

    if (entRef.isFixedValue()) {
        fixedValCounter++;
    }
    else {
        wildcardCounter++;
    }

    for (int i = 0; i < exprSpecs.size(); i++) {
        if (isWildCard(exprSpecs.at(i))) {
            wildcardCounter++;
        }
        else {
            fixedValCounter++;
        }
    }

    double prio = wildcardCounter * AppConstants::wildcardWeight + fixedValCounter * AppConstants::fixedValWeight;
    this->evalPriority = prio;
    return prio;
}

shared_ptr<SyntaxValidator<Pattern>> patTwoParamVal =
    make_shared<PatTwoParamSyntaxValidator>(PatTwoParamSyntaxValidator());
shared_ptr<SyntaxValidator<Pattern>> patThreeParamVal =
    make_shared<PatThreeParamSyntaxValidator>(PatThreeParamSyntaxValidator());

const unordered_map<int, shared_ptr<SyntaxValidator<Pattern>>> Pattern::paramCountToValidatorMap = {
    {2, patTwoParamVal},
    {3, patThreeParamVal},
};