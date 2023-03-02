#ifndef SPA_QPS_RELATIONSHIP_H
#define SPA_QPS_RELATIONSHIP_H
#include "Parameter.h"
#include <string>
#include <vector>

using namespace std;

enum class RelationshipType {
  FOLLOWS,
  FOLLOWST,
  PARENT,
  PARENTT,
  USES,
  MODIFIES,
  RELATIONSHIP,
  UNKNOWN
};

class Relationship {
public:
  static shared_ptr<Relationship> makeRelationship(string type,
                                                   vector<Parameter> params);
  virtual bool validateParams() = 0;
  RelationshipType type;
  vector<Parameter> params;
  Relationship();
  Relationship(const Relationship &);
  vector<Parameter *> getAllUncheckedSynonyms();
  vector<Parameter> getParameters();
  RelationshipType getType();
  bool operator==(const Relationship &) const;

private:
  static const unordered_map<string, RelationshipType> stringToTypeMap;
  static RelationshipType stringToType(string);
};

#endif // !SPA_QPS_RELATIONSHIP_H
