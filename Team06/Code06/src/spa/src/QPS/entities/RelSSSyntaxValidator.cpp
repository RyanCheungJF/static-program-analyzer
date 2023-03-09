#include "RelSSSyntaxValidator.h"

bool RelSSSyntaxValidator::validate(Relationship &rel)
{
  vector<Parameter> ps = rel.getParameters();
  if (ps.size() != 2) {
    return false;
  }
  if (!Parameter::isSyntacticStatementRef(ps[0])) {
    return false;
  }
  if (!Parameter::isSyntacticStatementRef(ps[1])) {
    return false;
  }
  return true;
}

RelSSSyntaxValidator::RelSSSyntaxValidator() {}
