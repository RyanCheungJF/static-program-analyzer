#ifndef SPA_QPS_PARENTRELATIONSHIP_H
#define SPA_QPS_PARENTRELATIONSHIP_H
#include "Relationship.h"
#include "exceptions/SyntaxException.h"

using namespace std;

class ParentRelationship : public Relationship {
public:
  ParentRelationship(vector<Parameter> &);
  bool validateParams();

private:
  bool validateSyntax(vector<Parameter> &);
};

#endif // SPA_QPS_PARENTRELATIONSHIP_H