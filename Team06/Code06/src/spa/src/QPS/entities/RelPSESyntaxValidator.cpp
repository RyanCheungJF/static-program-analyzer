#include "RelPSESyntaxValidator.h"

bool RelPSESyntaxValidator::validate(Relationship &rel) {
  vector<Parameter> ps = rel.getParameters();
  if (ps.size() != 2) {
    return false;
  }
  if (!Parameter::isSyntacticStatementRef(ps[0]) &&
      !Parameter::isProcedure(ps[0])) {
    return false;
  }
  if (!Parameter::isSyntacticEntityRef(ps[1])) {
    return false;
  }
  return true;
}

RelPSESyntaxValidator::RelPSESyntaxValidator() {}
