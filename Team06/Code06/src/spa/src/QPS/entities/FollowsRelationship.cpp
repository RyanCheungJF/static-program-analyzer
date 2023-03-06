#include "FollowsRelationship.h"

FollowsRelationship::FollowsRelationship(vector<Parameter> &ps) {
  if (!validateSyntaxStmtStmt(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::FOLLOWS;
  params = ps;
}

bool FollowsRelationship::validateParams() {
  if (!Parameter::isStatementRef(params[0])) {
    return false;
  }
  if (!Parameter::isStatementRef(params[1])) {
    return false;
  }
  return true;
}
