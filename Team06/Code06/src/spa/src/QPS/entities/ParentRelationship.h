#ifndef SPA_QPS_PARENTRELATIONSHIP_H
#define SPA_QPS_PARENTRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class ParentRelationship : public Relationship {
public:
  ParentRelationship(vector<Parameter> &);
  bool validateParams();
};

#endif // SPA_QPS_PARENTRELATIONSHIP_H