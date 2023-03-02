#include "FollowsTRelationship.h"

bool FollowsTRelationship::validateSyntax(vector<Parameter> ps) {
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

FollowsTRelationship::FollowsTRelationship(vector<Parameter> ps) {
  if (!validateSyntax(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::FOLLOWST;
  params = ps;
}

bool FollowsTRelationship::validateParams() {
  if (!Parameter::isStatementRef(params[0])) {
    return false;
  }
  if (!Parameter::isStatementRef(params[1])) {
    return false;
  }
  return true;
}