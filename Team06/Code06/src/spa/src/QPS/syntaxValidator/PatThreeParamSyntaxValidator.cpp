#include "PatThreeParamSyntaxValidator.h"

bool PatThreeParamSyntaxValidator::validate(Pattern& p) {
    vector<string> exprSpecs = p.getExprSpecs();
    if (!isWildCard(exprSpecs[0])) {
        throw SyntaxException();
    }
    if (!isWildCard(exprSpecs[1])) {
        throw SyntaxException();
    }
    if (!p.hasSyntacticEntityRef()) {
        throw SyntaxException();
    }
    return true;
}

PatThreeParamSyntaxValidator::PatThreeParamSyntaxValidator() {}
