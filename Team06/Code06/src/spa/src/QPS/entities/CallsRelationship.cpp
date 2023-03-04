#include "CallsRelationship.h"

CallsRelationship::CallsRelationship(vector<Parameter> &ps) {
  if (!validateSyntaxEntityEntity(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::CALLS;
  params = ps;
}

bool CallsRelationship::validateParams() {
  if (params[0].getType() != ParameterType::PROCEDURE &&
      !Parameter::isFixedStringOrWildcard(params[0])) {
    return false;
  }
  if (params[1].getType() != ParameterType::PROCEDURE &&
      !Parameter::isFixedStringOrWildcard(params[1])) {
    return false;
  }
  return true;
}


