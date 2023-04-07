#include "PatTwoParamSyntaxValidator.h"

bool PatTwoParamSyntaxValidator::validate(Pattern& p) {
    vector<string> exprSpecs = p.getExprSpecs();
    if (!isExprSpec(exprSpecs[0])) {
        throw SyntaxException();
    }
    if (!p.hasSyntacticEntityRef()) {
        throw SyntaxException();
    }
    return true;
}

PatTwoParamSyntaxValidator::PatTwoParamSyntaxValidator() {}
