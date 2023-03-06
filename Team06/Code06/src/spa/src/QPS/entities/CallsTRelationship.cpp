#include "CallsTRelationship.h"

CallsTRelationship::CallsTRelationship(vector<Parameter> &ps) {
  if (!validateSyntaxEntityEntity(ps)) {
    throw SyntaxException();
  }
  type = RelationshipType::CALLST;
  params = ps;
}

bool CallsTRelationship::validateParams() {
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
