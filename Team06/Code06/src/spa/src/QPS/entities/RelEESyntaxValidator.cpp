#include "RelEESyntaxValidator.h"

bool RelEESyntaxValidator::validate(Relationship &rel) {
  vector<Parameter> ps = rel.getParameters();
  if (ps.size() != 2) {
    return false;
  }
  if (!Parameter::isSyntacticEntityRef(ps[0])) {
    return false;
  }
  if (!Parameter::isSyntacticEntityRef(ps[1])) {
    return false;
  }
  return true;
}

RelEESyntaxValidator::RelEESyntaxValidator() {}
