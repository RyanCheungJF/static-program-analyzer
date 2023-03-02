#include "ParentTRelationship.h"

bool ParentTRelationship::validateSyntax(vector<Parameter> &ps) {
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

ParentTRelationship::ParentTRelationship(vector<Parameter> &ps) {
  if (!validateSyntax(ps)) {
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
