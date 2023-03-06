#ifndef SPA_QPS_RELATIONSHIP_H
#define SPA_QPS_RELATIONSHIP_H
#include "./utils/AppConstants.h"
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
  virtual bool validateParams() = 0;
  RelationshipType type;
  vector<Parameter> params;
  Relationship();
  Relationship(const Relationship &);
  vector<Parameter *> getAllUncheckedSynonyms();
  vector<Parameter> getParameters();
  RelationshipType getType();
  bool operator==(const Relationship &) const;
  bool validateSyntaxStmtStmt(vector<Parameter> &);
  bool validateSyntaxEntityEntity(vector<Parameter> &);
  bool validateSyntaxStmtProcEntity(vector<Parameter> &);

private:
  static const unordered_map<string, RelationshipType> stringToTypeMap;
  static RelationshipType stringToType(string);
  static const int NUM_OF_PARAMS = 2;
};

#endif // !SPA_QPS_RELATIONSHIP_H
