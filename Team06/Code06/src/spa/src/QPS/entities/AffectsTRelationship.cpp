#include "AffectsTRelationship.h"

AffectsTRelationship::AffectsTRelationship(vector<Parameter> &ps) {
  if (!validateSyntax(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::AFFECTST;
  params = ps;
}

bool AffectsTRelationship::validateParams() {
  if (!Parameter::isStatementRef(params[0])) {
    return false;
  }
  if (!Parameter::isStatementRef(params[1])) {
    return false;
  }
  return true;
}

bool AffectsTRelationship::validateSyntax(vector<Parameter> &ps) {
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