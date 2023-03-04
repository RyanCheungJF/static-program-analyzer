#include "Relationship.h"
#include "AffectsRelationship.h"
#include "AffectsTRelationship.h"
#include "CallsRelationship.h"
#include "CallsTRelationship.h"
#include "FollowsRelationship.h"
#include "FollowsTRelationship.h"
#include "ModifiesRelationship.h"
#include "NextRelationship.h"
#include "NextTRelationship.h"
#include "ParentRelationship.h"
#include "ParentTRelationship.h"
#include "UsesRelationship.h"

shared_ptr<Relationship>
Relationship::makeRelationship(string type, vector<Parameter> params) {
  RelationshipType rType = stringToType(type);
  switch (rType) {
  case RelationshipType::FOLLOWS:
    return make_shared<FollowsRelationship>(FollowsRelationship(params));
  case RelationshipType::FOLLOWST:
    return make_shared<FollowsTRelationship>(FollowsTRelationship(params));
  case RelationshipType::PARENT:
    return make_shared<ParentRelationship>(ParentRelationship(params));
  case RelationshipType::PARENTT:
    return make_shared<ParentTRelationship>(ParentTRelationship(params));
  case RelationshipType::USES:
    return make_shared<UsesRelationship>(UsesRelationship(params));
  case RelationshipType::MODIFIES:
    return make_shared<ModifiesRelationship>(ModifiesRelationship(params));
  case RelationshipType::NEXT:
    return make_shared<NextRelationship>(NextRelationship(params));
  case RelationshipType::NEXTT:
    return make_shared<NextTRelationship>(NextTRelationship(params));
  case RelationshipType::CALLS:
    return make_shared<CallsRelationship>(CallsRelationship(params));
  case RelationshipType::CALLST:
    return make_shared<CallsTRelationship>(CallsTRelationship(params));
  case RelationshipType::AFFECTS:
    return make_shared<AffectsRelationship>(AffectsRelationship(params));
  case RelationshipType::AFFECTST:
    return make_shared<AffectsTRelationship>(AffectsTRelationship(params));
  }
  throw SyntaxException();
}

Relationship::Relationship(const Relationship &r) {
  type = r.type;
  params = r.params;
}

vector<Parameter *> Relationship::getAllUncheckedSynonyms() {
  vector<Parameter *> synonyms;
  for (int i = 0; i < params.size(); i++) {
    if (params.at(i).isUncheckedSynonym()) {
      synonyms.push_back(&params.at(i));
    }
  }
  return synonyms;
}

vector<Parameter> Relationship::getParameters() { return params; }

bool Relationship::operator==(const Relationship &r) const {
  return type == r.type && params == r.params;
}

RelationshipType Relationship::getType() { return type; }

Relationship::Relationship() { type = RelationshipType::UNKNOWN; }

bool Relationship::validateSyntaxStmtStmt(vector<Parameter> &ps) {
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

bool Relationship::validateSyntaxEntityEntity(vector<Parameter> &ps) {
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

bool Relationship::validateSyntaxStmtProcEntity(vector<Parameter> &ps) {
  if (ps.size() != 2) {
    return false;
  }
  if (!Parameter::isSyntacticStatementRef(ps[0]) &&
      !Parameter::isProcedure(ps[0])) {
    return false;
  }
  if (!Parameter::isSyntacticEntityRef(ps[1])) {
    return false;
  }
  return true;
}

RelationshipType Relationship::stringToType(string s) {
  auto iter = Relationship::stringToTypeMap.find(s);
  if (iter == stringToTypeMap.end()) {
    return RelationshipType::UNKNOWN;
  }
  return iter->second;
}

const unordered_map<string, RelationshipType> Relationship::stringToTypeMap = {
    {AppConstants::FOLLOWS, RelationshipType::FOLLOWS},
    {AppConstants::FOLLOWST, RelationshipType::FOLLOWST},
    {AppConstants::PARENT, RelationshipType::PARENT},
    {AppConstants::PARENTT, RelationshipType::PARENTT},
    {AppConstants::USES, RelationshipType::USES},
    {AppConstants::MODIFIES, RelationshipType::MODIFIES},
    {AppConstants::NEXT, RelationshipType::NEXT},
    {AppConstants::NEXTT, RelationshipType::NEXTT},
    {AppConstants::CALLS, RelationshipType::CALLS},
    {AppConstants::CALLST, RelationshipType::CALLST},
    {AppConstants::AFFECTS, RelationshipType::AFFECTS},
    {AppConstants::AFFECTST, RelationshipType::AFFECTST}};
