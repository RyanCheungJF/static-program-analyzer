#include "CallsRelationship.h"

CallsRelationship::CallsRelationship(vector<Parameter> &ps) {
  if (!validateSyntax(ps)) {
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

bool CallsRelationship::validateSyntax(vector<Parameter> &ps) {
  if (ps.size() != 2) {
    return false;
  }
  if (!Parameter::isSyntacticEntityRef(ps[0])) {
    return false;
  }
  if (!Parameter::isSyntacticEntityRef(ps[1])) {
    return false;
  }
  return true;
}
