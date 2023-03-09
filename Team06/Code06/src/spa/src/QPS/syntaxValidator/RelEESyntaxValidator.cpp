#include "RelEESyntaxValidator.h"

bool RelEESyntaxValidator::validate(Relationship &rel) {
  vector<Parameter> ps = rel.getParameters();
  if (ps.size() != 2) {
    throw SyntaxException();
  }
  if (!Parameter::isSyntacticEntityRef(ps[0])) {
    throw SyntaxException();
  }
  if (!Parameter::isSyntacticEntityRef(ps[1])) {
    throw SyntaxException();
  }
  return true;
}

RelEESyntaxValidator::RelEESyntaxValidator() {}
