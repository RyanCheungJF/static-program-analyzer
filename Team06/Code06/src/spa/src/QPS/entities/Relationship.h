#ifndef SPA_QPS_RELATIONSHIP_H
#define SPA_QPS_RELATIONSHIP_H
#include "./utils/AppConstants.h"
#include "Parameter.h"
#include "../syntaxValidator/SyntaxValidator.h"
#include <string>
#include <vector>
#include <unordered_set>

using namespace std;

enum class RelationshipType {
  FOLLOWS,
  FOLLOWST,
  PARENT,
  PARENTT,
  USES,
  MODIFIES,
  NEXT,
  NEXTT,
  CALLS,
  CALLST,
  AFFECTS,
  AFFECTST,
  UNKNOWN
};

class Relationship {
public:
  static shared_ptr<Relationship> makeRelationship(string type,
                                                   vector<Parameter> params);
  bool validateParams();
  RelationshipType type;
  vector<Parameter> params;
  Relationship();
  Relationship(const Relationship &);
  Relationship(RelationshipType, vector<Parameter> &);
  vector<Parameter *> getAllUncheckedSynonyms();
  vector<Parameter> getParameters();
  RelationshipType getType();
  bool operator==(const Relationship &) const;

private:
  static const unordered_map<string, RelationshipType> stringToTypeMap;
  static const unordered_map<RelationshipType,
                             shared_ptr<SyntaxValidator<Relationship>>>
      typeToSyntaxValidatorMap;
  static const unordered_map<RelationshipType,
                             vector<unordered_set<ParameterType>>>
      typeToParameterTypes;
  static RelationshipType stringToType(string);
};

#endif // !SPA_QPS_RELATIONSHIP_H
