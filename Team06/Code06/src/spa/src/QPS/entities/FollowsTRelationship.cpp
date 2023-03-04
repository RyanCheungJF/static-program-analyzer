#include "FollowsTRelationship.h"

FollowsTRelationship::FollowsTRelationship(vector<Parameter> &ps) {
  if (!validateSyntaxStmtStmt(ps)) {
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