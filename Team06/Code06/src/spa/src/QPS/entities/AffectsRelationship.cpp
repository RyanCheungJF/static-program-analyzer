#include "AffectsRelationship.h"

AffectsRelationship::AffectsRelationship(vector<Parameter> &ps) {
  if (!validateSyntaxStmtStmt(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::AFFECTS;
  params = ps;
}

bool AffectsRelationship::validateParams() {
  if (!(params[0].getType() == ParameterType::ASSIGN) &&
      !(Parameter::isFixedIntOrWildCard(params[0]))) {
    return false;
  }
  if (!(params[1].getType() == ParameterType::ASSIGN) &&
      !(Parameter::isFixedIntOrWildCard(params[1]))) {
    return false;
  }
  return true;
}
