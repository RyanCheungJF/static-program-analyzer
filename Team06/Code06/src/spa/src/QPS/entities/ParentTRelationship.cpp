#include "ParentTRelationship.h"

ParentTRelationship::ParentTRelationship(vector<Parameter> &ps) {
  if (!validateSyntaxStmtStmt(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::PARENTT;
  params = ps;
}

bool ParentTRelationship::validateParams() {
  if (!Parameter::isStatementRef(params[0])) {
    return false;
  }
  if (!Parameter::isStatementRef(params[1])) {
    return false;
  }
  return true;
}
