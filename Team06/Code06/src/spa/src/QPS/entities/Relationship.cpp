#include "Relationship.h"
#include "FollowsRelationship.h"
#include "FollowsTRelationship.h"
#include "ModifiesRelationship.h"
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

Relationship::Relationship() {
  type = RelationshipType::UNKNOWN;
  params;
}

RelationshipType Relationship::stringToType(string s) {
  auto iter = Relationship::stringToTypeMap.find(s);
  if (iter == stringToTypeMap.end()) {
    return RelationshipType::UNKNOWN;
  }
  return iter->second;
}

const unordered_map<string, RelationshipType> Relationship::stringToTypeMap = {
    {"Follows", RelationshipType::FOLLOWS},
    {"Follows*", RelationshipType::FOLLOWST},
    {"Parent", RelationshipType::PARENT},
    {"Parent*", RelationshipType::PARENTT},
    {"Uses", RelationshipType::USES},
    {"Modifies", RelationshipType::MODIFIES}};
