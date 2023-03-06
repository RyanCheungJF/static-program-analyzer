#include "ModifiesRelationship.h"

ModifiesRelationship::ModifiesRelationship(vector<Parameter> &ps) {
  if (!validateSyntaxStmtProcEntity(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::MODIFIES;
  params = ps;
}

bool ModifiesRelationship::validateParams() {
  if (params[0].getValue() == "_" || !Parameter::isStatementRef(params[0]) &&
                                         !Parameter::isProcedure(params[0])) {
    return false;
  }
  if (!Parameter::isEntityRef(params[1])) {
    return false;
  }
  return true;
}
