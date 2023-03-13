#include "PatTwoParamSyntaxValidator.h"

bool PatTwoParamSyntaxValidator::validate(Pattern &p) {
  vector<string> exprSpecs = p.getExprSpecs();
  if (!isExprSpec(exprSpecs[0])) {
    throw SyntaxException();
  }
  if (!Parameter::isSyntacticEntityRef(*p.getEntRef())) {
    throw SyntaxException();
  }
  return true;
}

PatTwoParamSyntaxValidator::PatTwoParamSyntaxValidator() {}
